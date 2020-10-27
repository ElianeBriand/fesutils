# Quick start guide

## Build instruction

Install the dependencies (Ubuntu package name for reference purpose):

| Depenency     | Ubuntu package |
| ------------- |-------------|
| Boost >1.67    | libboost-dev | 
| Eigen 3     | libeigen3-dev      | 
| Threading Building Block (Intel tbb) | libbtbb-dev     | 


Fetch source:

```
git clone --depth=1 https://github.com/ElianeBriand/fesutils.git
```

Initialise submodules:
```
git submodule update --init --recursive
```

Set-up for parallel build (typically, change number to the number of core in machine, or put 1):
```
export MAKE_NUM_PROCESS=4
```

Build dependencies (`fmt`, `yaml-cpp` & `indicators`):
```
cd bundled-deps
mkdir build-fmt && cd build-fmt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../fmt
make -j$MAKE_NUM_PROCESS && make install
cd ..
mkdir build-yaml-cpp && cd build-yaml-cpp
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../yaml-cpp
make -j$MAKE_NUM_PROCESS && make install
cd ..
mkdir build-indicators && cd build-indicators
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../deps-install ../indicators
make -j$MAKE_NUM_PROCESS && make install
cd ../..
```

CMake build of the main project:
```
mkdir build && cd build
cmake ..
make -j$MAKE_NUM_PROCESS
cd ..
```


## fesutils invokation


Invoke the fesutils executable (found in the build directory) with a YAML runfile, for instance :

```
./build/fesutils yaml_runfile.yaml
```

Or for a working examples:

```
./build/fesutils examples/fragment_reconstruction_short.yaml

```

Modify the examples file for your usecase. You may be interested in [worked out runfile/usage examples](./usage_examples/list_usage_examples.md).