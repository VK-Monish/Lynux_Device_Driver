obj-m += file.o

KDIR = /lib/modules/$(shell uname -r)/build

M := $(shell pwd)

.PHONY: all clean

all:
	make -C $(KDIR) M=$(M) modules 
clean:
	make -C $(KDIR) M=$(M) clean 
