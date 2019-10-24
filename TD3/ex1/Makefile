MPICXX = mpic++
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXEC= main

%.o :%.cpp
	$(MPICXX) -o $@ -c $<

main : $(OBJ)
	$(MPICXX) -o $@ $^

run: main
	mpirun --oversubscribe -np 4 ./main 2 2 #&& rm -rf main *.o

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
