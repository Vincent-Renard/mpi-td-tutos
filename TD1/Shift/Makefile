MPICXX = mpic++
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXEC= main

%.o: %.cpp
	$(MPICXX) -o $@ -c $<

main: $(OBJ)
	$(MPICXX) -o $@ $^

run: main
	mpirun --oversubscribe -np 4 ./main 100000 && rm -rf *.o main

clean:
	@rm -rf *.o

cleanall: clean
	@rm -rf $(EXEC)
