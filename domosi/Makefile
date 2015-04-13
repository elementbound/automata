CXX_FLAGS = -std=c++11 -I../ 

build: 
	g++ ${CXX_FLAGS} -c ../latinsquare/grid.cpp -o bin/grid.o
	g++ ${CXX_FLAGS} -c automata.cpp -o bin/automata.o
	g++ ${CXX_FLAGS} -c bitstream.cpp -o bin/bitstream.o
	g++ ${CXX_FLAGS} -c endomosi.cpp -o bin/endomosi.o
	g++ ${CXX_FLAGS} -c dedomosi.cpp -o bin/dedomosi.o
	g++ bin/grid.o bin/automata.o bin/bitstream.o bin/endomosi.o -o endomosi
	g++ bin/grid.o bin/automata.o bin/bitstream.o bin/dedomosi.o -o dedomosi
	
test: build
	endomosi ../lipsum.txt en.txt
	dedomosi en.txt de.txt
	
test_min: build
	endomosi ../min.txt en.txt
	dedomosi en.txt de.txt