### Makefile --- 

## Author: jerryxgh@gmail.com
## Version: $Id: Makefile,v 0.0 2012/07/05 15:00:24 jerry Exp $
## Keywords: 
## X-URL: 
TARGET  := main
SRCS    := main.c
OBJS    := ${SRCS:.c=.o} 
CC	:= gcc

CCFLAGS = -std=c99 -Wall -Werror -ggdb 
LDFLAGS = 
LIBS    = 

.PHONY: all clean 
all: ${TARGET} 

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS} 

${OBJS}: %.o: %.c
	${CC} ${CCFLAGS} -o $@ -c $< 

clean:: 
	-rm -f *~ *.o ${TARGET} 

### Makefile ends here
