all: compile clean

compile:
	python ${PRET_ISS}/scripts/compile_threads.py . -v

clean:
	$(RM) *.exe *.o bare_thread*.srec prom*.srec prom.h
	
distclean: clean
	$(RM) *.srec
