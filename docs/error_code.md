

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
