SRCS=test_suffix.c
OBJS=${SRCS:.c=}

INCLUDES =  -I ../include
CC = gcc
LIBTREAP =  ../lib/libsuffixtree.a
CFLAGS = -g -ansi -Wall $(INCLUDES)

COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS)

all: $(OBJS)

test_treap: test_treap.c $(LIBTREAP)


clean:
	rm -f $(OBJS)

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
