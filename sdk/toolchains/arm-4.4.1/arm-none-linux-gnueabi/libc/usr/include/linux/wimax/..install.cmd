cmd_/scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/wimax/.install := perl scripts/headers_install.pl /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/include/linux/wimax /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/wimax arm i2400m.h; perl scripts/headers_install.pl /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/include/linux/wimax /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/wimax arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/wimax/$$F; done; touch /scratch/nsidwell/arm/linux/obj/linux-2012.03-57-arm-none-linux-gnueabi-i686-pc-linux-gnu/tmp-install/include/linux/wimax/.install
