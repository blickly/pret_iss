LDFLAGS=$(filter-out -D%,$(CFLAGS))   
#LDFLAGS=$(CFLAGS)
CCFLAGS=$(CFLAGS) -msoft-float
C_FILES=all_threads.c
CC=sparc-elf-gcc
LD=sparc-elf-ld

all: clean compile

compile:
	$(PRET_ISS)/scripts/compile_threads.py .

simulate:
	$(PRET_ISS)/real_world_simulators/robot_simulator/game.py -p .

debug:
	$(PRET_ISS)/scripts/csim.py .

thread%.o: $(C_FILES) 
	cp $(PRET_ISS)/soft/deadline.h .
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@
	rm deadline.h

thread%.exe: thread%.o
	$(LD) $(LDFLAGS) $^ -o $@ -e main  
#	$(CC) $(CCFLAGS) $(INCDIR) $(LIBDIR) $^ -o $@ $(LIBS)

thread%.srec: thread%.exe
	sparc-elf-objcopy -O srec $< $@

thread%.dump: thread%.exe
	sparc-elf-objdump -D $< > $@

clean: 
	$(RM) *.exe *.dmp *.o *~ *.srec *.dump deadline.h

.PHONY: compile run debug clean
