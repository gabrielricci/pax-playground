cmd_/scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/spi/.install := perl scripts/headers_install.pl /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/include/linux/spi /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/spi arm spidev.h; perl scripts/headers_install.pl /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/include/linux/spi /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/spi arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/spi/$$F; done; touch /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/spi/.install