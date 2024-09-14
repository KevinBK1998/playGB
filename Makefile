EXECUTABLE = a.out

.PHONY: build run test

run: $(EXECUTABLE)
	./$(EXECUTABLE)

build: $(EXECUTABLE)

$(EXECUTABLE): src/main.cpp src/myFunctions.cpp
	g++ src/main.cpp src/myFunctions.cpp -o $@

build-test: $(EXECUTABLE) CMakeLists.txt
	cmake -S . -B cmake-build
	cmake --build cmake-build
	touch $@

test: build-test
	ctest --test-dir cmake-build
