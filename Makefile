all:
	g++ `pkg-config --cflags opencv` obj_marker.cpp `pkg-config --libs opencv` -o binary