
THREAD_NUM=4
INCDIR= 
LIBDIR= 
LIBS= -lm 
CFLAGS+= -O2
C_FILES=thread$(THREAD_NUM).c

include thread_common.makefile
