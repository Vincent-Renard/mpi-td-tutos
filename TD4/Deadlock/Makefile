CC := g++
CFLAGS := -std=c++11

all : Deadlock

VecAdd : Deadlock.o
	$(CC) $(CFLAGS)  -o $@  $<

%.o : %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

clean:
	rm -f *.o *~ Deadlock
