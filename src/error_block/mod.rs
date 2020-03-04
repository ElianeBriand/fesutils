
#[cfg(test)]
#[path = "./unit_tests.rs"]
mod error_block_unit_tests;


use ndarray::prelude::*;
use crate::fes_grid_parser::Header;
use noisy_float::prelude::{r64, R64};
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

use std::thread;
use std::time::Duration;

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


fn process_block_cvw_store_data(first_line: &str, rest_of_block: &str, process_argument: &mut (&Header, &mut  ArrayD::<R64>, &mut u64) ) -> () { 
    let (ref header,
        ref mut traj_data,
        ref mut count) = &mut *process_argument;
    let mut rest_lines = rest_of_block.lines();
    let mut tokens: Vec<&str> = first_line.split_whitespace().collect();
    loop {
        if tokens.len() != 0 {
            if ! tokens[0].starts_with("#") {
                if tokens.len() >= (header.n_vars + 2) {
                    let mut in_range = true;
                    for i in 1..(header.n_vars+2) { // this iterate over vars + weight
                        let value = &tokens[i].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse value: {} ", &tokens[i]);
                        process::exit(1); });
                        if i < header.n_vars+1 {
                            let var_min: f64 = header.var_mins[i-1];
                            let var_max: f64 = header.var_maxs[i-1];
                            if value < &var_min || value > &var_max {
                                //print!("{} {} < {} < {}\n", i, var_min, value, var_max );
                                in_range = false;
                                break
                            }
                        }
                        traj_data[[**count as usize,i-1]] = r64(*value);
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



fn do_histanalysis_for_blocksize(pb: &ProgressBar, blocksize: u64, complete_traj_data: &ArrayD::<R64>) {
    let num_block = complete_traj_data.len() / blocksize as usize;

    for i in 0..100 {
        thread::sleep(Duration::from_millis(2));
        pb.inc(1);
        pb.tick();
    }

    pb.tick();
}

fn do_block_analysis(complete_traj_data: &ArrayD::<R64>, blocksizes: &Vec<u64>) {
    let pb = ProgressBar::new((blocksizes.len() * 100).try_into().unwrap());
    let progress_style = ProgressStyle::default_bar()
    .template("{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] ({eta})")
    .progress_chars("#>-").tick_chars("|/-\\");
    pb.set_style(progress_style.clone());
    pb.tick();

    for (i, blocksize) in blocksizes.iter().enumerate() {
        
        pb.set_position(i.try_into().unwrap());
        do_histanalysis_for_blocksize(&pb,*blocksize,complete_traj_data);
        pb.println(format!("Done {}", i));
        pb.tick();
    }


}


fn read_cvwhgt_file(cvwhgt_path_str: &str, header: &Header, dims_vec: &Vec::<usize>, fes_grid: &ArrayD::<R64>) -> ()  {

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

    let mut sum_weight_grid = ArrayD::<R64>::zeros(IxDyn(dims_vec));
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

    println!("\n\nTrajectory summary:");
    println!("Total lines       : {:>10}",  count_total_line);
    println!("In-range lines    : {:>10}  ({:>5.2}%)",  count_useful_line, ((count_useful_line as f64)/((count_total_line+1) as f64)*100.0) );

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
    traj_data_point_dims.push(header.n_vars + 1);

    let mut complete_traj_data = ArrayD::<R64>::zeros(IxDyn(&traj_data_point_dims));

    let mut count: u64 = 0;
    let mut process_arguments = ( header, &mut complete_traj_data, &mut count);
    
    cvw_reader.seek(SeekFrom::Start(0)).unwrap();
    blockprocessing_bufreader(&mut cvw_reader, cvw_size.try_into().unwrap(),  &mut process_block_cvw_store_data, &mut process_arguments);

    if count != count_useful_line {
        println!("WARNING: in-range count from phase 2/5 is {:>10}, but {:>10} for phase 3/5. Check that no other process change the file while this run",count_useful_line, count);
    }

    println!(
        "{} Block histogram analysis",
        style("[4/5]").bold().dim()
    );


    do_block_analysis(&complete_traj_data, &vec![1000,2000,3000,4000]);

    println!(
        "{} Final analysis",
        style("[5/5]").bold().dim()
    );


}





/// Toplevel function for running a error block analysis from required filepath
pub fn do_error_block(fesgrid_path_str: &str, cvwhgt_path_str: &str, ff_col: &str) {
    println!("\nFES grid file        : {}", fesgrid_path_str);
    println!("Weighted colvar file : {}", cvwhgt_path_str);
    print!("\n");

    let start_instant = Instant::now();

	println!(
        "{} Reading FES grid file",
        style("[1/5]").bold().dim()
    );
	
	let (header, dims_vec, fes_grid) = fes_grid_parser::fn_parse_fes_grid(fesgrid_path_str, ff_col);



    read_cvwhgt_file(cvwhgt_path_str, &header, &dims_vec, &fes_grid);

    println!(
        "\n{} (Duration: {})",
        style("Done !").bold(), HumanDuration(start_instant.elapsed())
    );

}

