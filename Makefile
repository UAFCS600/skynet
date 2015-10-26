SRC=src
INC=include
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC)
LIB=

AI_SRC=$(SRC)/expr_parser.cpp $(SRC)/neuralnet.cpp
JSON_SRC=$(SRC)/json.cpp $(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
MONGOOSE_SRC=$(INC)/mongoose/mongoose.c

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: server

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(AI_SRC) $(JSON_SRC) $(MONGOOSE_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

clean:
	-rm -rf server server.exe
