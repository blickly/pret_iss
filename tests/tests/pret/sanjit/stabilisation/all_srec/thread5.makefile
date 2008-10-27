
THREAD_NUM=5
C_FILES=thread$(THREAD_NUM).c
LIBDIR= 
LIBS= -lm 
CFLAGS+= -O0 -DPRET

include thread_common.makefile
