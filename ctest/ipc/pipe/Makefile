ifdef DEBUG
	CFLAGS+=-g
endif

Result := pipe.out

all : $(Result)

$(Result) : %.out : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean

clean :
	$(RM) $(Result)
