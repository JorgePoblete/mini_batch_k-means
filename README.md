## Mini-batch k-means
Simple k-means using [Eigen 3](http://eigen.tuxfamily.org/) and support for the [Intel Xeon Phi Coprocessor](http://www.intel.la/content/www/xl/es/processors/xeon/xeon-phi-detail.html) (mic) implemented ussing the [EM](http://www.ics.uci.edu/~smyth/courses/cs274/notes/EMnotes.pdf) and [Mini-batch](http://www.eecs.tufts.edu/~dsculley/papers/fastkmeans.pdf) Algorithm.

It offers two build modes, one that only uses [Eigen 3](http://eigen.tuxfamily.org/) and other that mix [Eigen 3](http://eigen.tuxfamily.org/) functionalities with the mic.

## Build
It is asumed that you have [Eigen 3](http://eigen.tuxfamily.org/) installed on your system in *"/usr/local/include/eigen3/"*, if [Eigen 3](http://eigen.tuxfamily.org/) is installed in other path you can edit the makefile or just add *"EIGEN_PATH=YOUR_PATH"* to the make command.

**Eigen Only:**
In order to build using only [Eigen 3](http://eigen.tuxfamily.org/) is required to have a c++11 and [OpenMP](http://openmp.org/wp/openmp-compilers/) compatible compiler.

run *"make"*.

**Eigen and mic:**
In order to build using mic support is required to have an Intel compiler with support for [OpenMP 4](http://www.openmp.org/mp-documents/OpenMP4.0.0.pdf) directives and of course a compatible mic device.

run *"make DFLAGS=-DMIC CC=icpc"*.

## Run

To use the compiled executable run *"./kmeans K N D < InputFile"* where K, N and D represents the number of clusters, the number of data points and the dimension size of the data points respectively, the InputFile represents the file where the data points are stored, this file must have N * D numeric values.
