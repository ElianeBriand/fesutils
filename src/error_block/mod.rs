extern crate rayon;

use crate::fes_grid_parser;


use console::{style};
use indicatif::{HumanDuration};

use std::time::{Instant};








pub fn do_error_block(fesgrid_path_str: &str, cvwhgt_path_str: &str, ff_col: &str) {
    println!("\nFES grid file        : {}", fesgrid_path_str);
    println!("Weighted colvar file : {}", cvwhgt_path_str);
    print!("\n");

    let start_instant = Instant::now();

	println!(
        "{} Reading FES grid file",
        style("[1/4]").bold().dim()
    );
	
	fes_grid_parser::fn_parse_fes_grid(fesgrid_path_str, ff_col);

	println!(
        "{} Reading weighted frame data",
        style("[2/4]").bold().dim()
    );

    println!("Done running error block analysis in {}", HumanDuration(start_instant.elapsed()));

}

