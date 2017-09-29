# compiler to use
CC = clang

# compiler flags to pass
CFLAGS = -g -O0 -Qunused-arguments -std=c11 -Wall

# name for executable
EXE = expense

# space-separated list of header files
HDRS = sllnode.h

# space-separated list of source files
SRCS = sllnode.c expense.c

# automatically generated list of object(.o) files
OBJS = $(SRCS:.c=.o)

# default target
$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# dependencies
$(OBJS) : $(HDRS) Makefile

# housekeeping
clean:
	rm -f core $(EXE) *.o *.csv
