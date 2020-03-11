
#[cfg(test)]
#[path = "./unit_tests.rs"]
mod error_block_unit_tests;


use crate::GeneralSettings;
use ndarray::prelude::*;
use ndarray::Zip;
use crate::fes_grid_parser::Header;
use noisy_float::prelude::{r64, R64, Float};

use crate::csv;


use crate::fes_grid_parser;

use console::{style};
use indicatif::{HumanDuration};

use std::time::{Instant};

use std::io::{BufReader, SeekFrom, Seek};

use std::process;
use std::fs::File;
use std::convert::TryInto;

use crate::fileblock_processing::blockprocessing_bufreader;
use crate::fes_grid_parser::BIG_UNREACHABLE_ENERGY;

use indicatif::{ProgressBar, ProgressStyle};




// TODO: Implement CVW header parsing
// Even if just to reject badly formated file

fn process_block_cvw_first_pass(first_line: &str, rest_of_block: &str, process_argument: &mut (&Header, &mut ArrayD::<R64>, &mut  ArrayD::<R64>, &mut u64, &mut u64) ) -> () { 
    let (ref header,
        ref mut sum_weight_grid,
        ref mut sample_count_grid,
        ref mut useful_line,
        ref mut total_line) = &mut *process_argument;
    let mut rest_lines = rest_of_block.lines();
    let mut tokens: Vec<&str> = first_line.split_whitespace().collect();
    loop {
        **total_line +=  1;
        if tokens.len() != 0 {
            if ! tokens[0].starts_with("#") {
                if tokens.len() >= (header.n_vars + 2) {
                    let mut indexes = Vec::<usize>::new();
                    let mut in_range = true;
                    for i in 1..(header.n_vars+1) {
                        let value = &tokens[i].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse value: {} ", &tokens[i]);
                        process::exit(1); });
                        let var_min: f64 = header.var_mins[i-1];
                        let var_max: f64 = header.var_maxs[i-1];
                        if value < &var_min || value > &var_max {
                            //print!("{} {} < {} < {}\n", i, var_min, value, var_max );
                            in_range = false;
                            break
                        }
                        let offset = (((value - var_min)/header.var_gridspacing[i-1]).floor()) as usize;
                        indexes.push(offset);
                    }
                    if in_range {
                        **useful_line +=  1;
                        let weight_f64 = &tokens[(header.n_vars+1)].parse::<f64>().unwrap_or_else(|_|{
                            println!("Cannot parse value: {} ", &tokens[(header.n_vars+1)]);
                            process::exit(1); });
                        let weight = r64(*weight_f64);
                        sum_weight_grid[IxDyn(&indexes)] += weight;
                        sample_count_grid[IxDyn(&indexes)] += 1.00;
                    }
                } else {
                    println!("Misformated line encoutered, and discarded.");
                }

            }
        }
        // get new tokens
        let tokens_option = rest_lines.next();
        if tokens_option == None {
            break
        }else {
            tokens = tokens_option.unwrap().split_whitespace().collect();
        }
    }
}


fn process_block_cvw_store_data(first_line: &str, rest_of_block: &str, process_argument: &mut (&Header, &mut  ArrayD::<u64>, &mut  Array1::<R64>, &mut u64, &Vec::<usize>) ) -> () { 
    let (ref header,
        ref mut traj_data_indexes,
        ref mut traj_data_weight,
        ref mut count,
        ref dims_vect) = &mut *process_argument;
    let mut rest_lines = rest_of_block.lines();
    let mut tokens: Vec<&str> = first_line.split_whitespace().collect();
    loop {
        if tokens.len() != 0 {
            if ! tokens[0].starts_with("#") {
                if tokens.len() >= (header.n_vars + 2) {
                    let mut in_range = true;

                    let weight_value = &tokens[header.n_vars+1].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse weight value: {} ", &tokens[header.n_vars+1]);
                        process::exit(1); });
                    traj_data_weight[**count as usize] = r64(*weight_value);
                    
                    for i in 1..(header.n_vars+1) { // this iterate over vars
                        let value = &tokens[i].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse var value: {} ", &tokens[i]);
                        process::exit(1); });

                        let var_min: f64 = header.var_mins[i-1];
                        let var_max: f64 = header.var_maxs[i-1];
                        if value < &var_min || value > &var_max {
                            //print!("{} {} < {} < {}\n", i, var_min, value, var_max );
                            in_range = false;
                            break
                        }
                        let offset = (((value - var_min)/header.var_gridspacing[i-1]).floor()) as usize;

                        if (offset >= dims_vect[i-1]) && in_range  {
                            println!("Value is marked in range but offset is greater than dimension: offset {}  dim {}",offset, dims_vect[i-1] );
                        }

                        traj_data_indexes[[**count as usize,i-1]] = offset.try_into().unwrap();
                    }
                    if in_range {
                        **count += 1;
                    }
                } else {
                    println!("Misformated line encoutered, and discarded.");
                }

            }
        }
        // get new tokens
        let tokens_option = rest_lines.next();
        if tokens_option == None {
            break
        }else {
            tokens = tokens_option.unwrap().split_whitespace().collect();
        }
    }
}



