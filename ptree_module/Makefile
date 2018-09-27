obj-m := ptree.o
KDIR := ~/goldfish

all:
		make -C $(KDIR) M=$(PWD) modules

clean:
		make -C $(KDIR) M=$(PWD) clean
