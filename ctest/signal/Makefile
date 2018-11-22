ifdef DEBUG
	CFLAGS+=-g
endif

Result := sig.out outsig.out rt-signal.out

all : $(Result)

$(Result) : %.out : %.c
	$(CC) $(CFLAGS) -o $@ $<

.PYHONY : clean

clean :
	$(RM) $(Result)
