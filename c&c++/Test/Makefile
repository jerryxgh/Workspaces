### Makefile --- 

## Author: jerryxgh@gmail.com
## Version: 0.0
## Keywords: 
## X-URL: not distributed yet
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

clean: 
-rm -f *~ *.o ${TARGET} 

### Makefile ends here
