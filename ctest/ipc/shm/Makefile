ifdef DEBUG
	CFLAGS=-g
endif

all: write.out read.out

write.out : write.c
	${CC} ${CFLAGS} $< -o $@
	
read.out : read.c
	${CC} ${CFLAGS} $< -o $@
		
.PHONY : clean

clean : 
	${RM} write.out read.out
