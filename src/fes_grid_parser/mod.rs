//use rayon::prelude::*;

use std::fs::File;
use std::io::{BufRead, BufReader,Read, Seek, SeekFrom};
use std::process;
use std::convert::TryInto;
//use std::time::{Instant};


use console::{style};
use indicatif::{ProgressBar, ProgressStyle};


fn reconstruct_first_line(remainder: &str, block: &str) -> (String, usize) {
	let mut line = String::from(remainder);
	let end_of_the_line_idx = block.find("\n").unwrap();
	line += &block[..end_of_the_line_idx];
	return (line, end_of_the_line_idx+1);
}

fn parse_fesgrid_line(line: &str) -> u32 {
	let tokens: Vec<&str> = line.split_whitespace().collect();
	if tokens.len() == 0 {
		return 0;
	}
	return 1;
}


fn process_block(line_remainder: &str, current_block: &str) -> () {
	let (first_line, start_of_rest) = reconstruct_first_line(line_remainder,current_block);
	let _rest_of_block = &current_block[start_of_rest..];
	parse_fesgrid_line(&first_line);
/*
	let res: Vec<_> = rest_of_block
			.par_lines()
			.map(|line| parse_fesgrid_line(line)).collect();
*/
}



pub struct Header {
    pub fields: Vec<String>,
    pub idx_bias: Option<usize>,
    pub idx_var: Vec<usize>
}


impl Default for Header {
    fn default() -> Header {
        Header {
            fields: Vec::new(),
            idx_bias: None,
            idx_var: Vec::new()
        }
    }
}


pub fn parse_header<R: Read>(buf_reader: &mut BufReader<R>, ff_col: &str) -> Header {
	let header = Header {..Default::default()};
	for line_res in buf_reader.lines() {
		let line = line_res.unwrap();
		if line.len() == 0 {
			continue
		}

		if line.starts_with("#") {
			// do parsing
			if line.starts_with("#! FIELDS") {
				let tokens: Vec<_> = line.split_whitespace().collect();
				let _fields_tokens = &tokens[2..];

			}else if line.starts_with("#! SET normalisation") {

			} else if line.starts_with("#! SET min_") {

			} else if line.starts_with("#! SET max_") {

			}else {
				println!("Unexpected header line: {}", line);
			}
		} else {
			break; // Stop parsing header at first non-# line
		}

	}

	// Check if header is complete
	if header.idx_bias == None {
		println!("Cannot find the free energy column in the grid ! (column name \"{}\")",ff_col);
		println!("Use --fes_column_name <colname> to specify a column name");
		println!("Or maybe the header is missing/malformed ?");
		process::exit(2);
	}
	return header;
}


pub fn fn_parse_fes_grid(fesgrid_path_str: &str, ff_col: &str) {
		let fesgrid_file = File::open(fesgrid_path_str).unwrap_or_else(|err| {
        println!("Cannot open FES grid file {}: {}",fesgrid_path_str, err);
        process::exit(1);
    });

	let fesgrid_size = fesgrid_file.metadata().unwrap().len();
    const BUFF_CAPACITY: usize = 1024 * 1024 * 10;
    let mut fesgrid_reader = BufReader::with_capacity(BUFF_CAPACITY, fesgrid_file);


    //let start_instant = Instant::now();
    let progress_style = ProgressStyle::default_bar()
        .template("{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] {bytes}/{total_bytes} ({eta})")
        .progress_chars("#>-").tick_chars("|/-\\");

	println!(
        "      {} Reading header",
        style("--").bold().dim()
    );


	let _header = parse_header(&mut fesgrid_reader, ff_col);

    fesgrid_reader.seek(SeekFrom::Start(0)).unwrap();

    let first_pass_progress = ProgressBar::new(fesgrid_size);
    first_pass_progress.set_style(progress_style);
	first_pass_progress.tick();

    let mut size_processed = 0;
    let mut line_remainder = String::new();
	loop {
		first_pass_progress.set_position(size_processed.try_into().unwrap());
        first_pass_progress.tick();

        let buffer = fesgrid_reader.fill_buf().unwrap_or_else(|err| {
        println!("Error while reading FES grid file {}: {}",fesgrid_path_str, err);
        process::exit(1);
    	});
    	let len_read = buffer.len();
        if len_read == 0 {
            break;
        }
        // process data here


        let buffer_string = String::from_utf8(buffer.to_vec()).expect("Unexpected error while decoding to String");
		let idx_last_newline = buffer_string.rfind("\n");

		match idx_last_newline{
			None => { // No newline: the buffer does not contain a complete line
				line_remainder += &buffer_string;
				continue
			}, 
			Some(idx) => {
				// Exclude last incomplete line from being processed
				let current_block = &buffer_string[..idx];
				// but keep it to reconstruct it in the next block
				let next_line_remainder = &buffer_string[idx..];
				// Call processing function
				process_block(&line_remainder, current_block);
				line_remainder = String::from(next_line_remainder);
			}
		}

        fesgrid_reader.consume(len_read);
        size_processed += len_read;
    }

    first_pass_progress.finish_and_clear();
}