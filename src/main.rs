extern crate clap;
extern crate num_traits;
extern crate rayon;
extern crate noisy_float;
extern crate approx;
extern crate csv;


use crate::fragment_reconstruct::do_fragment_reconstruct;

use clap::{Arg, App, SubCommand, value_t, AppSettings};


mod error_block;
mod fes_grid_parser;
mod fileblock_processing;
mod fragment_reconstruct;



const VERSION: Option<&'static str> = option_env!("CARGO_PKG_VERSION");


pub struct GeneralSettings {
    pub threads_num: usize,
    pub output_stem: String,
    pub ff_column: String
}


impl Default for GeneralSettings {
    fn default() -> GeneralSettings {
        GeneralSettings {
            threads_num: 1,
            output_stem: "fesutil_output".to_string(),
            ff_column: "ff".to_string()
        }
    }
}



fn main() {

    println!("fesutils - Free Energy Surface post-processing");
	println!("v{}\n", VERSION.unwrap_or("ersion unknown"));

    let matches = App::new("")
        .setting(AppSettings::SubcommandRequiredElseHelp)
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
        .arg(Arg::with_name("output_stem")
                 .short("o")
                 .long("output_stem")
                 .value_name("output_stem")
                 .required(false)
                 .takes_value(true)
                 .help("Stem for the output filenames (eg: \"output_1\" -> output_1.csv, output_1.dat ... )"))
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
                    .arg(Arg::with_name("output_stem")
                     .short("o")
                     .long("output_stem")
                     .value_name("output_stem")
                     .required(false)
                     .takes_value(true)
                     .help("Stem for the output filenames (eg: \"output_1\" -> output_1.csv, output_1.dat ... )"))
                  )
        .subcommand(SubCommand::with_name("fragment_reconstruct")
                  .about("Reconstruct FES from fragments FES")
                  .arg(Arg::with_name("Fragment FES grid file")
                      .short("g")
                      .long("fesgrid")
                      .value_name("fesgrid.dat")
                      .required(true)
                      .min_values(1)
                      .help("FES grid for each fragment. Accept multiple argument for multiple fragments."))
                    .arg(Arg::with_name("Relative conformation file")
                      .short("c")
                      .long("conformations")
                      .value_name("conformations.xyz")
                      .required(true)
                      .help("File containing the relative position of the fragment, for multiple conformations."))
                    .arg(Arg::with_name("output_stem")
                     .short("o")
                     .long("output_stem")
                     .value_name("output_stem")
                     .required(false)
                     .takes_value(true)
                     .help("Stem for the output filenames (eg: \"output_1\" -> output_1.csv, output_1.dat ... )"))
                  )
        .get_matches();

    let mut settings = GeneralSettings {..Default::default()};
    let thr_num = value_t!(matches.value_of("thread_num"), usize).unwrap_or(1); 
    settings.threads_num = thr_num;

    let o_stem: String = value_t!(matches.value_of("output_stem"), String).unwrap_or("fesutil_output".to_string()); 
    settings.output_stem = o_stem;

    rayon::ThreadPoolBuilder::new().num_threads(thr_num).build_global().unwrap();

    println!("Using {} threads.", thr_num);

    if let Some(matches) = matches.subcommand_matches("block_error") {
        let fesgrid_path_str =  matches.value_of("Full FES grid file")
        .expect("Unexpectedly missing mandatory argument: fesgrid");
        let cvwhgt_path_str =  matches.value_of("CV file with frame weight")
        .expect("Unexpectedly missing mandatory argument: cvrwhgt");

        let o_stem: String = value_t!(matches.value_of("output_stem"), String).unwrap_or("fesutil_output".to_string());
        if settings.output_stem != "fesutil_output".to_string() {
            println!("Output stem argument specifies more than once, subcommand argument used. (old value: \"{}\", new value: \"{}\")", settings.output_stem, o_stem);
        }
        settings.output_stem = o_stem;

        let ff_col =  matches.value_of("ff_column").unwrap_or("ff");
        settings.ff_column = ff_col.to_string();

        error_block::do_error_block(&settings, &fesgrid_path_str, &cvwhgt_path_str);
    }else if let Some(matches) = matches.subcommand_matches("fragment_reconstruct") {

        let fesgrid_paths_str: Vec<&str> = matches.values_of("files")
        .expect("Unexpectedly missing mandatory argument: --fesgrid : Fragment FES grid file")
        .collect();

        let rel_conf_path =  matches.value_of("Relative conformation file")
        .expect("Unexpectedly missing mandatory argument: --conformations : Relative conformation file");

        do_fragment_reconstruct(&settings, fesgrid_paths_str, rel_conf_path);
    } else {

    }

}