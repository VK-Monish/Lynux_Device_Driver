obj-m += mod_timer.o 

KDIR := /lib/modules/$(shell uname -r)/build
M := $(shell pwd)

.PHONY: clean all

all:
	make -C $(KDIR) M=$(M) modules

clean:	
	make -C $(KDIR) M=$(M) clean
