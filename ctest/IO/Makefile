ifdef DEBUG
	CFLAGS+=-g
endif

Result := fcntl.out

all : $(Result)

$(Result) : %.out : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean

clean :
	$(RM) $(Result)
