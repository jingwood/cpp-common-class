CXX=clang++
CXXFLAGS=-O3 -std=c++11 -pthread -Wall -arch arm64 #-pedantic
LIB=
CX=$(CXX) $(CXXFLAGS)
BIN=libucm.a
VPATH = ../../src/ucm/

SRCS = $(wildcard $(VPATH)*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(notdir $(SRCS)))

all: $(BIN)

$(BIN): $(OBJS)
	ar -crv $(BIN) $(OBJS)

%.o:    %.cpp
	$(CX) -c $< -o $@

clean:
	rm -f $(BIN) *.o
	rm -rf $(BIN).dSYM

