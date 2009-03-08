LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float -mflat
CLANGFLAGS=$(filter-out -T%,$(LDFLAGS)) -triple=sparc-elf-linux
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

thread%.rewritten.c: $(C_FILES)
	clang $(CLANGFLAGS) -rewrite-pret $<
	mv all_threads.rewritten.c $@

thread%.o: thread%.rewritten.c
	cp $(PRET_ISS)/soft/deadline.h .
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

#thread%.o: $(C_FILES) 
#	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

thread%.exe: thread%.o
	$(CC) $(CCFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

thread%.srec: thread%.exe
	sparc-elf-objcopy -O srec $< $@

thread%.dump: thread%.exe
	sparc-elf-objdump -D $< > $@

clean: 
	$(RM) *.exe *.dmp *.o *~ *.srec *.dump *.rewritten.c deadline.h

.PHONY: compile run debug clean
