ifdef DEBUG
	CFLAGS=-g
endif

all: msgsnd.out msgrcv.out posix_msg.out posix_send.out posix_rcv.out


msgsnd.out : msgsnd.c
	${CC} ${CFLAGS} $< -o $@

msgrcv.out : msgrcv.c
	${CC} ${CFLAGS} $< -o $@

posix_msg.out : posix_msg.c
	${CC} ${CFLAGS} $< -o $@ -lrt

posix_send.out : posix_send.c
	${CC} ${CFLAGS} $< -o $@ -lrt

posix_rcv.out : posix_rcv.c
	${CC} ${CFLAGS} $< -o $@ -lrt

.PHONY : clean

clean :
	${RM} msgsnd.out msgrcv.out posix_msg.out
