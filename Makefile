CXX=g++
CC=gcc
CXXFLAGS=-g -std=c++11

SRC=./src
BIN=./bin

.PHONY:clean debug

main.out:  $(SRC)/example.cc $(SRC)/bloom_parameters.cc $(SRC)/MurmurHash2.cc
	$(CXX) $(SRC)/example.cc $(SRC)/bloom_parameters.cc $(SRC)/MurmurHash2.cc $(CXXFLAGS) -o $(BIN)/example.out

debug:
	$(CXX) -g $(SRC)/example.cc $(SRC)/bloom_parameters.cc $(SRC)/MurmurHash2.cc $(CXXFLAGS) -o $(BIN)/example.debug
clean:
	rm -rf $(BIN)/*
