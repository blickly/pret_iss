all: compile clean

compile:
	python ${PRET_ISS}/scripts/compile_threads.py . -v

thread0.srec thread0.dump:
	make -f thread0.makefile

thread1.srec thread1.dump:
	make -f thread1.makefile

thread2.srec thread2.dump:
	make -f thread2.makefile

thread3.srec thread3.dump:
	make -f thread3.makefile

thread4.srec thread4.dump:
	make -f thread4.makefile

thread5.srec thread5.dump:
	make -f thread5.makefile

clean:
	$(RM) *.exe *.o prom*.srec prom.h
	
distclean: clean
	$(RM) *.srec 
