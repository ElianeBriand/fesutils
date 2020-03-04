

use indicatif::{ProgressBar, ProgressStyle};

use std::io::{BufReader, BufRead, Read};
use std::convert::TryInto;
use std::process;

/// Reconstruct a full line from the remainder of the last block and the current block
///
/// Return the line, and the offset to be used in the current block to not double count the first
fn reconstruct_first_line(remainder: &str, block: &str) -> (String, usize) {
    let mut line = String::from(remainder);
    let end_of_the_line_idx = block.find("\n").unwrap();
    line += &block[..end_of_the_line_idx];
    return (line, end_of_the_line_idx+1);
}

pub fn blockprocessing_bufreader<R: Read, T>(bufreader: &mut BufReader<R>, total_size: usize,  processing_func: &mut dyn FnMut(&str, &str,&mut T) -> (), process_arguments: &mut T) -> () {
    let first_pass_progress = ProgressBar::new(total_size.try_into().unwrap());
    let progress_style = ProgressStyle::default_bar()
    .template("{spinner:.green} [{elapsed_precise}] [{bar:40.cyan/blue}] {bytes}/{total_bytes} ({eta})")
    .progress_chars("#>-").tick_chars("|/-\\");
    first_pass_progress.set_style(progress_style);
    first_pass_progress.tick();

    let mut size_processed: usize = 0;
    let mut line_remainder = String::new();
    loop {
        first_pass_progress.set_position(size_processed.try_into().unwrap());
        first_pass_progress.tick();

        let buffer = bufreader.fill_buf().unwrap_or_else(|err| {
        println!("Error while block reading file grid file {}", err);
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
                let (first_line, start_of_rest) = reconstruct_first_line(&line_remainder,current_block);
                let rest_of_block = &current_block[start_of_rest..];
                // process_arguments = (header: &Header, fes_grid: &mut ArrayD::<R64>)
                processing_func(&first_line, rest_of_block, process_arguments);
                line_remainder = String::from(next_line_remainder);
            }
        }

        bufreader.consume(len_read);
        size_processed += len_read;
    }

    first_pass_progress.finish_and_clear();
}