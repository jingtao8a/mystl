CPP      = g++.exe
OBJ      = main.o
LINKOBJ  = main.o
BIN      = stl.exe
INCLUDE  = -I ./

.PHONY: all clean

all: $(BIN)

clean: 
	rm $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) 

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(INCLUDE) -std=c++11
