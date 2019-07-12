include ../make.inc
# ---------------------------------------------

CFLAGS	:= -xhost -qopenmp -O2 -std=c++11 $(DEFS) -fPIC

# ---------------------------------------------

EXEC	= intelqs`python3-config --extension-suffix`

CFLAGS	+= `python3 -m pybind11 --includes`
EXTRA	+= -shared

INC	+= -I../qureg
OBJS	:= /nfs_home/giangiac/Intel-QS/qureg/qHiPSTER.a

SRC	:= intelqs.cpp
SRC_O	:= intelqs.o

$(EXEC) :	$(SRC_O) $(OBJS)
		$(CC) $(CFLAGS) $(EXTRA) $(LDFLAGS) $< $(OBJS) -o $(EXEC)

$(SRC_O) :	$(SRC) ../make.inc makefile $(OBJS) $(INCS)
		$(CC) $(CFLAGS) $(INC) -c $< -o $@

# ---------------------------------------------

clean:
	rm -f *.o *.out
