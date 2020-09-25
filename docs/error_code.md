

# Error code 00001

The given .dat file does not contain enough information to create a grid. There need to be at least one variable
with the following requiered field attribute:
* Field min and max values
* Field nbins
* Field periodic

Typical `head` of a correct .dat grid file:
```
#! FIELDS mph_position.x mph_position.y mph_position.z metad_act_site.bias der_mph_position.x der_mph_position.y der_mph_position.z
#! SET min_mph_position.x -5.80
#! SET max_mph_position.x 18.80
#! SET nbins_mph_position.x  494
#! SET periodic_mph_position.x false
#! SET min_mph_position.y -2.35
#! SET max_mph_position.y 18.00
#! SET nbins_mph_position.y  409
#! SET periodic_mph_position.y false
#! SET min_mph_position.z -4.70
#! SET max_mph_position.z 13.90
#! SET nbins_mph_position.z  374
#! SET periodic_mph_position.z false
   -5.800000000   -2.350000000   -4.700000000    0.000000000    0.000000000    0.000000000    0.000000000
```

# Error code 00002

Boolean litteral value in .dat file must be one of the following: "true" or "false". This is case sensitive. Litteral zero
 and one ("1" and "0") are not adequate.
 
# Warning code 00003

This warning occurs when a line in a CV file contain more (or less) value that should be expected according to the header.
This sort of file corruption occurs often when the MD run have crashed, and it is restarted directly in the same file 
(Plumed directive `RESTART`). The offending line is ignored, and this is in most case fine - as the loss of data is minute.

# Error code 00004

The header seems to contain a zero or negative nbins attribute - nbins should be strictly positive as it correspond to the
number of bins in the grid.

# Error code 00005

A file corresponding with the given file name hint could not be found in the search directories. Add the directory
containing the file to the search_directory section of the runfile, for instance:

```yaml
search_directory:
    all: "/home/user/cluster_import/"
```


# Error code 00101

**Type**: Runfile structure error

The fes_data section should be structured as a map from the name of the particular data to its parameters, like so:

```yaml
fes_data:
    meac:
        name: "Methylacetate"
        cv_varnames:
            - "fragment_position.x"
            - "fragment_position.y"
            - "fragment_position.z"
        wall_bias_varnames:
            - "wall_lower.bias"
            - "wall_upper.bias"
        metad_bias_varname: "metad_position.bias"
        cv_filename_hint: "CV_fragment_meac"
        hills_filename_hint: "HILLS_fragment_meac"
    bzn:
        name: "Benzene"
        cv_varnames:
            - "fragment_position.x"
            - "fragment_position.y"
            - "fragment_position.z"
        wall_bias_varnames:
            - "wall_lower.bias"
            - "wall_upper.bias"
        metad_bias_varname: "metad_position.bias"
        cv_filename_hint: "CV_fragment_benzene"
        hills_filename_hint: "HILLS_fragment_benzene"

```

# Error code 00102

**Type**: Runfile structure error

For each input fes data, the following properties are mandatory:

 - `name`
 - `cv_varnames`
 - `cv_filename_hint`
 - `metad_bias_varname`
 - Either a `hills_filename_hint` or a `grid_filename_hint` (or both - grid will then be used)

Optional properties include:

- wall_bias_varnames

Example of minimal declaration:

```yaml
fes_data:
    meac:
        name: "Methylacetate"
        cv_varnames:
            - "fragment_position.x"
            - "fragment_position.y"
            - "fragment_position.z"
        metad_bias_varname: "metad_position.bias"
        cv_filename_hint: "CV_fragment_meac"
        hills_filename_hint: "HILLS_fragment_meac"
```

# Error code 00103

**Type**: Runfile structure error

The reweight section should be structured as a map from the name of the particular data to its parameters, like so:

```yaml
reweight:
    rw_all:
        method: reweight_no_interp
        wall_reached: discard
        data: [meac, bzn, pip]
```

The `data` section refers to shorthand label from the fes_data section, either a single value (`data: bzn`) or a 
sequence of values (`data: [meac, bzn, pip]`)

The `wall_reached` section refers to the behaviour when the simulation has run into biaising wall, ie non zero value in the
 variable specifies in the `wall_bias_varnames` section of `fes_data`.  Possible specifiers include:

- `discard` : ignore all steps where any wall is reached

The `method` section refers to the reweighting method.  Possible specifiers include:

- `reweight_no_interp` : use the value in the hills data without any interpolation (spline, ...)

# Error code 00104

**Type**: Runfile structure error

The steps section should be structured as a sequence of map:

```yaml
steps:
    - fes_data : [meac, bzn, pip]
    - reweight: rw_all
    - histogram: histo_all
    - reconstruction: mph_1point
    - reconstruction: mph_1conformer
    - reconstruction: mph_multiconformer
```

The map specifies a function of the program (reweight, histogram ...) with the parameter set to be run. The parameter sets
will have been defined in previous section bearing the function name.

