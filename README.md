# icfpc2022
ICFPC2022 sanma team repository

## Prerequisites

* Linux (Ubuntu 20.04 LTS (not 18.04 LTS which lacks g++-10)) / WSL1 on Windows 10
  * `sudo apt install build-essential git-core cmake g++-10 libtool autoconf texinfo libopencv-dev`
* Windows 10/11
  * Visual Studio 2022 (17.3.3)
  * CMake 3.24.1 (add to PATH)

## Build (Linux / WSL)

```
git clone --recursive https://github.com/nodchip/icfpc2022.git
# if you forgot to clone with --recursive, try: git submodule update --init
cd icfpc2022
bash build.sh
```

### GUI on WSL

Set up X server on Windows. [This](https://til.swfz.io/entries/wsl2_with_x/) is an example reference. 

## Build (Visual Studio)

```
git clone --recursive https://github.com/nodchip/icfpc2022.git
open Visual Studio 2022 Developer Console
cd icfpc2022
libs/build.bat # to build external libraries (Debug and Release)
start vs/ICFPC2022.sln
select Release;x64 or Debug;x64
Build Solution
```

## Run Tests

```
./test # run all tests
./test --gtest_filter=TestExample.* # run specific tests.
```

## Solve a problem

### Linux

```
./solver solve IntervalDPSolver ../data/problems/8.txt
```

* Some solver accepts solver-specific options.
  ```
  ./solver solve QuadTree ../data/problems/8.txt --quad-tree-min-cell-size 4
  ```

* Add another solver after comma delimiter to run it successively.
  ```
  ./solver solve SplitToGrid,Mean ../data/problems/8.txt
  ```

### Windows (console)

* current directory: vs\{solver,judge,test}
* exe path: vs\x64\{Release,Debug}

TODO(someone): Write this section.

## Visualizer

```
./solver visualize ISL_FILE_PATH [PROBLEM_FILE_PATH]
```
If one omits PROBLEM_FILE_PATH, it will be guessed from ISL_FILE_PATH name and its content.

## Manual solver

```
./solver solve Visualizer --manual 2
```

## Apply a solver to all problems

TODO(someone): Write this section.

## Add a Solver

TODO(someone): Write this section.

## Notes

* all source codes are UTF-8.
* line endings are LF.
