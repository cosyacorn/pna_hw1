CC=mpicc
CCFLAGS=-O3
INCS=Machine.h Comms.h Grid.h Field.h
LIBS=-lm

OBJS=comms.o grid.o machine.o field.o


# Run gs_eo with (eg) mpirun -np 8 1000 1000 4 2 
gs_eo: gs_eo.c ${INCS} ${OBJS}
	${CC} ${CCFLAGS} -o gs_eo gs_eo.c ${OBJS} ${LIBS}

comms.o: comms.c ${INCS}
	${CC} ${CCFLAGS} -c comms.c 
grid.o: grid.c ${INCS}
	${CC} ${CCFLAGS} -c grid.c 
machine.o: machine.c ${INCS}
	${CC} ${CCFLAGS} -c machine.c 
field.o: field.c ${INCS}
	${CC} ${CCFLAGS} -c field.c 
	
clean:
	rm -f ${OBJS} grid.out* gs_eo

