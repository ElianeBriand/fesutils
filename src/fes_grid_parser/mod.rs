
#[cfg(test)]
#[path = "./unit_tests.rs"]
mod fes_grid_parser_unit_tests;

//use rayon::prelude::*;

use ndarray::ArrayD;
use ndarray::IxDyn;

use std::fs::File;
use std::io::{BufRead, BufReader,Read, Seek, SeekFrom};
use std::process;
use std::convert::TryInto;
use std::str::FromStr;
//use std::time::{Instant};


use noisy_float::types::{R64, r64};

use console::{style};

use crate::fileblock_processing::blockprocessing_bufreader;


pub struct Header {
    pub fields: Vec<String>,
    pub idx_ff: Option<usize>,
    pub idx_var: Vec<usize>,
    pub idx_dff: Vec<usize>,
    pub normalization: Option<f64>,
    pub n_vars: usize,
    pub var_mins: Vec<f64>,
    pub var_maxs: Vec<f64>,
    pub var_nbins: Vec<usize>,
    pub var_gridspacing: Vec<f64>,
    pub var_binedges: Vec< Vec<R64> >,
}


impl Default for Header {
    fn default() -> Header {
        Header {
            fields: Vec::new(),
            idx_ff: None,
            idx_var: Vec::new(),
            idx_dff: Vec::new(),
            normalization: None,
            n_vars: 0,
            var_mins: Vec::new(),
            var_maxs: Vec::new(),
            var_nbins: Vec::new(),
            var_gridspacing: Vec::new(),
            var_binedges: Vec::new()
        }
    }
}


pub const BIG_UNREACHABLE_ENERGY : f64 = 10_000.0;



