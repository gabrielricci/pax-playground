# SumUp PAX application makefile
# @author Gabriel Ricci <gabriel.ricci@sumup.com>
#
# ATTENTION: This is supposed to be executed from within the compiler box, which
#            is usualluy vagrant.
#
# Usage: make all

RM    := rm -rf
CC    := /pax-sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-gcc
STRIP := /pax-sdk/toolchains/arm-4.4.1/bin/arm-none-linux-gnueabi-strip

PAX_INCS := -I"/pax-sdk/platforms/paxngfp_201205/include"
PAX_INCS += -I"/pax-sdk/platforms/paxngfp_201205/include/freetype2"
PAX_INCS += -I"/pax-sdk/toolchains/arm-4.4.1/arm-none-linux-gnueabi/libc/usr/include"
PAX_INCS += -I"/pax-sdk/toolchains/arm-4.4.1/lib/gcc/arm-none-linux-gnueabi/4.6.3/include"

CFLAGS  := -O0 -g2 -Wall -funwind-tables -I"inc/" -I"src/" $(PAX_INCS)
LDFLAGS := -L"/pax-sdk/platforms/paxngfp_201205/lib" -Wl,-rpath=//opt/lib -Wl,-rpath=./lib -Wl,-rpath-link,"/pax-sdk/platforms/paxngfp_201205/lib" -L"lib/"
LDLIBS  := -losal -lcrypto -lfreetype -lpng -lpthread -lts -lxui
# LDLIBS  := -lClEntryS2FMProlin_v503 -lEMVS2FMProlin_v601 -lMC_Prolin_v502 -lClWaveS2FMProlin_v303 -lClqPBOCS2FMProlin_v300 -losal -lui_linux -lui_icon -lcrypto -lnvram -lfreetype -lpng -lpthread -lts -lxui


SRCS := $(wildcard **/*.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
NOSTRIP := app.nostrip

all: app

app: $(NOSTRIP)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Strip'
	$(STRIP) -g $(NOSTRIP) -o"App"
	@echo 'Finished building target: $@'
	@echo ' '

app.nostrip: $(OBJS)
	@echo 'Invoking: GCC Linker'
	$(CC) -o"app.nostrip" $(OBJS) $(LDFLAGS) $(LDLIBS)
	@echo 'Finished building: $@'
	@echo ' '

%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Compiler'
	$(CC) $(CFLAGS) -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean:
	$(RM) $(OBJS) $(NOSTRIP) App
	@echo ' '
