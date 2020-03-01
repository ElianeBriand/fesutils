extern crate clap;
extern crate rayon;

use clap::{Arg, App, SubCommand, value_t};


mod error_block;
mod fes_grid_parser;


const VERSION: Option<&'static str> = option_env!("CARGO_PKG_VERSION");



fn main() {

    println!("fesutils - Free Energy Surface post-processing");
	println!("v{}\n", VERSION.unwrap_or("ersion unknown"));

    let matches = App::new("")
        .arg(Arg::with_name("Log output")
                 .short("l")
                 .long("log")
                 .value_name("fes.log")
                 .takes_value(true)
                 .help("Log messages to file."))
        .arg(Arg::with_name("thread_num")
                 .short("t")
                 .long("thread")
                 .value_name("thread_num")
                 .takes_value(true)
                 .help("Number of thread for parallel processing"))
        .subcommand(SubCommand::with_name("block_error")
                  .about("Run block analysis to compute error bars on a FES")
                  .arg(Arg::with_name("Full FES grid file")
                      .short("g")
                      .long("fesgrid")
                      .value_name("fesgrid.dat")
                      .required(true)
                      .help("FES grid for the complete run(s). Output of DUMP_GRID after CONVERT_TO_FES."))
                  .arg(Arg::with_name("CV file with frame weight")
                      .short("w")
                      .long("cvweighted")
                      .value_name("COLVAR.dat")
                      .required(true)
                      .help("File with the CV value for each frame (or stride of frame), with frame weight (output of REWEITH_BIAS, REWEIGHT_METAD ...)"))
                    .arg(Arg::with_name("ff_column")
                      .long("fes_column_name")
                      .value_name("ff")
                      .required(false)
                      .help("Name of the column/variable for the free energy in the FES grid. Default to \"ff\""))
                  )

        .get_matches();

    let thr_num = value_t!(matches.value_of("thread_num"), usize).unwrap_or(4); 

    rayon::ThreadPoolBuilder::new().num_threads(thr_num).build_global().unwrap();

    println!("Using {} threads.", thr_num);

    if let Some(matches) = matches.subcommand_matches("block_error") {
    	let fesgrid_path_str =  matches.value_of("Full FES grid file")
    	.expect("Unexpectedly missing mandatory argument: fesgrid");
    	let cvwhgt_path_str =  matches.value_of("CV file with frame weight")
    	.expect("Unexpectedly missing mandatory argument: cvrwhgt");

    	let ff_col =  matches.value_of("ff_column").unwrap_or("ff");

        error_block::do_error_block(&fesgrid_path_str, &cvwhgt_path_str, &ff_col);
	}

}