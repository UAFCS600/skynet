SRC=src
INC=include
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC)
LIB=
BIN=build/bin/

JSON_SRC=$(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
MONGOOSE_SRC=$(INC)/mongoose/mongoose.c
SKYNET_SRC=$(SRC)/skynet/checkers_board.cpp $(SRC)/skynet/neuralnet.cpp

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: server unit_tests

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(SRC)/json.cpp $(JSON_SRC) $(MONGOOSE_SRC) $(SKYNET_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

unit_tests: $(SRC)/unit_tests.cpp $(SKYNET_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

clean:
	-rm -rf server server.exe unit_tests unit_tests.exe
