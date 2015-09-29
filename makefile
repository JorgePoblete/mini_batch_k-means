OBJ=mini_batch_kmeans.o utils.o main.o
EIGEN_PATH=/usr/local/include/eigen3/
INCLUDES=-I $(EIGEN_PATH)
CC=g++
CCFLAGS=-Wall -std=c++11
OBJFLAGS=-c
LDLIBS=-fopenmp
DFLAGS=

$(VERBOSE).SILENT:

all: kmeans
	rm -f *.o

mini_batch_kmeans.o: mini_batch_kmeans.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(DFLAGS) $(LDLIBS) $(INCLUDES) mini_batch_kmeans.cpp

utils.o: utils.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) utils.cpp

main.o: main.cpp
	$(CC) $(OBJFLAGS) $(CCFLAGS) $(INCLUDES) main.cpp

kmeans: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) $(LDLIBS) $(INCLUDES) -o kmeans

clean:
	rm -f *.o kmeans
