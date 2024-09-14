run: build
	./a.out
build: src/main.cpp src/myFunctions.cpp
	g++ src/main.cpp src/myFunctions.cpp
