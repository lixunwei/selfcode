ifdef DEBUG
	CFLAGS=-g
endif

Result = msgsnd.out msgrcv.out posix_msg.out posix_send.out \
		 posix_rcv.out posix_trcv.out

all : $(Result)

$(filter posix_%.out,$(Result)) : LDFLAGS=-lrt

$(Result) : %.out : %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY : clean

clean :
	$(RM) $(Result)
