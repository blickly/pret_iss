LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float
SREC=thread$(THREAD_NUM).srec
EXE=thread$(THREAD_NUM).exe
DUMP=thread$(THREAD_NUM).dump
CC=sparc-elf-gcc
LD=sparc-elf-ld


thread$(THREAD_NUM)_all: $(SREC) $(DUMP)

$(SREC): $(EXE)
	sparc-elf-objcopy -O srec $< $@

$(DUMP): $(EXE)
	sparc-elf-objdump -D $< > $@

$(EXE): $(C_FILES:.c=.o) 
	# This is a lightweight linking that does not include any libraries
	$(LD) $(LDFLAGS) $^ -o $@ -e main

.c.o:
	$(CC) $(CCFLAGS) $(INCDIR) -c $<

clean: 
	$(RM) *.exe *.dump *.srec *.o prom.h

