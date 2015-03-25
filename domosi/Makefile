build: 
	g++ -std=c++11 -c ../latinsquare/grid.cpp -o bin/grid.o
	g++ -std=c++11 -c -I../ automata.cpp -o bin/automata.o
	g++ -std=c++11 -c -I../ endomosi.cpp -o bin/endomosi.o
	g++ -std=c++11 -c -I../ dedomosi.cpp -o bin/dedomosi.o
	g++ bin/grid.o bin/automata.o bin/endomosi.o -o endomosi
	g++ bin/grid.o bin/automata.o bin/dedomosi.o -o dedomosi