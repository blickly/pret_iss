
INCDIR= 
LIBDIR= 
LIBS= -lm
THREAD_NUM=1
C_FILES=thread$(THREAD_NUM).c
CFLAGS+=-O0 -DPRET


include thread_common.makefile
