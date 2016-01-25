all: make

gtest:
	cd build; src/lib/test/test_lib

ctest:
	cd build; ctest

make:
	cd build; make VERBOSE=1

cmake:
	mkdir build; cd build; CC=gcc CXX=g++ cmake cmake ..

clean:
	-rm -rf build; find . -name '*~' | xargs rm -f
