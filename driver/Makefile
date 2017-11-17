
KERNEL_VER:=$(shell uname -r)
KERNEL_DIR:=/lib/modules/$(KERNEL_VER)/build
INSTALL_DIR:=/lib/modules/$(KERNEL_VER)/ttyPos

obj-m := ttyPos.o


all:
	$(MAKE) modules -C $(KERNEL_DIR) SUBDIRS=$(shell pwd)

clean:
	$(RM) *.o *.ko *.mod.* .*.cmd *~
	$(RM) -r .tmp_versions

install: all
	install -D -m 644 ttyPos.ko $(INSTALL_DIR)/ttyPos.ko
	/sbin/depmod -a
uninstall:
	modprobe -r ttyPos ; echo -n
	$(RM) $(INSTALL_DIR)/ttyPos.ko
	/sbin/depmod -a

