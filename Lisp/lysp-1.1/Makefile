CFLAGS  = -O  -g -Wall
CFLAGSO = -O3 -g -Wall -DNDEBUG
CFLAGSs = -Os -g -Wall -DNDEBUG
LDLIBS  = -rdynamic

all : lysp gclysp

lysp : lysp.c gc.c
	$(CC) $(CFLAGS)  -DBDWGC=0 -o $@ lysp.c gc.c $(LDLIBS) -ldl
	size $@

olysp: lysp.c gc.c
	$(CC) $(CFLAGSO) -DBDWGC=0 -o $@ lysp.c gc.c $(LDLIBS) -ldl
	size $@

ulysp: lysp.c gc.c
	$(CC) $(CFLAGSs) -DBDWGC=0 -o $@ lysp.c gc.c $(LDLIBS) -ldl
	size $@

gclysp: lysp.c
	$(CC) $(CFLAGSO) -DBDWGC=1  -o $@ lysp.c $(LDLIBS) -lgc
	size $@

run : all
	./lysp -v -x test.l

test : lysp olysp ulysp gclysp
	@echo
	time   ./lysp bench.l; time   ./lysp bench.l
	@echo
	time  ./ulysp bench.l; time  ./ulysp bench.l
	@echo
	time  ./olysp bench.l; time  ./olysp bench.l
	@echo
	time ./gclysp bench.l; time ./gclysp bench.l

clean : .FORCE
	rm -rf *~ *.o lysp olysp ulysp gclysp *.dSYM

.FORCE :
