ROOTDIR = $(N64_INST)
GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
MKSPRITE = $(ROOTDIR)/bin/mksprite
N64TOOL = $(ROOTDIR)/bin/n64tool
N64_AUDIOCONV = $(ROOTDIR)/bin/audioconv64
LINK_FLAGS = -L$(ROOTDIR)/lib -L$(ROOTDIR)/mips64-elf/lib -ldragon -lc -lm -ldragonsys -Tn64.ld
PROG_NAME = FissionFailure64
CFLAGS = -std=gnu99 -march=vr4300 -mtune=vr4300 -O2 -Wall -Werror -I$(ROOTDIR)/include -Iinclude -I/usr/local/include/
ASFLAGS = -mtune=vr4300 -march=vr4300
CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy

all: build

build: ##    Create rom.
	@docker --version &> /dev/null
	@if [ $$? -ne 0 ]; then echo "Building rom..." && make $(PROG_NAME).z64; fi
	@if [ $$? -eq 0 ]; then echo "Building rom inside docker environment..." && make docker-rom; fi

gengfx:##   Generate UI gfx.
	@docker --version &> /dev/null
	@if [ $$? -ne 0 ]; then echo "Generating gfx..." && ./resources/gfx.sh; fi
	@if [ $$? -eq 0 ]; then echo "Generating gfx inside docker environment..." && make docker-gengfx; fi

docker-gengfx: setup
	@docker run --rm -v ${CURDIR}:/game build ./resources/gfx.sh

docker-rom: setup
	@docker run --rm -v ${CURDIR}:/game build make $(PROG_NAME).z64

rebuild: clean build	##  Erase temp files and create rom.

# gfx #
PNGS := $(wildcard resources/gfx/sprites/*.png) $(wildcard resources/gfx/sprites/*/*.png)
SPRITES := $(subst .png,.sprite,$(subst resources/,filesystem/,$(PNGS)))
filesystem/gfx/%.sprite: resources/gfx/%.png
	@mkdir -p `echo $@ | xargs dirname`
	$(MKSPRITE) 16 1 1 $< $@

# sfx #
WAVS := $(wildcard resources/sfx/*.wav) 
WAV64S := $(subst .wav,.wav64,$(subst resources/,filesystem/,$(WAVS)))
filesystem/sfx/%.wav64: resources/sfx/%.wav
	@mkdir -p `echo $@ | xargs dirname`
	$(N64_AUDIOCONV) -o $@ $<

# code #
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)
$(PROG_NAME).elf : $(OBJS)
	$(LD) -o $@ $^ $(LINK_FLAGS)

$(PROG_NAME).bin : $(PROG_NAME).elf
	$(OBJCOPY) -O binary $< $@

# dfs #
$(PROG_NAME).dfs: $(SPRITES) $(WAV64S)
	@mkdir -p ./filesystem/
	@echo `git rev-parse HEAD` > ./filesystem/hash
	$(MKDFSPATH) $@ ./filesystem/

$(PROG_NAME).z64: $(PROG_NAME).bin $(PROG_NAME).dfs
	@rm -f $@
	$(N64TOOL) -t "$(PROG_NAME)" -h $(ROOTDIR)/mips64-elf/lib/header -o $(PROG_NAME).z64 $(PROG_NAME).bin -s 1M $(PROG_NAME).dfs
	$(CHKSUM64PATH) $@

setup:		##    Create dev environment (docker image).
	@docker build -q -t build  - < Dockerfile > /dev/null

resetup:	##  Force recreate the dev environment (docker image).
	@echo "Rebuilding dev environment in docker..."
	@docker build -q -t build  --no-cache  - < Dockerfile > /dev/null

resetup-v:	##  Force recreate the dev environment (docker image).
	@echo "Rebuilding dev environment in docker..."
	@docker build -t build  --no-cache  - < Dockerfile

cen64:		##    Start rom in CEN64 emulator.
	@echo "Starting cen64..."
	$(CEN64_DIR)/cen64 -multithread -noaudio -is-viewer -controller num=1 $(CEN64_DIR)/pifdata.bin $(PROG_NAME).z64

clean:		##    Cleanup temp files.
	@echo "Cleaning up temp files..."
	rm -rf *.z64 *.elf src/*.o *.bin *.dfs ./filesystem

help:		##     Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/:.*##/:/' 
