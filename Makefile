SRC=src
INC=include
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC) -I$(INC)/Catch/single_include/
LIB=
BIN=build/bin/

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: server unit_tests

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(SRC)/json.cpp $(SRC)/neuralnet.cpp $(INC)/mongoose/mongoose.c $(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

unit_tests: $(SRC)/unit_tests.cpp $(SRC)/api_outline.cpp $(SRC)/board.cpp $(SRC)/neuralnet.cpp
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

clean:
	-rm -rf server server.exe unit_tests unit_tests.exe
