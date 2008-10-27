ifeq ($(shell uname),Linux)
  THREAD = -pthread
endif

CC     = g++
OPT    = -O3
DEBUG  = -g
OTHER  = -Wall
CFLAGS = $(OPT) $(OTHER) $(THREAD)
#CFLAGS = $(DEBUG) $(OTHER)

MODULE = run

SRCS =  main.cpp debugger.cpp srec_parser.cpp cycle_counter.cpp instruction.cpp \
        module_base.cpp register_file.cpp special_reg.cpp \
        hw_thread.cpp hw_thread_ptr.cpp hw_thread_controller.cpp \
        memory_unit.cpp mem_location.cpp memory_controller.cpp \
        fetch.cpp decode.cpp regacc.cpp execute.cpp mem.cpp except.cpp

OBJS = $(SRCS:.cpp=.o)

include ./Makefile.defs
