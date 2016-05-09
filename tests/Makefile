SRCS=test_suffix_tree.c
OBJS=${SRCS:.c=}

INCLUDES =  -I ../include
CC = gcc
LIBSUFFIXTREE =  ../lib/libsuffixtree.a
CFLAGS = -g -ansi -Wall $(INCLUDES)

COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS)

all: $(OBJS)

test_suffix_tree: test_suffix_tree.c $(LIBSUFFIXTREE)


clean:
	rm -f $(OBJS)

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
