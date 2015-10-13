SRC=src
INC=include
CATCH=-I$(INC)/Catch/single_include/
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC)
LIB=
TESTS=./src/neuralnet.cpp ./src/api_outline.cpp ./test/catch_main.cpp ./test/test_neuralnet.cpp
OBJS=./src/neuralnet.o ./src/api_outline.o ./test/catch_main.o ./test/test_neuralnet.o
BIN=build/bin/

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

.PHONY: run build clean

all: server tests

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(SRC)/json.cpp $(SRC)/neuralnet.cpp $(INC)/mongoose/mongoose.c $(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

%.o: %.cpp $(TESTS)
	$(CXX) -c -o $@ $(CFLAGS) $(CATCH) -I$(SRC) $<

tests: $(OBJS)
	@mkdir -p build/bin
	$(CXX) -o $(BIN)$@ $(CFLAGS) $(CATCH) $^

run:
	./build/bin/tests

clean:
	-rm -rf server server.exe
	-rm -rf ./build
	-rm -rf ./test/*.o
	-rm -rf ./src/*.o
