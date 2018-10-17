OUTDIR := ./out
RBSRC := ./rb-tree
BINDIR := $(addprefix $(OUTDIR)/,bin)
TMPDIR := $(addprefix $(OUTDIR)/,obj)
RBOBJ := $(patsubst $(RBSRC)/%.c,$(TMPDIR)/%.o,$(wildcard $(RBSRC)/*.c))

rb-tree : mkoutdir rb.out

rb.out:$(RBOBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(RBOBJ):$(TMPDIR)/%.o:$(RBSRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : mkoutdir clean testapi

testapi :
	@echo "testapi"
	@echo "${mkfile_path}"
	@echo "${andcond}"
	@echo "${RBSOURCE}"
	@echo "${RBOBJ}"

mkoutdir :
	@echo "Create BIN and OBJ dir"
	@if [ ! -d ${BINDIR} -o ! -d ${TMPDIR} ];then\
		mkdir -p ${BINDIR};\
		mkdir -p ${TMPDIR};\
	fi

clean :
	@echo "Clean All tmp file and result"
	$(RM) $(TMPDIR)/* $(BINDIR)/*
	$(RM) rb.out
