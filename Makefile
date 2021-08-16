all:
	g++ src/main.cpp -o recursive-fcn-calc `fltk-config --ldflags` -O3
debug:
	g++ src/main.cpp -o recursive-fcn-calc -g `fltk-config --ldflags` -Wall
