
INCDIR= 
LIBDIR= 
LIBS= -lm
THREAD_NUM=1
C_FILES=thread$(THREAD_NUM).c
CFLAGS+=-O3


include thread_common.makefile
