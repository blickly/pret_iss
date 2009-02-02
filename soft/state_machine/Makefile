LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float
C_FILES=all_threads.c
CC=sparc-elf-gcc
LD=sparc-elf-ld

compile:
	$(PRET_ISS)/scripts/compile_threads.py .

run: compile
	$(PRET_ISS)/src/pret . -1

debug: compile
	$(PRET_ISS)/scripts/csim.py .

%.exe: %.o
#	# This is more lightweight by not linking in any libraries
#	$(LD) $(LDFLAGS) $^ -o $@ -e main
	# This is a normal configuration that supports libraries
	$(CC) $(CCFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

%.srec: %.exe
	sparc-elf-objcopy -O srec $< $@

%.dump: %.exe
	sparc-elf-objdump -D $< > $@

*.o: $(C_FILES) 
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

clean: 
	$(RM) *.exe *.dmp *.o *~ *.srec *.dump

.PHONY: compile run debug clean
