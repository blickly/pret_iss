LDFLAGS=$(filter-out -D%,$(CFLAGS))
#LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float -O1
C_FILES=all_threads.c
CC=sparc-elf-gcc
LD=sparc-elf-ld

all: clean compile

compile:
	$(PRET_ISS)/scripts/compile_threads.py .

run:
	$(PRET_ISS)/src/pret . -1

debug:
	$(PRET_ISS)/scripts/csim.py .

thread%.o: $(C_FILES) 
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

thread%.exe: thread%.o
# This is more lightweight by not linking in any libraries
#	$(LD) $(LDFLAGS) $^ -o $@ -e main
# This is a normal configuration that supports libraries	
	$(CC) $(LDFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

thread%.srec: thread%.exe
	sparc-elf-objcopy -O srec $< $@

thread%.dump: thread%.exe
	sparc-elf-objdump -D $< > $@

clean: 
	$(RM) *.exe *.dmp *.o *~ *.srec *.dump

.PHONY: compile run debug clean
