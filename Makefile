SRC=src
INC=include
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-result -Wno-deprecated-register -I$(INC) -I$(SRC)
LIB=
DIRS= examples/*

JSON_SRC=$(INC)/jsoncpp/json_reader.cpp $(INC)/jsoncpp/json_value.cpp $(INC)/jsoncpp/json_writer.cpp $(SRC)/json/json.cpp
MONGOOSE_SRC=$(INC)/mongoose/mongoose.c
SKYNET_SRC=$(SRC)/skynet/checkers.cpp $(SRC)/skynet/checkers_client.cpp $(SRC)/skynet/neuralnet.cpp

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32
endif

all: server unit_tests examples

server: $(SRC)/server/server.cpp $(SRC)/server/game_manager.cpp $(SRC)/server/handler.cpp $(JSON_SRC) $(MONGOOSE_SRC) $(SKYNET_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

unit_tests: $(SRC)/unit_tests/unit_tests.cpp $(JSON_SRC) $(MONGOOSE_SRC) $(SKYNET_SRC)
	$(CXX) $(CFLAGS) $(LIB) $^ -o $@

examples: $(JSON_SRC) $(MONGOOSE_SRC) $(SKYNET_SRC)
	-for d in $(DIRS); do ( cd $$d; $(MAKE) clean all ); done

clean:
	-rm -rf server server.exe unit_tests unit_tests.exe
	-for d in $(DIRS); do ( cd $$d; $(MAKE) clean ); done
