SRCS=$(shell ls *.c)
OBJS=$(SRCS:.c=.o)
TARGETS=$(OBJS:.o=)
CFLAGS := -g 
LIBS := -pthread -lrt

.c.o:
	gcc $(CFLAGS) -c -o $@ $<

.PHONEY: all

all: $(TARGETS)

$(TARGETS): %: %.o
	gcc $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGETS)
	rm -f stat.txt
