SRC=src
INC=include
CATCH=-I$(INC)/Catch/single_include/
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -I$(INC)
LIB=
TESTS=./test/catch_main.cpp ./test/test_neuralnet.cpp
OBJS=./test/catch_main.o ./test/test_neuralnet.o

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

.PHONY: run build clean

all: server

build:
	@echo "Creating build directory structure."
	@mkdir -p build/bin

server: $(SRC)/server.cpp $(SRC)/handler.cpp $(SRC)/json.cpp $(SRC)/neuralnet.cpp $(INC)/mongoose/mongoose.c $(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

%.o: %.cpp $(TESTS)
	$(CXX) -c -o $@ -I./ $(CFLAGS) $(CATCH) -I$(SRC) $<

tests: $(OBJS)
	$(CXX) $(CFLAGS) -I./ $(CATCH) ./api_outline.cpp ./src/neuralnet.cpp $^ -o ./build/bin/$@ 

run:
	./build/bin/tests

clean:
	-rm -rf server server.exe
	-rm -rf ./build
	-rm -rf ./test/*.o
