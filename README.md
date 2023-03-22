# SABT
![Tests](https://github.com/knyazer/SABT/actions/workflows/tests.yml/badge.svg)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

SABT, abbreviation from the Stack Associated Beam Tracing, is an algorithm, based on [nvidia 2010 paper](https://research.nvidia.com/sites/default/files/pubs/2010-02_Efficient-Sparse-Voxel/laine2010tr1_paper.pdf), which introduces a number of additional optimizations and heuristics, to reduce the number of empirically chosen constants in the original algorithm. Moreover, it introduces an attempt to make the algorithm a bit more lazy, by reusing previously computed beams, however, in practice this optimization does not lead to significant speedup due to the large effect of small angular perturbations.

The contributions are welcome. There is a lot to be done, at least the code should be transferred to CUDA to have a possibility to compare performance with the original :)

### Quickstart

To run the code, one should clone the repository, then update submodules, and build everything:
```
git clone https://github.com/knyazer/SABT
cd SABT
git submodule init
git submodule update

mkdir build
cmake ..
make -j8
```
After this, two executables will be available: first one is ```build/demo/demo```, which is a demo showing a dragon. And there is a ```build/tests/tests```, which is a huge set of tests, covering most of the functionality of the engine.

I didn't use a lot of libraries, so most of the math is done by basic code :) This could be a reason for pure performance, but probably not..
