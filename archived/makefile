CC = g++-9 -fopenmp
PROM = ./bin/DII_STT.ex
CFLAGS = -I.
DEPS = $(shell find ./src -name "*.h")
SRC = $(shell find ./src -name "*.cpp")
OBJ = $(SRC:%.cpp=%.o)

$(PROM): $(OBJ)
	$(CC) -o $(PROM) $(OBJ) $(CFLAGS) -O2

%.o:%.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) -O2

clean:
	rm -rf $(OBJ)