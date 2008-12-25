LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float
SREC=bare_thread$(THREAD_NUM).srec
EXE=bare_thread$(THREAD_NUM).exe
DUMP=bare_thread$(THREAD_NUM).dump
CC=sparc-elf-gcc
LD=sparc-elf-ld


thread$(THREAD_NUM)_all: $(SREC) $(DUMP)

$(SREC): $(EXE)
	sparc-elf-objcopy -O srec $< $@

$(DUMP): $(EXE)
	sparc-elf-objdump -D $< > $@

$(EXE): $(C_FILES:.c=.o) 
	# This is more lightweight by not linking in any libraries
	#$(LD) $(LDFLAGS) $(LIBDIR) $(LIBS) $^ -o $@ 
	#	 This is a normal configuration that supports libraries
	$(CC) $(CCFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) $(INCDIR) -c $<

clean: 
	$(RM) *.exe *.dump *.srec *.o prom.h

