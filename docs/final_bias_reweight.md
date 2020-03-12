final_bias_reweight - Reweight frames from a MetaD trajectory using a final bias grid.

# Important notes
If ranges are specified for the variable, all frames outside these ranges will be discarded. 
This can be useful if you have used UPPER and LOWER_WALLS, which this subcommand cannot reweight : specify
a range that strictly fits within the boundaries defined by the walls.

All other bias (not contained on the grid file) are not taken into account.

# Usages

```
fesutils final_bias_reweight --cvfile CVFILE.dat --outfile CVREWEIGHTED.dat --metadgrid METADGRID.dat --ranges_min 0.0,1.2,3.1 --ranges_max  10.0,4.0,5.0  
```

**CVFILE.dat** - File with the value of the CV for each frame to be reweighted.

`head CVFILE.dat` ypically look something like this:
```
#! FIELDS time mph_position.x mph_position.y mph_position.z
 0.000000 5.153229 8.120083 4.590947
 0.002000 5.152779 8.119988 4.591062
 0.004000 5.152216 8.120186 4.590817
 0.006000 5.151726 8.120554 4.590228
 0.008000 5.151549 8.120990 4.589488
 0.010000 5.151855 8.121302 4.588833
 0.012000 5.152663 8.121293 4.588459
 0.014000 5.153845 8.120868 4.588499
 0.016000 5.155179 8.120089 4.589015
```

and has been produced by a plumed config lines like:
```
PRINT ARG=mph_position.x,mph_position.y,mph_position.z FILE=DTMPH_COARSE_METAD STRIDE=1
```


**METADGRID.dat** - Grid file written with WGRID_FILE in the Plumed MetaD directive


