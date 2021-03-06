.PHONY : all test clean heap hashing

CXX         = g++
PYTHON      = /usr/local/bin/python
CPPFLAGS    = -fPIC -g -std=c++11 -Wall -pedantic
LDFLAGS     = -shared `python-config --ldflags`

DEPS        = pybind11 out
TARGET      = stub.so
HEADERS     = $(shell ls src/*.hpp)
SOURCES     = $(shell ls src/*.cpp | xargs -n 1 basename)
OBJ         = $(patsubst %,out/%,$(SOURCES:.cpp=.o))

all: $(TARGET)

test: heap splay

heap: heap.py $(TARGET)
	$(PYTHON) $<

hashing: hashing.py $(TARGET)
	$(PYTHON) $<

pybind11:
	git clone https://github.com/pybind/pybind11.git

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

out/%.o: src/%.cpp $(DEPS) $(HEADERS)
	$(CXX) -c -o $@ $< $(CPPFLAGS) -Isrc -Ipybind11/include `python-config --cflags`

clean:
	rm -rf out $(TARGET)

out:
	mkdir $@