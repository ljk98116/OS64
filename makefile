# directory
DIRECTORY = tools $(patsubst %,kernel/%,$(KDIRECTORY))

KDIRECTORY = init trap console libs debug mm driver

KERNEL_DIRECTORY = $(patsubst %,kernel/%,$(KDIRECTORY))

INCLUDE = $(patsubst %,-I %,$(DIRECTORY))
INCLUDE += -I libs 

TARGET_DIRECTORY = bin

$(TARGET_DIRECTORY):$(DIRECTORY)
	mkdir bin

# compile options
CC = gcc
CFLAGS = -nostdinc -fno-stack-protector -mcmodel=large -fno-builtin -m64 -c $(INCLUDE) -Os

LD = ld
LDFLAGS = -b elf64-x86-64

OBJDUMP = objdump
OBJCOPY = objcopy

# ultimate image file
THOMAS_IMG = bin/thomas.img

# bootloader compile
BOOT = bin/boot.bin
LOADER = bin/loader.bin

$(BOOT):$(TARGET_DIRECTORY)
	@echo compiling boot sector code
	nasm bootloader/boot.asm -I bootloader -o $@
	
$(LOADER):$(TARGET_DIRECTORY)
	@echo compiling loader code
	nasm bootloader/loader.asm -I bootloader -o $@

# kernel compile
KERNEL = bin/kernel.bin 
KERN_C = $(shell find $(KERNEL_DIRECTORY) -name "*.c")
KERN_S = $(shell find $(KERNEL_DIRECTORY) -name "*.S")
KERN_COBJ = $(KERN_C:.c=.o)
KERN_SOBJ = $(KERN_S:.S=.o)

SYSTEM = bin/system

$(KERN_COBJ):%.o:%.c
	@echo ****compiling kernel C source code****
	$(CC) $(CFLAGS) -o $@ $<

$(KERN_SOBJ):%.o:%.S
	@echo ****compiling kernel GAS source code****
	as --64 -o $@ $<

$(SYSTEM): $(TARGET_DIRECTORY) $(KERN_COBJ) $(KERN_SOBJ)
	$(LD) $(LDFLAGS) -T tools/kernel.lds -o $@ $(KERN_SOBJ) $(KERN_COBJ) 
	$(OBJCOPY) -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $@ $(KERNEL)

OBJ = $(shell find $(KERNEL_DIRECTORY) -name "*.o")

# image made
$(THOMAS_IMG):$(BOOT) $(LOADER) $(SYSTEM)
	@echo ****building thomas.img****
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOT) of=$@ bs=512 count=1 conv=notrunc 
	sudo mount $(THOMAS_IMG) mnt/ -t vfat -o loop
	sudo cp $(LOADER) mnt/
	sudo cp $(KERNEL) mnt/
	sync
	sudo umount mnt/

bochs : $(THOMAS_IMG)
	bochs -f tools/bochsrc
	
.PHONY:all
all:$(THOMAS_IMG)

.PHONY:clean
clean:
	rm -rf bin $(OBJ)
