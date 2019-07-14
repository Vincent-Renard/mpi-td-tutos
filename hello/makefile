EXECS=hello
MPICC?=mpicc

all:
	${EXECS}

mpi_hello: hello.c
	${MPICC} -o hello hello.c  ; ./hello

clean:
	rm ${EXECS}
