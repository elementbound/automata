build: 
	g++ -c -std=c++11 grid.cpp -o bin/grid.o
	g++ -c -std=c++11 latinsquare.cpp -o bin/latinsquare.o
	g++ -c -std=c++11 enlatin.cpp -o bin/enlatin.o
	g++ -c -std=c++11 delatin.cpp -o bin/delatin.o
	
	g++ bin/grid.o bin/latinsquare.o -o latinsquare
	g++ bin/grid.o bin/enlatin.o	 -o enlatin
	g++ bin/grid.o bin/delatin.o	 -o delatin