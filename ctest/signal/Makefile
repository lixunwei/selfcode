ifdef DEBUG
	CFLAGS=-g
endif

all : sig.out outsig.out

sig.out : sig.c
	$(CC) $(CFLAGS) -o $@ $<

outsig.out : outsig.c
	$(CC) $(CFLAGS) -o $@ $<

.PYHONY : clean

clean :
	$(RM) sig.out
