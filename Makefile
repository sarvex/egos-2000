all: binary
	@echo "$(YELLOW)-------- Create the Disk Image --------$(END)"
	$(CC) $(TOOLS)/mkfs.c library/file/file.c -DMKFS $(INCLUDE) -o $(TOOLS)/mkfs
	cd $(TOOLS); ./mkfs

binary:
	mkdir -p $(DEBUG) $(RELEASE)
	@echo "$(GREEN)-------- Compile the Grass Layer --------$(END)"
	$(RISCV_CC) $(COMMON) $(GRASS_SRCS) $(GRASS_LD) -o $(RELEASE)/grass.elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(RELEASE)/grass.elf > $(DEBUG)/grass.lst
	@echo "$(YELLOW)-------- Compile the Earth Layer --------$(END)"
	$(RISCV_CC) $(COMMON) $(EARTH_SRCS) $(EARTH_LD) -o $(RELEASE)/earth.elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(RELEASE)/earth.elf > $(DEBUG)/earth.lst

clean:
	rm -rf build
	rm -rf $(TOOLS)/mkfs $(TOOLS)/mkrom
	rm -rf $(TOOLS)/disk.img $(TOOLS)/bootROM.bin $(TOOLS)/bootROM.mcs

RISCV_CC = riscv64-unknown-elf-gcc
OBJDUMP = riscv64-unknown-elf-objdump
OBJCOPY = riscv64-unknown-elf-objcopy

APPS_SRCS = apps/app.S library/*/*.c
GRASS_SRCS = grass/grass.S grass/*.c library/elf/*.c
EARTH_SRCS = earth/earth.S earth/*.c earth/sd/*.c library/elf/*.c library/libc/*.c

CFLAGS = -march=rv32i -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -nostartfiles -nostdlib
INCLUDE = -Ilibrary -Ilibrary/elf -Ilibrary/libc -Ilibrary/file -Ilibrary/servers

COMMON = $(CFLAGS) $(LDFLAGS) $(INCLUDE) -D CPU_CLOCK_RATE=65000000

APPS_LD = -Tapps/app.lds -lc -lgcc
GRASS_LD = -Tgrass/grass.lds -lc -lgcc
EARTH_LD = -Tearth/earth.lds -lc -lgcc

TOOLS = tools
DEBUG = build/debug
RELEASE = build/release
OBJDUMP_FLAGS =  --source --all-headers --demangle --line-numbers --wide

GREEN = \033[1;32m
YELLOW = \033[1;33m
CYAN = \033[1;36m
END = \033[0m
