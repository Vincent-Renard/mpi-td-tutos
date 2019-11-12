CC := g++
CFLAGS := -std=c++11

all : VecAdd

VecAdd : VecAdd.o
	$(CC) $(CFLAGS)  -o $@  $<

%.o : %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

clean:
	rm -f *.o *~ VecAdd
