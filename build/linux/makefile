CXX=clang++
CXXFLAGS=-O3 -std=c++11 -pthread -Wall #-pedantic
LIB=
CX=$(CXX) $(CXXFLAGS)
BIN=libucm.a
R=../../src/ucm

VPATH = ../../src/ucm

SRCS = $(wildcard $(R)/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCS)))

all: $(BIN)

$(BIN): $(OBJS)
	ar -crv $(BIN) $(OBJS)

%.o:    %.cpp
	$(CX) -c $< -o $@

clean:
	rm -f $(BIN) *.o
	rm -rf $(BIN).dSYM

