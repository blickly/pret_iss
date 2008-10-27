
THREAD_NUM=3
INCDIR= 
LIBDIR= 
LIBS= -lm 
CFLAGS+=-O3


C_FILES=thread$(THREAD_NUM).c

include thread_common.makefile