fn do_histanalysis_for_blocksize(blocksize: u64, traj_data_indexes: &ArrayD::<u64>, traj_data_weight : &Array1::<R64>, header: &Header, dims_vec: &Vec::<usize>, fes_total_present_bins : u64) -> R64 {
    let num_block = traj_data_weight.len() / blocksize as usize;

    println!("    # Block size {} : {} blocks #  ",blocksize, num_block);
    let pb = ProgressBar::new((num_block).try_into().unwrap());
    let progress_style = ProgressStyle::default_bar()
    .template("{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] ({eta}) | {msg}")
    .progress_chars("#>-").tick_chars("|/-\\");
    pb.set_style(progress_style.clone());
    pb.tick();


    let num_dims = traj_data_indexes.dim()[1];

    let mut dim_vec_blockaware = Vec::<usize>::new();
    dim_vec_blockaware.push(num_block);
    for dim in dims_vec {
        dim_vec_blockaware.push(*dim);
    }

    let total_number_of_bins: usize = dims_vec.iter().fold(1, |sum, &val| {sum * val});


    let mut avg_global_grid          = ArrayD::<R64>::zeros(IxDyn(&dim_vec_blockaware));
    let mut sample_count_global_grid = ArrayD::<R64>::zeros(IxDyn(&dim_vec_blockaware));
    let mut sampled_global_grid      = ArrayD::<R64>::zeros(IxDyn(&dim_vec_blockaware)); // Binary : 1.0: sampled / 0.0 not sampled

    let mut sqr_avg_avg_sampled        = ArrayD::<R64>::zeros(IxDyn(&dims_vec));
    let mut avg_sqravg_sampled        = ArrayD::<R64>::zeros(IxDyn(&dims_vec));
    let mut variance_avg_grid           = ArrayD::<R64>::zeros(IxDyn(&dims_vec));

    for i in 0..num_block {
        pb.set_message(&format!("Block {} of {}",i,num_block).to_string());
        pb.inc(1);
        pb.tick();
        let starting_index =       i * (blocksize as  usize);
        let ending_index   = (i + 1) * (blocksize as  usize);

        let mut sample_count_grid = ArrayD::<R64>::zeros(IxDyn(dims_vec));
        let mut sum_weight_grid = ArrayD::<R64>::zeros(IxDyn(dims_vec));


        for i in starting_index..ending_index {
            
            if i % 100 == 0{
                pb.tick();
            }

            let weight =  traj_data_weight[[i]];

            let mut indexes = Vec::<usize>::new();
            for dim in 0..num_dims {
                let indx_val = traj_data_indexes[[i, dim]].try_into().unwrap();
                if indx_val >= dims_vec[dim]  {
                    println!("Value is supposedly in range but offset is greater than dimension: offset {}  dim {}",indx_val, dims_vec[i] );
                }
                indexes.push(indx_val);
            }

            sum_weight_grid[IxDyn(&indexes)] += weight;
            sample_count_grid[IxDyn(&indexes)] += 1.0;
        }
        pb.tick();

        let mut avg_grid = sum_weight_grid / (&sample_count_grid + 0.0001);
        let mut sampled_grid = &sample_count_grid.mapv(|a| if a >= 1.0 {r64(1.0)} else {r64(0.0)});

        {
            Zip::from(&mut avg_grid)
            .and(&sample_count_grid)
            .apply(|avg, &sample_count| {
                *avg = if sample_count == 0.0 {r64(0.0)} else {*avg};
            });
        }


        avg_global_grid.slice_mut(s![i, .., .., ..]).assign(&avg_grid);
        sample_count_global_grid.slice_mut(s![i, .., .., ..]).assign(&sample_count_grid);
        sampled_global_grid.slice_mut(s![i, .., .., ..]).assign(&sampled_grid);
    }

    let avg_global_summed = &avg_global_grid.sum_axis(Axis(0));
    let sampled_global_summed = &sampled_global_grid.sum_axis(Axis(0));

    {
            Zip::from(&mut sqr_avg_avg_sampled)
            .and(avg_global_summed)
            .and(sampled_global_summed)
            .apply(|avg_avg, &avg_summed,  &sum_sampled| {
                *avg_avg = if sum_sampled > 0.0 {(avg_summed / sum_sampled).powi(2)} else {r64(0.0)};
            });
    }

    {
            Zip::from(&mut avg_sqravg_sampled)
            .and(avg_global_summed)
            .and(sampled_global_summed)
            .apply(|avg_sqravg, &avg_summed,  &sum_sampled| {
                *avg_sqravg = if sum_sampled > 0.0 {avg_summed.powi(2) / sum_sampled} else {r64(0.0)};
            });
    }


    {
            Zip::from(&mut variance_avg_grid)
            .and(&avg_sqravg_sampled)
            .and(&sqr_avg_avg_sampled)
            .apply(|var_avg, &avg_sqravg,  &sqr_avg_avg| {
                *var_avg = avg_sqravg - sqr_avg_avg;
            });
    }

    let count_sampled = sampled_global_grid.sum_axis(Axis(0));
    let in_all_blocks_count : u64 = count_sampled.mapv(|e| e > r64((blocksize) as f64 - 0.1)).mapv(|e| if e {1} else {0}).sum();
    let in_all_blocks_but_one_count : u64 = count_sampled.mapv(|e| e > r64((blocksize - 1) as f64 - 0.1)).mapv(|e| if e {1} else {0}).sum();
    let in_all_blocks_but_two_count : u64 = count_sampled.mapv(|e| e > r64((blocksize - 2) as f64 - 0.1)).mapv(|e| if e {1} else {0}).sum();

    let sum_var = variance_avg_grid.sum();
    let mean_denominator : R64 = variance_avg_grid.mapv(|e| e > 0.0).mapv(|e| if e {r64(1.0)} else {r64(0.0)}).sum();

    let mean_variance = sum_var / mean_denominator;

    let two_sampled_voxel : u64 = count_sampled.mapv(|e| e > r64(2.0)).mapv(|e| if e {1} else {0}).sum();
    let three_sampled_voxel : u64 = count_sampled.mapv(|e| e > r64(3.0)).mapv(|e| if e {1} else {0}).sum();
    let ten_sampled_voxel : u64 = count_sampled.mapv(|e| e > r64(10.0)).mapv(|e| if e {1} else {0}).sum();


    pb.println(format!("    Voxel seen in at least                Total possible voxel          Voxel in FES profile   "));
    pb.println(format!("        2 blocks                      :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        two_sampled_voxel, total_number_of_bins,
                                                                                        (two_sampled_voxel as f64/total_number_of_bins as f64)*100.0,
                                                                                        two_sampled_voxel,fes_total_present_bins,
                                                                                        (two_sampled_voxel as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("        3 blocks                      :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        three_sampled_voxel, total_number_of_bins,
                                                                                        (three_sampled_voxel as f64/total_number_of_bins as f64)*100.0,
                                                                                        three_sampled_voxel,fes_total_present_bins,
                                                                                        (three_sampled_voxel as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("       10 blocks                      :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        ten_sampled_voxel, total_number_of_bins,
                                                                                        (ten_sampled_voxel as f64/total_number_of_bins as f64)*100.0,
                                                                                        ten_sampled_voxel,fes_total_present_bins,
                                                                                        (ten_sampled_voxel as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("      all blocks but 2                :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        in_all_blocks_but_two_count, total_number_of_bins,
                                                                                        (in_all_blocks_but_two_count as f64/total_number_of_bins as f64)*100.0,
                                                                                        in_all_blocks_but_two_count,fes_total_present_bins,
                                                                                        (in_all_blocks_but_two_count as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("      all blocks but 1                :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        in_all_blocks_but_one_count, total_number_of_bins,
                                                                                        (in_all_blocks_but_one_count as f64/total_number_of_bins as f64)*100.0,
                                                                                        in_all_blocks_but_one_count,fes_total_present_bins,
                                                                                        (in_all_blocks_but_one_count as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("      all blocks                      :  {:>9}/{:9} ({:4.2}%) {:>9}/{:9} ({:4.2}%)",
                                                                                        in_all_blocks_count, total_number_of_bins,
                                                                                        (in_all_blocks_count as f64/total_number_of_bins as f64)*100.0,
                                                                                        in_all_blocks_count,fes_total_present_bins,
                                                                                        (in_all_blocks_count as f64/fes_total_present_bins as f64)*100.0 ));
    pb.println(format!("    Mean variance                     : {:10.5} (kj/mol)^2",mean_variance));
    pb.println(format!("         Mean variance denominator    : {:10.5} ", mean_denominator));

    pb.println("    Blocksize finished\n\n");
    pb.finish_and_clear();

    return mean_variance;
}


fn do_block_analysis(traj_data_indexes: &ArrayD::<u64>,
                     traj_data_weight : &Array1::<R64>,
                     blocksizes: &Vec<u64>,
                     header: &Header,
                     dims_vec: &Vec::<usize>,
                     fes_total_present_bins : u64) -> Vec::<(u64, R64)> {

    let num_blocksize = blocksizes.len();

    let mut results = Vec::<(u64, R64)>::new();

    for (i, blocksize) in blocksizes.iter().enumerate() {
        let block_mean_variance = do_histanalysis_for_blocksize(*blocksize,traj_data_indexes,traj_data_weight, header, dims_vec, fes_total_present_bins);
        results.push((*blocksize, block_mean_variance));
    }

    return results;

}

fn block_size_schedule(num_frame: u64, num_blocksize: u64) -> Vec<u64> {
    let increment: f64 = ((10*10)/num_blocksize) as f64;
    let mut binsizes = Vec::<u64>::new();
    print!("Auto-generated blocksizes: ");
    for i in 1..num_blocksize+1  {
        let binsize = (1.0/((i as f64) * increment) * (num_frame as f64)) as u64;
        binsizes.push(binsize);
        print!("{}, ", binsize);
    }
    print!("\n");
    return binsizes;
}



fn read_cvwhgt_file(settings: &GeneralSettings, cvwhgt_path_str: &str, header: &Header, dims_vec: &Vec::<usize>, fes_grid: &ArrayD::<R64>) -> ()  {

    println!(
        "{} Reading weighted frame data: complete trajectory",
        style("[2/5]").bold().dim()
    );

    let cvw_file = File::open(cvwhgt_path_str).unwrap_or_else(|err| {
        println!("Cannot open reweigthed CV file {}: {}",cvwhgt_path_str, err);
        process::exit(1);
    });

    let cvw_size = cvw_file.metadata().unwrap().len();
    const BUFF_CAPACITY: usize = 1024 * 1024;
    let mut cvw_reader = BufReader::with_capacity(BUFF_CAPACITY, cvw_file);

    let mut sample_count_grid = ArrayD::<R64>::zeros(IxDyn(dims_vec));
    let mut sum_weight_grid = ArrayD::<R64>::zeros(IxDyn(dims_vec));
    let mut count_useful_line: u64 = 0;
    let mut count_total_line: u64 = 0;

    let mut process_arguments = ( header, &mut sum_weight_grid, &mut sample_count_grid, &mut count_useful_line, &mut count_total_line );

    blockprocessing_bufreader(&mut cvw_reader, cvw_size.try_into().unwrap(),  &mut process_block_cvw_first_pass, &mut process_arguments);
    
    let total_number_of_bins: usize = dims_vec.iter().fold(1, |sum, &val| {sum * val});
    let sampled_bins : u64 = sample_count_grid.mapv(|e| e > 1.0).mapv(|e| if e {1} else {0}).sum();
    let twice_sampled_bins : u64 = sample_count_grid.mapv(|e| e > 2.0).mapv(|e| if e {1} else {0}).sum();
    let thrice_sampled_bins : u64 = sample_count_grid.mapv(|e| e > 3.0).mapv(|e| if e {1} else {0}).sum();
    let ten_sampled_bins : u64 = sample_count_grid.mapv(|e| e > 10.0).mapv(|e| if e {1} else {0}).sum();

    let fes_total_present_bins : u64 = fes_grid.mapv(|e| e < BIG_UNREACHABLE_ENERGY).mapv(|e| if e {1} else {0}).sum();

    let estimated_traj_ram_space_mb = (count_useful_line*((header.n_vars + 1) as u64)*64)/(8*1024*1024);

    println!("\n\nTrajectory summary:");
    println!("Total lines       : {:>10}",  count_total_line);
    println!("In-range lines    : {:>10}  ({:>5.2}%)",  count_useful_line, ((count_useful_line as f64)/((count_total_line+1) as f64)*100.0) );
    println!(" Est. RAM space   : {:>10} Mb  ",  estimated_traj_ram_space_mb);

    println!("\nComplete histogram summary:");
    println!("Total bins        : {:>10}",  total_number_of_bins );
    println!("Sampled bins      : {:>10}  ({:>5.2}%)",  sampled_bins, ((sampled_bins as f64)/((total_number_of_bins+1) as f64)*100.0) );
    println!(" 2-Sampled bins   : {:>10}  ({:>5.2}%)",  twice_sampled_bins, ((twice_sampled_bins as f64)/((total_number_of_bins+1) as f64)*100.0) );
    println!(" 3-Sampled bins   : {:>10}  ({:>5.2}%)",  thrice_sampled_bins, ((thrice_sampled_bins as f64)/((total_number_of_bins+1) as f64)*100.0) );
    println!("10-Sampled bins   : {:>10}  ({:>5.2}%)",  ten_sampled_bins, ((ten_sampled_bins as f64)/((total_number_of_bins+1) as f64)*100.0) );

    println!("\nFES grid summary:");
    println!("Bins in profile   : {:>10}  ({:>5.2}%)",  fes_total_present_bins, ((fes_total_present_bins as f64)/((total_number_of_bins+1) as f64)*100.0) );
    println!("\n");


    println!(
        "{} Reading weighted frame data: storing in-range points",
        style("[3/5]").bold().dim()
    );

    let mut traj_data_point_dims = Vec::<usize>::new();
    traj_data_point_dims.push(count_useful_line.try_into().unwrap());
    traj_data_point_dims.push(header.n_vars);

    let mut traj_data_indexes = ArrayD::<u64>::zeros(IxDyn(&traj_data_point_dims));
    let mut traj_data_weight = Array1::<R64>::zeros((count_useful_line.try_into().unwrap(),));

    let mut count: u64 = 0;
    let mut process_arguments = ( header, &mut traj_data_indexes, &mut traj_data_weight, &mut count, dims_vec);
    
    cvw_reader.seek(SeekFrom::Start(0)).unwrap();
    blockprocessing_bufreader(&mut cvw_reader, cvw_size.try_into().unwrap(),  &mut process_block_cvw_store_data, &mut process_arguments);

    if count != count_useful_line {
        println!("WARNING: in-range count from phase 2/5 is {:>10}, but {:>10} for phase 3/5. Check that no other process change the file while this run",count_useful_line, count);
    }

    println!(
        "{} Block histogram analysis",
        style("[4/5]").bold().dim()
    );

    let blocksizes = block_size_schedule(count_useful_line, 10);
    let results = do_block_analysis(&traj_data_indexes, &traj_data_weight,   &blocksizes, header, dims_vec, fes_total_present_bins);

    let filename = settings.output_stem.clone() + "_error_blockanalysis.csv";
    println!("Writing error block analysis results to file: {}", filename);
    let mut wtr = csv::Writer::from_path(filename).unwrap();
    
    wtr.write_record(&["blocksize", "variance"]).unwrap();
    for (bs, var) in results {
        wtr.write_record(&[bs.to_string(), var.to_string()]).unwrap();
    }
    wtr.flush().unwrap();


    println!(
        "{} Final analysis",
        style("[5/5]").bold().dim()
    );


}





/// Toplevel function for running a error block analysis from required filepath
pub fn do_error_block(settings: &GeneralSettings, fesgrid_path_str: &str, cvwhgt_path_str: &str) {
    println!("\nFES grid file        : {}", fesgrid_path_str);
    println!("Weighted colvar file : {}", cvwhgt_path_str);
    print!("\n");

    let start_instant = Instant::now();

	println!(
        "{} Reading FES grid file",
        style("[1/5]").bold().dim()
    );
	
    let ref ff_col = settings.ff_column;

	let (header, dims_vec, fes_grid) = fes_grid_parser::fn_parse_fes_grid(fesgrid_path_str, ff_col);



    read_cvwhgt_file(settings, cvwhgt_path_str, &header, &dims_vec, &fes_grid);

    println!(
        "\n{} (Duration: {})",
        style("Done !").bold(), HumanDuration(start_instant.elapsed())
    );

}

