### Makefile ---

## Author: xghemail@gmail.com
## Version: $Id: Makefile,v 0.0 2012/03/21 02:35:05 jerry Exp $
## Keywords: 
## X-URL: 

TARGET  := main
SRCS    := main.c
OBJS    := ${SRCS:.c=.o} 
CC 	:= gcc

#CCFLAGS = -std=gnu99 -O2 -Wall -Werror -ggdb 
CCFLAGS = -std=c99 -Wall -ggdb 
LDFLAGS = 
LIBS    = 

.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS} 

${OBJS}: %.o: %.c
	${CC} ${CCFLAGS} -o $@ -c $< 

run: ${TARGET}
	./${TARGET}
clean: 
	-rm -f *~ *.o ${TARGET} 

### Makefile ends here
