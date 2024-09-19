# Frequent Substring Trees

This repository contains the accompanying code for our paper "Frequent Substring Trees: Using Tries for Symbol-Table Construction in String Compression" (authors: Dominik Probst and Klaus Meyer-Wegener).

Frequent Substring Trees (FSTs) are a novel trie-like data structure designed to be utilized to enhance the compression factor of symbol-table-based string compression in databases. Experimental results demonstrate that with FSTs the size of compressed columns can be reduced by up to 24% compared to the most advanced existing method, Fast Static Symbol Table (FSST) by Boncz et al.

## Third-Party Code and Licensing Information

### Fast Static Symbol Table (FSST) 

Since our approach only concerns the construction of the symbol table, we use FSST by Boncz et al. to perform the actual compression. FSST's code is also used to establish benchmark comparison values. We have included the code published at https://github.com/cwida/fsst in this repository.

#### Affected Files

The folder [lib/fsst](lib/fsst) contains a copy of the code from https://github.com/cwida/fsst (Commit 42850e13ba220dbba5fd721a4c54f969e2a45ac5). We have made some modifications to the code (e.g., to introduce measurement points), which are each marked by a `MODIFIED: [DESCRIPTION]` and an `END OF MODIFIED` comment.

The files [CMakeLists.txt](CMakeLists.txt), [src/fsst/fsst.cpp](src/fsst/fsst.cpp), [src/fsst/fsst.h](src/fsst/fsst.h), and [src/fsst/libfsst.cpp](src/fsst/libfsst.cpp) are also based on their counterparts in [lib/fsst](lib/fsst). However, they have been adapted so loosely that not every change in the code is marked.

#### License

The FSST code used in this repository is licensed under the MIT License. A copy of the corresponding license text can be found at [lib/fsst/LICENSE](lib/fsst/LICENSE).