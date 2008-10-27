all: compile clean

compile:
	python compile_threads.py

clean:
	$(RM) *.exe *.o bare_thread*.srec prom*.srec prom.h
	
distclean: clean
	$(RM) *.srec
