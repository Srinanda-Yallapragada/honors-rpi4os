CFILES = $(wildcard *.c) # all the files containing working code
OFILES = $(CFILES:.c=.o) # all the object binaries from the c files
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles 
# Wall enables all compiler warnings 
# O2 -  Optimize even more. GCC performs nearly all supported optimizations that do not involve a space-speed tradeoff. The compiler does not perform loop unrolling or function inlining when you specify -O2. 
# -ffreestanding A freestanding environment is one in which the standard library may not exist, and program startup may not necessarily be at "main". The option -ffreestanding directs the compiler to not assume that standard functions have their usual definition. https://stackoverflow.com/questions/17692428/what-is-ffreestanding-option-in-gcc  Do not use the standard  system  startup  files  or  libraries  when
# 
# linking.   No  startup  files and only the libraries you specify are
# passed to the linker, and options specifying linkage of  the  system
# libraries, such as -static-libgcc or -shared-libgcc, are ignored.#
#
#

GCCPATH=/home/nandu/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin
# absolute path to arm cross compiler latest release instead of the one used in rpi4os.com tutorial
all: clean kernel8.img
# kernel8.img is the main kernal file that needs to be put onto the SD card

boot.o: boot.S
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot.S -o boot.o
	# elf is executable and linking format. there are elf files on the sd card from Raspbian
	# This line compiles the boot assembly code using the cross compiler

%.o: %.c
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

	# linking all the object files and creating the kernal8.img 

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true
	#remove all compiled code
