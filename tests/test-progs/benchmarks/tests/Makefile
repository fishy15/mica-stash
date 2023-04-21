all: build/sparseadd64 build/blockadd64 build/roundrobin64 build/prefixscan64 build/sparseadd100 build/blockadd100 build/roundrobin100 build/prefixscan100 build/sparseadd500 build/blockadd500 build/roundrobin500 build/prefixscan500 build/sparseadd1000 build/blockadd1000 build/roundrobin1000 build/prefixscan1000

clean:
	rm -rf build/

build/sparseadd64: sparseadd.cpp
	mkdir -p build
	g++ -o build/sparseadd64 sparseadd.cpp -pthread -std=c++11 -DN=64

build/blockadd64: blockadd.cpp
	mkdir -p build
	g++ -o build/blockadd64 blockadd.cpp -pthread -std=c++11 -DN=64

build/roundrobin64: roundrobin.cpp
	mkdir -p build
	g++ -o build/roundrobin64 roundrobin.cpp -pthread -std=c++11 -DN=64

build/prefixscan64: prefixscan.cpp
	mkdir -p build
	g++ -o build/prefixscan64 prefixscan.cpp -pthread -std=c++11 -DN=64

build/sparseadd100: sparseadd.cpp
	mkdir -p build
	g++ -o build/sparseadd100 sparseadd.cpp -pthread -std=c++11 -DN=100

build/blockadd100: blockadd.cpp
	mkdir -p build
	g++ -o build/blockadd100 blockadd.cpp -pthread -std=c++11 -DN=100

build/roundrobin100: roundrobin.cpp
	mkdir -p build
	g++ -o build/roundrobin100 roundrobin.cpp -pthread -std=c++11 -DN=100

build/prefixscan100: prefixscan.cpp
	mkdir -p build
	g++ -o build/prefixscan100 prefixscan.cpp -pthread -std=c++11 -DN=100

build/sparseadd500: sparseadd.cpp
	mkdir -p build
	g++ -o build/sparseadd500 sparseadd.cpp -pthread -std=c++11 -DN=500

build/blockadd500: blockadd.cpp
	mkdir -p build
	g++ -o build/blockadd500 blockadd.cpp -pthread -std=c++11 -DN=500

build/roundrobin500: roundrobin.cpp
	mkdir -p build
	g++ -o build/roundrobin500 roundrobin.cpp -pthread -std=c++11 -DN=500

build/prefixscan500: prefixscan.cpp
	mkdir -p build
	g++ -o build/prefixscan500 prefixscan.cpp -pthread -std=c++11 -DN=500

build/sparseadd1000: sparseadd.cpp
	mkdir -p build
	g++ -o build/sparseadd1000 sparseadd.cpp -pthread -std=c++11 -DN=1000

build/blockadd1000: blockadd.cpp
	mkdir -p build
	g++ -o build/blockadd1000 blockadd.cpp -pthread -std=c++11 -DN=1000

build/roundrobin1000: roundrobin.cpp
	mkdir -p build
	g++ -o build/roundrobin1000 roundrobin.cpp -pthread -std=c++11 -DN=1000

build/prefixscan1000: prefixscan.cpp
	mkdir -p build
	g++ -o build/prefixscan1000 prefixscan.cpp -pthread -std=c++11 -DN=1000
