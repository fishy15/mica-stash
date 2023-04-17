all: build/vectoradd build/roundrobin

build/vectoradd: vectoradd.cpp
	mkdir -p build
	g++ -o build/vectoradd vectoradd.cpp -pthread -std=c++11

build/roundrobin: roundrobin.cpp
	mkdir -p build
	g++ -o build/roundrobin roundrobin.cpp -pthread -std=c++11