/// Process a block of line and feed into the fes_grid ndarray
fn process_block_fesgrid(first_line: &str, rest_of_block: &str, process_argument: &mut (&Header, &mut ArrayD::<R64>) ) -> () {
    let (ref header, ref mut fes_grid) = &mut *process_argument;
    let mut rest_lines = rest_of_block.lines();

    let spacing_epsilon = 0.0001;
    let mut tokens: Vec<&str> = first_line.split_whitespace().collect();
    loop {
        if tokens.len() != 0 {
            if ! tokens[0].starts_with("#") {
                if tokens.len() >= (header.n_vars + 1) {
                    let mut indexes = Vec::<usize>::new();
                    for i in 0..(header.n_vars) {
                        let value = &tokens[i].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse value: {} ", &tokens[i]);
                        process::exit(1); });
                        let offset = ((((value + spacing_epsilon) - header.var_mins[i])/header.var_gridspacing[i]).floor()) as usize;
                        indexes.push(offset);
                    }
                    let ff = &tokens[header.idx_ff.unwrap()].parse::<f64>().unwrap_or_else(|_|{
                        println!("Cannot parse value: {} ", &tokens[header.idx_ff.unwrap()]);
                        process::exit(1); });
                    let ff_clipped = if *ff > BIG_UNREACHABLE_ENERGY { BIG_UNREACHABLE_ENERGY } else {*ff}; // Remap infinities to very big
                    fes_grid[IxDyn(&indexes)] = r64(ff_clipped);
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






fn parse_or_exit<T: FromStr>(field: &str) -> T {
    field.parse::<T>().unwrap_or_else(|_|{
                    println!("Cannot parse value: {} ", field);
                    process::exit(1); })
}

/// Extract a name from a prefix names ("min_", "min_xxx"  --> "xxx")
fn extract_varname_from_prefixed(prefix: &str, field: &str) -> String {
    let var_name = field.get(prefix.len()..).unwrap();
    if var_name.len() == 0 {
        println!("Unexpected empty variable name in field: {}", field);
        process::exit(1);
    }
    return var_name.to_string();
}

/// For a given axis, given a number of bins, a minimum and a maximum, compute the edge of the histogram bins, and the spacing/width of bins
fn spacing_bin_edges_from_limits_nbins(var_min: f64, var_max: f64, nbins: usize, minmax_margin: f64) -> (f64, Vec<R64>) {
    let range = r64(var_max) - r64(var_min);
    let spacing = range/r64(nbins as f64);
    let num_edges = nbins + 1;
    let mut bin_edges : Vec<R64> = Vec::new();

    bin_edges.push( r64(var_min - minmax_margin));
    for i in 1..(num_edges-1) {
        bin_edges.push( r64(var_min) + r64(i as f64) * spacing);
    }
    bin_edges.push(r64(var_max + minmax_margin));

    return (spacing.raw(), bin_edges);
}

/// Parse a Plumed FES grid header
pub fn parse_header<R: Read>(buf_reader: &mut BufReader<R>, ff_col: &str) -> Header {
	let mut header = Header {..Default::default()};
	for line_res in buf_reader.lines() {
		let line = line_res.unwrap();
		if line.len() == 0 {
			continue
		}

		if line.starts_with("#") {
            let tokens: Vec<_> = line.split_whitespace().collect();

			if line.starts_with("#! FIELDS") {
				
				let fields_tokens = &tokens[2..];
                for field in fields_tokens {
                    &mut header.fields.push(field.to_string());
                }
                for (i, field) in &mut header.fields.iter().enumerate() {
                    if field == "ff" {
                        header.idx_ff = Some(i);
                    }else if field.starts_with("dff_") {
                        &mut header.idx_dff.push(i);
                    }else {
                        // Probably a variable
                        &mut header.idx_var.push(i);
                    }
                }


			}else if line.starts_with("#! SET normalisation") {
                let norm_amount_str = tokens.get(3).unwrap_or_else(||{
                    println!("Missing normalization value on line: {}", line);
                    process::exit(1);});
                let norm_amount = parse_or_exit::<f64>(norm_amount_str);
                header.normalization = if norm_amount != 1.00 { Some(norm_amount) } else {None};
			} else if line.starts_with("#! SET m") {
                let var_field_str = tokens.get(2).unwrap();
                let var_name = extract_varname_from_prefixed("min_",var_field_str);
                let idx_var = header.fields.iter()
                                .position(|x| x == &var_name)
                                .unwrap_or_else(||{println!("Cannot find field with name {}", var_name);process::exit(1);});
                if (idx_var >  header.var_mins.len()) || (idx_var >  header.var_maxs.len()) {
                    println!("Encountered out-of-order record for variable {}. Check header of FES grid.",var_name );
                    process::exit(1);
                }
                let var_value_str = tokens.get(3).unwrap_or_else(||{println!("Missing minimal value at line {}", line);process::exit(1);});
                let var_value_float = parse_or_exit::<f64>(var_value_str);
                if line.starts_with("#! SET min_") {
                    &mut header.var_mins.push(var_value_float);
                } else if line.starts_with("#! SET max_") {
                    &mut header.var_maxs.push(var_value_float);
                } else {
                    println!("Unexpected header field name: {}",var_field_str);
                }
            } else if line.starts_with("#! SET nbins_") {
                let var_field_str = tokens.get(2).unwrap();
                let var_name = extract_varname_from_prefixed("nbins_",var_field_str);
                let idx_var = header.fields.iter()
                                .position(|x| x == &var_name)
                                .unwrap_or_else(||{println!("Cannot find field with name {}", var_name);process::exit(1);});
                if (idx_var >  header.var_mins.len()) || (idx_var >  header.var_maxs.len()) {
                    println!("Encountered out-of-order nbins_ record for variable {}. Check header of FES grid.",var_name );
                    process::exit(1);
                }
                let var_nbins_str = tokens.get(3).unwrap_or_else(||{println!("Missing minimal value at line {}", line);process::exit(1);});
                let var_value_int = parse_or_exit::<i32>(var_nbins_str);
                if var_value_int <= 0 {
                    println!("Incorrect header: number of bins should be positive, is {}",var_value_int);
                    process::exit(1);
                }
                &mut header.var_nbins.push(var_value_int.try_into().unwrap());
            } else if line.starts_with("#! SET periodic_") {
                let var_field_str = tokens.get(2).unwrap();
                let var_field_value_str = tokens.get(3).unwrap_or(&"");
                if var_field_value_str != &"false" {
                    println!("Warning: Periodic variable {} will be treated as non-periodic (functionality not implemented)",var_field_str);
                }
			} else {
				println!("Unexpected header line: {}", line);
			}
		} else {
			break; // Stop parsing header at first non-# line
		}

	}

	// Check if header is complete, and for consistency
    if &header.idx_var.len() != &header.idx_dff.len() {
        println!("Mismatch between variable and dff fields as declared in the header.\nPlease check grid file generation for mistake.");
    }else {
        for ((i, idx_var), idx_dff) in header.idx_var.iter().enumerate().zip(&header.idx_dff) {
            let var_str = * &header.fields.get(*idx_var).unwrap();
            let dff_str = * &header.fields.get(*idx_dff).unwrap();
            let min_val = * &header.var_mins.get(i).unwrap();
            let max_val = * &header.var_maxs.get(i).unwrap();
            let nbins_val = * &header.var_nbins.get(i).unwrap();
            if *dff_str != (String::from("dff_") + var_str) {
                println!("Mismatch between variable and dff fields as declared in the header: {} vs {}",var_str, dff_str );
            }
            let (bin_spacing, bins_edges) = spacing_bin_edges_from_limits_nbins(*min_val, *max_val, *nbins_val,0.1);
            header.var_gridspacing.push(bin_spacing);
            header.var_binedges.push(bins_edges);
            println!("           Var: {} | range: {:<8.3} - {:<8.3} | nbins {:5} - spacing {:<8.3}", var_str, min_val, max_val, nbins_val, bin_spacing);
        }
    }
	if header.idx_ff == None {
		println!("Cannot find the free energy column in the grid ! (column name \"{}\")",ff_col);
		println!("Use --fes_column_name <colname> to specify a column name");
		println!("Or maybe the header is missing/malformed ?");
		process::exit(2);
	}
    for idx in &header.idx_var {
        if idx >= &header.idx_ff.unwrap() {
            println!("Unexpected ordering of the variable: ff field before subsequent variable.");
            println!("Check FES grid headers");
            process::exit(2);
        }
    }
    if header.idx_ff.unwrap() != header.idx_var.len() {
            println!("Unexpected ordering of the variable: non-variable field before ff field ?");
            println!("Check FES grid headers");
            process::exit(2);
    }

    header.n_vars = header.var_nbins.len();
	return header;
}




/// Parse a Plumed FES grid: extract data from the header, returns the grid dimensions, and the data in a ArrayD
pub fn fn_parse_fes_grid(fesgrid_path_str: &str, ff_col: &str) -> (Header, Vec::<usize>,  ArrayD::<R64>)  {
	let fesgrid_file = File::open(fesgrid_path_str).unwrap_or_else(|err| {
        println!("Cannot open FES grid file {}: {}",fesgrid_path_str, err);
        process::exit(1);
    });

	let fesgrid_size = fesgrid_file.metadata().unwrap().len();
    const BUFF_CAPACITY: usize = 1024 * 1024 ;
    let mut fesgrid_reader = BufReader::with_capacity(BUFF_CAPACITY, fesgrid_file);


    //let start_instant = Instant::now();


	println!(
        "      {} Reading header",
        style("--").bold().dim()
    );


	let header = parse_header(&mut fesgrid_reader, ff_col);


    let mut dims_vec = Vec::<usize>::new();
    for nbins in &header.var_nbins {
        dims_vec.push(*nbins + 1);
    }
    let mut fes_grid = ArrayD::<R64>::zeros(IxDyn(&dims_vec));

    println!(
        "      {} Reading grid",
        style("--").bold().dim()
    );

    fesgrid_reader.seek(SeekFrom::Start(0)).unwrap();

    let mut process_arguments = ( &header, &mut fes_grid);

    blockprocessing_bufreader(&mut fesgrid_reader, fesgrid_size.try_into().unwrap(),  &mut process_block_fesgrid, &mut process_arguments);


    return (header, dims_vec, fes_grid);
}