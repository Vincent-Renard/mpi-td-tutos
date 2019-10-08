MPICXX = mpic++
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXEC= main

%.o :%.cpp
	$(MPICXX) -o $@ -c $<

main : $(OBJ)
	$(MPICXX) -o $@ $^

run:
	mpirun --oversubscribe -np 4 ./main 22 0 #&& rm -rf main *.o

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
