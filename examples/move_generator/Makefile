SRC=../../src
INC=../../include
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -Wno-deprecated-register -I$(INC) -I$(SRC)
LIB=

JSON_SRC=$(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp $(SRC)/json/json.cpp
MONGOOSE_SRC=$(INC)/mongoose/mongoose.c
SKYNET_SRC=$(SRC)/skynet/checkers.cpp $(SRC)/skynet/checkers_client.cpp $(SRC)/skynet/neuralnet.cpp

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: move_generator

move_generator: main.cpp $(JSON_SRC) $(MONGOOSE_SRC) $(SKYNET_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

clean:
	-rm -rf move_generator move_generator.exe
