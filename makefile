CC=g++
KEY=-O2 -lpthread
PATH_HEADER=./
EXE=exe
SRC=HW28.cpp
OBJ=$(SRC:.cpp=.o)

#run: all
#	./$(EXE)

all: $(SRC) $(EXE) clean
 
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ -I$(PATH_HEADER) $(KEY)

%.o : %.cpp
	$(CC) -c $< -o $@ 

clean:
	rm -rf *.o *.a


