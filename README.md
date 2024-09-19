# Frequent Substring Trees

This repository contains the accompanying code for our paper "Frequent Substring Trees: Using Tries for Symbol-Table Construction in String Compression" (authors: Dominik Probst and Klaus Meyer-Wegener).

Frequent Substring Trees (FSTs) are a novel trie-like data structure designed to be utilized to enhance the compression factor of symbol-table-based string compression in databases. Experimental results demonstrate that with FSTs the size of compressed columns can be reduced by up to 24% compared to the most advanced existing method, Fast Static Symbol Table (FSST) by Boncz et al.

## Usage

### Compilation

Our prototype is primarily written in C++. To build the various binaries, CMake (v3.10+) must be installed on the respective machine. 

The compilation is carried out using the following two commands:

```
cmake .
cmake --build .
```

### Execution

During compilation, a total of four different programs are built: `fst`, `fst_tests`, `fsst_with_fst`, and `fsst_without_fst`. Each of these programs serves a different purpose, with `fsst_with_fst` representing the prototype presented in our paper.

#### Executable `fst`

The executable `fst` provides a standalone version of our method. When executing `./fst`, strings can be interactively added to an FST through the console, allowing users to observe how symbol table entries are generated from the FST. Actual data compression does not take place.

#### Executable `fst_tests`

Our method's code is extensively covered by test cases. The executable `fst_tests` contains these test cases and can be executed via `./fst_tests`. This does not include testing the integration with a symbol table-based method, nor does it test actual data compression.

#### Executable `fsst_with_fst`

The executable `fsst_with_fst` corresponds to the prototype presented in our work, where FSTs are used within the symbol table-based method FSST developed by Boncz et al.

##### Compression

To perform compression, use the following command:

```
./fsst_with_fst [samples] [input] [output]
```

The parameters are as follows:

- `samples` **(required)**: If preferred line sampling is to be used, a positive integer must be provided, indicating the number of lines to be sampled. To use FSST's sampling approach instead, pass `fsst`.

- `input` **(required)**: The path to the file to be compressed.

- `output` **(optional)**: An optional path to store the compressed file.

##### Decompression

To perform decompression, use the following command:

```
./fsst_with_fst -d [input] [output]
```

The parameters are as follows:

- `input` **(required)**: The path to the file to be decompressed.

- `output` **(required)**: The path to store the decompressed file.

#### Executable `fsst_without_fst`

The executable `fsst_without_fst` is the binary for the original FSST code without our method.

##### Compression

To perform compression, use the following command:

```
./fsst_without_fst [input] [output]
```

The parameters are as follows:

- `input` **(required)**: The path to the file to be compressed.

- `output` **(optional)**: An optional path to store the compressed file.

##### Decompression

To perform decompression, use the following command:

```
./fsst_without_fst -d [input] [output]
```

The parameters are as follows:

- `input` **(required)**: The path to the file to be decompressed.

- `output` **(required)**: The path to store the decompressed file.

### Benchmark

For our paper, we repeatedly compressed all datasets contained in dbtext ([lib/fsst/paper/dbtext](lib/fsst/paper/dbtext)) using both `fsst_with_fst` and `fsst_with_fst`. The benchmark was conducted using [benchmark/benchmark.py](benchmark/benchmark.py) and subsequently processed for the paper using [benchmark/preprocessing.py](benchmark/preprocessing.py).

#### Reproduction

##### Actual Benchmark

To reproduce the results from the paper, the benchmark can be run with the following command:

```
python3 -u benchmark/benchmark.py | tee benchmark/running.log
```

Please note that Python 3 must be installed on your system, and the binaries need to be compiled beforehand. The command must also be executed from the root directory of this repository. The total runtime of the benchmark on our system was approximately one week.

##### Preprocessing

After the benchmark is complete, the results can be processed using our preprocessing pipeline:

```
python3 benchmark/preprocessing.py
```

For this step, Python 3, as well as pandas and NumPy, must be installed. This command must also be executed from the root directory of this repository.

#### Our Results

We are also providing the results of the benchmark run conducted as part of our paper, along with all artifacts (e.g. results and compressed files). 

Since these artifacts have a size of multiple GBs, we have not added them directly to this repository. 

Instead, the artifacts are included as a submodule under the path [benchmark/paper](benchmark/paper) and can be downloaded either directly during the initial clone using 

```
git clone --recurse-submodules git@github.com:FAU-CS6/Frequent-Substring-Trees.git
```

or added to an already cloned repository afterwards using 

```
git submodule update --init --recursive
```

## Third-Party Code and Licensing Information

### Fast Static Symbol Table (FSST) 

Since our approach only concerns the construction of the symbol table, we use FSST by Boncz et al. to perform the actual compression. FSST's code is also used to establish benchmark comparison values. We have included the code published at https://github.com/cwida/fsst in this repository.

#### Affected Files

The folder [lib/fsst](lib/fsst) contains a copy of [https://github.com/cwida/fsst](https://github.com/cwida/fsst) (Commit [42850e13ba220dbba5fd721a4c54f969e2a45ac5](https://github.com/cwida/fsst/tree/42850e13ba220dbba5fd721a4c54f969e2a45ac5)). We have made some modifications to the code (e.g., to introduce measurement points), which are marked by a `MODIFIED: [DESCRIPTION]` and an `END OF MODIFIED` comment.

The files [CMakeLists.txt](CMakeLists.txt), [src/fsst/fsst.cpp](src/fsst/fsst.cpp), [src/fsst/fsst.h](src/fsst/fsst.h), and [src/fsst/libfsst.cpp](src/fsst/libfsst.cpp) are also based on their counterparts in [lib/fsst](lib/fsst). However they mostly have been adapted very loosely, so not every change is marked.

#### License

The FSST code used in this repository is licensed under the MIT License. A copy of the corresponding license text can be found at [lib/fsst/LICENSE](lib/fsst/LICENSE).