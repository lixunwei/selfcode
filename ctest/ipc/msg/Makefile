ifdef DEBUG
	CFLAGS=-g
endif

all: msgsnd.out msgrcv.out


msgsnd.out : msgsnd.c
	${CC} ${CFLAGS} $< -o $@

msgrcv.out : msgrcv.c
	${CC} ${CFLAGS} $< -o $@


.PHONY : clean

clean :
	${RM} msgsnd.out msgrcv.out
