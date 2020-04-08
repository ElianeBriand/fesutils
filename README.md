fesutils is a utility to post-process Plumed-computed free energy surface data, including final bias reweighting and error
 bar computation using block analysis.
 
While some of this can be done with Plumed itself, the specialisation of this tools may allow for speed-ups in some cases.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.com/ElianeBriand/fesutils.svg?branch=master)](https://travis-ci.com/ElianeBriand/fesutils)
[![codecov](https://codecov.io/gh/ElianeBriand/fesutils/branch/master/graph/badge.svg)](https://codecov.io/gh/ElianeBriand/fesutils)



# Build
Initialise submodules and cmake build:
```
git submodule update --init --recursive
```

Build `fmt` (tensor compiler):
```
cd bundled-deps
mkdir build-fmt && cd build-fmt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../fmt
make && make install
cd ../..
```

CMake build of the main project:
```
mkdir build && cd build
cmake ..
make
```

# Dependencies:

- Boost
- Eigen
- Threading Building Block (intel tbb)

- indicator (bundled in submodule)


