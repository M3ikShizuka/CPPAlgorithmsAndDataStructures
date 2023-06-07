# Algorithms and Data Structures in modern C++
![build](https://github.com/M3ikShizuka/CPPAlgorithmsAndDataStructures/actions/workflows/cmake-multi-platform.yml/badge.svg) ![Static Badge](https://img.shields.io/badge/Code%20Coverage-99%25-green)

* Algorithms
    * Sort
        * [BubbleSort](./src/Algorithms/Sort/BubbleSort)
        * [HeapSort](./src/Algorithms/Sort/HeapSort)
        * [InsertionSort](./src/Algorithms/Sort/InsertionSort)
        * [MergeSort](./src/Algorithms/Sort/MergeSort)
        * [QuickSort](./src/Algorithms/Sort/QuickSort)
        * [SelectionSort](./src/Algorithms/Sort/SelectionSort)
* Data Structures
    * Non-linear Data Structure
        * Basic
            * Trees
                * [Binary Search Tree (BST)](src/DataStructures/Non-linear/Basic/Trees/BinarySearchTree)  
                    * Based on loop [No augmentation, no extra field `parent`]
        * Complex (based on basic)
            * Trees
                * [Heap / Binary Heap](src/DataStructures/Non-linear/Complex/Trees/Heap)
                * [Red-Black Tree](src/DataStructures/Non-linear/Complex/Trees/RedBlackTreeLoop)
                    * Based on loop
                * [AVL Tree](src/DataStructures/Non-linear/Complex/Trees/AVLTree)
                    * [Based on loop](src/DataStructures/Non-linear/Complex/Trees/AVLTree/AVLTreeLoop)
                    * [Based on recursion](src/DataStructures/Non-linear/Complex/Trees/AVLTree/AVLTreeRecursion)
* ...
## Build
### IDE CLion
Open project directory in CLion
### Manually (Terminal)
Required tools:
* [CMake](https://cmake.org/download/)
* [Ninja](https://ninja-build.org)
* Development Environment (Toolchain):
    * [Mingw-builds](https://www.mingw-w64.org/downloads/#mingw-builds) on Windows
    * MSVC tools (cl, lm, windbg, ...) environment:([x64/x86/...] Native Tools Command Prompt for VS [20**]) - Supplied with Visual Studio. on Windows 
    * GNU Compiler Collection (gcc, g++, ld, gdb, ...) on *nix operating systems.
    * LLVM Compiler Infrastructure (clang, clang++, lld, lldb, ...) on *nix operating systems.
#### [NOTE] Mingw-builds on Windows 
1. Download mingw with seh-ucrt-rt or seh-msvcrt-rt support from [Mingw-builds](https://www.mingw-w64.org/downloads/#mingw-builds) subsection.
Example: x86_64-13.1.0-release-win32-seh-ucrt-rt_v11-rev1.7z
2. Unpack downloaded mingw archive.
3. Add the path to mingw/bin folder in Windows environment variable "PATH" (for user and system).
#### Build project
**Replace [...] on you paths and names.**  
* [project root directory] = root directory of this project wich you pulled (git clone).  
* [ninja root directory] = path where ninja.exe is located.
* [configuration directory] = usually something like: "[project root directory]/build/x64-win-debug-mingw"  

Configure project  
[Note]: You can use CMake arguments like
* -DCMAKE_C_COMPILER
* -DCMAKE_CXX_COMPILER
* -DCMAKE_LINKER
* -DCMAKE_VERBOSE_MAKEFILE=ON - Verbose ninja, makefile, etc... or set in CMakeList.txt or use -v on build
* ...  

to specify paths for tools.

```bash
cmake 
-DCMAKE_VERBOSE_MAKEFILE=ON
-DCMAKE_BUILD_TYPE=Debug
-DCMAKE_C_COMPILER=gcc
-DCMAKE_CXX_COMPILER=g++
-DCMAKE_LINKER=ld
-DCMAKE_MAKE_PROGRAM="[ninja root directory]/ninja.exe"
-G Ninja
-S "[project root directory]"
-B "[configuration directory]"
```

[NOTE]: For verbose ninja, makefile, etc... just use argument -v on build: cmake.exe --build ...mybuild/x64-win-debug-mingw --target all -j 6 -v

Build configuration  
```bash
cmake --build "[configuration directory]" --target all -j 6
```
Clean
```bash
cmake --build "[configuration directory]" --target clean -j 6
```
## Run tests
Run all unit tests  
```bash
ctest --test-dir "[configuration directory]"
```
or
```bash
ctest --extra-verbose --test-dir "[configuration directory]"
```
Example run only BubbleSort.
```bash
"[configuration directory]\src\Algorithms\Sort\BubbleSort\BubbleSort.exe"
```
## Known problems
If you build project for some another processor architecture you can get error like: "is not able to compile a simple test program."  
**Solution:** Uncomment some of this lines in root CMakeLists.txt
```cmake
# Fix cmake compile test "is not able to compile a simple test program."
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# or
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)
```
ーーーーーーーーーーーーーーーーーーーーーーーーー  
## Code Author: [Mikhail Medvedev (M3ik Shizuka)](https://m3ikshizuka.github.io)