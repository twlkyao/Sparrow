CROSS_COMPILE	:= 

CC	:= $(CROSS_COMPILE)gcc
LD	:= $(CROSS_COMPILE)ld
AR	:= $(CROSS_COMPILE)ar
NM	:= $(CROSS_COMPILE)nm

CFLAGS	:= -c -Werror -g -I$(CURDIR)/../include -I$(CURDIR)/.. -I/usr/local/include -D__KERNEL__ -D__ARCH_X86__ 
#-DSLAB_OPTIMIZE

subdirs := fs/ mm/ util/ glibc/ shell/

test := test

objects := test.o

.PHONY: $(subdirs)

all: $(subdirs) $(test)

$(subdirs):
	$(MAKE) --directory=$@

$(test): $(objects) fs/built-in.o mm/built-in.o util/built-in.o glibc/built-in.o shell/built-in.o
	$(CC) fs/built-in.o mm/built-in.o util/built-in.o glibc/built-in.o shell/built-in.o $(objects) -o $@ -L/usr/local/lib -lcunit

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o $(objects) $(test)
	for d in $(subdirs);	\
		do	\
			$(MAKE) --directory=$$d clean || exit 1;	\
		done
