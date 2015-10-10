SRC=src
INC=include
CATCH=-I$(INC)/Catch/single_include/
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC)
LIB=

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: server

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(SRC)/json.cpp $(SRC)/neuralnet.cpp $(INC)/mongoose/mongoose.c $(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

catch: 
	@mkdir -p build/obj
	$(CXX) $(CFLAGS) $(CATCH) ./test/catch_main.cpp -o ./build/obj/catch_main.o

clean:
	-rm -rf server server.exe
	-rm -rf ./build
