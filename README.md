fesutils is a utility to post-process Plumed-computed free energy surface data, including final bias reweighting and error
 bar computation using block analysis.
 
While some of this can be done with Plumed itself, the specialisation of this tools may allow for speed-ups in some cases.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.com/ElianeBriand/fesutils.svg?branch=master)](https://travis-ci.com/ElianeBriand/fesutils)
[![codecov](https://codecov.io/gh/ElianeBriand/fesutils/branch/master/graph/badge.svg)](https://codecov.io/gh/ElianeBriand/fesutils)

**Documentation** : https://elianebriand.github.io/fesutils/

# Build

After having installed the non-bundled dependencies (see below), initialise submodules:
```
git submodule update --init --recursive
```

Build dependencies (`fmt`, `yaml-cpp` & `indicators`):
```
cd bundled-deps
mkdir build-fmt && cd build-fmt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../fmt
make && make install
cd ..
mkdir build-yaml-cpp && cd build-yaml-cpp
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../yaml-cpp
make && make install
cd ..
mkdir build-indicators && cd build-indicators
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../indicators
make && make install
cd ../..
```

CMake build of the main project:
```
mkdir build && cd build
cmake ..
make
cd ..
```

# How to use

Invoke the fesutils executable (found in the build directory) with a YAML runfile, for instance :

```
./build/fesutils yaml_runfile.yaml
```

Or for a working examples:

```
./build/fesutils yaml_runfile.yaml
```

See documentation for worked out usage examples: https://elianebriand.github.io/fesutils/

# Dependencies:

Must be installed:
- Boost >1.67
- Eigen 3
- Threading Building Block (Intel tbb)

Bundled-in submodules:
- indicator
- fmt
- yaml-cpp

