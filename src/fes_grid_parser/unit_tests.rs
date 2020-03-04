use crate::fes_grid_parser;

use approx::abs_diff_eq;

use std::f64;


#[test]
fn t_extract_prefixed_varname() {
    let res = fes_grid_parser::extract_varname_from_prefixed("min_", "min_varname");
    assert_eq!(res.to_string(), "varname");
}

#[test]
fn t_reconstruct_first_line() {
    let (first_line, offset) = fes_grid_parser::reconstruct_first_line("BB", "B\nAAA");

    assert_eq!(first_line.to_string(), "BBB");
    assert_eq!(offset, 2);
}

#[test]
fn t_grid_bins() {
    // Even number of bins
    let minmax_margin = 0.5;
    let min_val = 1.0;
    let max_val = 4.0;
    let nbins = 3;
    let expected_spacing = (max_val - min_val)/ (nbins as f64);
    let expected_edges = [  min_val-minmax_margin,
                            min_val + (1 as f64)*expected_spacing ,
                            min_val + (2 as f64)*expected_spacing ,
                            max_val + minmax_margin ];

    let (spacing, bins_edges_vec) = fes_grid_parser::spacing_bin_edges_from_limits_nbins(min_val, max_val, nbins, minmax_margin);
    assert_eq!(bins_edges_vec.len(), nbins+1);
    abs_diff_eq!(spacing , expected_spacing, epsilon = 0.001);
    assert_eq!(&bins_edges_vec[..], &expected_edges[..]);

    // Odd number of bins
    let minmax_margin = 0.5;
    let min_val = 1.0;
    let max_val = 4.0;
    let nbins = 5;
    let expected_spacing = (max_val - min_val)/ (nbins as f64);
    let expected_edges = [  min_val-minmax_margin,
                            min_val + (1 as f64)*expected_spacing ,
                            min_val + (2 as f64)*expected_spacing ,
                            min_val + (3 as f64)*expected_spacing ,
                            min_val + (4 as f64)*expected_spacing ,
                            max_val + minmax_margin ];

    let (spacing, bins_edges_vec) = fes_grid_parser::spacing_bin_edges_from_limits_nbins(min_val, max_val, nbins, minmax_margin);
    assert_eq!(bins_edges_vec.len(), nbins+1);
    abs_diff_eq!(spacing, expected_spacing, epsilon = f64::EPSILON);
    assert_eq!(&bins_edges_vec[..], &expected_edges[..]);

    // One single bin
    let minmax_margin = 0.5;
    let min_val = 1.0;
    let max_val = 4.0;
    let nbins = 1;
    let expected_spacing = (max_val - min_val)/ (nbins as f64);
    let expected_edges = [  min_val-minmax_margin,
                            max_val + minmax_margin ];

    let (spacing, bins_edges_vec) = fes_grid_parser::spacing_bin_edges_from_limits_nbins(min_val, max_val, nbins, minmax_margin);
    assert_eq!(bins_edges_vec.len(), nbins+1);
    abs_diff_eq!(spacing, expected_spacing, epsilon = f64::EPSILON);
    assert_eq!(&bins_edges_vec[..], &expected_edges[..]);


}

