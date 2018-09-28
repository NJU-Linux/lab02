obj-m := ptree.o
KDIR := ~/kernel

all:
		make -C $(KDIR) M=$(PWD) modules

clean:
		make -C $(KDIR) M=$(PWD) clean
