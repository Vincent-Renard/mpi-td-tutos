CC := g++
CFLAGS := -std=c++11

all : Pi

Pi : Pi.o
	$(CC) $(CFLAGS) -fopenmp -o $@  $< && ./Pi 10000000 16 && make clean

%.o : %.cpp
	$(CC) $(CFLAGS) -fopenmp -o $@ -c $<

clean:
	rm -f *.o *~ Pi
