cmd_/pax-driver/ttyPos.ko := ld -r -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o /pax-driver/ttyPos.ko /pax-driver/ttyPos.o /pax-driver/ttyPos.mod.o
