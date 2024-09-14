EXECUTABLE = a.out

.PHONY: build clean run test

build: $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): src/main.cpp src/Memory.cpp
	g++ $^ -o $@

build-test: $(EXECUTABLE) CMakeLists.txt
	cmake -S . -B cmake-build
	cmake --build cmake-build
	touch $@

test: build-test
	ctest --test-dir cmake-build

test-failed-verbose: build-test
	ctest --test-dir cmake-build --rerun-failed --output-on-failure
