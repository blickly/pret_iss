LDFLAGS=$(filter-out -D%,$(CFLAGS))   
CCFLAGS=$(CFLAGS) -msoft-float
CLANGFLAGS=$(filter -D%,$(CFLAGS))
C_FILES=all_threads.c
CC=sparc-elf-gcc
LD=sparc-elf-ld
ISPM_FILES=thread0.ispm thread1.ispm thread2.ispm thread3.ispm thread4.ispm thread5.ispm

all: clean compile

compile: $(ISPM_FILES)
	$(PRET_ISS)/scripts/compile_threads.py .

simulate:
	$(PRET_ISS)/real_world_simulators/robot_simulator/game.py -p .

simulatedumpdata:
	$(PRET_ISS)/real_world_simulators/robot_simulator/game.py -p . -s

simulatefromdata:
	$(PRET_ISS)/real_world_simulators/robot_simulator/game.py -l

debug:
	$(PRET_ISS)/scripts/csim.py .

thread%.ispm: thread%.ispm.conf thread%.dump
	$(PRET_ISS)/scripts/extract_ispm_alloc.py $<

grid.txt.h: grid.txt
	tac $< | sed 's/$$/"/' | sed 's/^/,\n"/' | tail -n +2 > $@

deadline.h:
	cp $(PRET_ISS)/soft/deadline.h .

$(ISPM_FILES:ispm=c): $(C_FILES) deadline.h
	clang -triple=sparc-elf-linux -rewrite-pret $(CLANGFLAGS) $(INCDIR) $<
	mv $(<:c=rewritten.c) $@

thread%.o: thread%.c grid.txt.h deadline.h
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

thread%.exe: thread%.o
#	$(LD) $(LDFLAGS) $^ -o $@ -e main  
	$(CC) $(CCFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

thread%.srec: thread%.exe
	sparc-elf-objcopy -O srec $< $@

thread%.dump: thread%.exe
	sparc-elf-objdump -D $< > $@

clean: 
	$(RM) *.exe *.dmp *.o *~ *.srec *.dump deadline.h

.PHONY: all compile simulate simulatedumpdata simulatefromdata debug clean
