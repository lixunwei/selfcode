ifdef DEBUG
	CFLAGS+=-g
endif

Result := file_lock.out

all : $(Result)

$(Result) : %.out : %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean

clean :
	$(RM) $(Result)
