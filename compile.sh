#note that c++11 standard must be used to compile the prorgams, otherwise ground
# ground truth does not work.
g++ -std=c++11 -O3  $1.cpp source/groundtruth.cpp source/utility.cpp source/faceFunctions.cpp source/houghFunctions.cpp source/f1score.cpp /usr/local/lib/libopencv_* -o outfiles/$1.out
#./$1.out output/dart13_output.png
