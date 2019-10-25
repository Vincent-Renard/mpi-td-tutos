MPICXX = mpic++
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)
EXEC= main

%.o :%.cpp
	$(MPICXX) -o $@ -c $<

main : $(OBJ)
	$(MPICXX) -o $@ $^

run: main
	OMPI_MCA_btl=self,tcp mpirun --oversubscribe -np 16 ./main && rm -rf main *.o

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
