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
	gcc $(CFLAGS) -o t12_$@ $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGETS) t12_*
	rm -f stat.txt
