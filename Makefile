ARES_BIN := /Applications/ares.app/Contents/MacOS/ares

BUILD_DIR := build
SOURCE_DIR := src
ROM_NAME := FissionFailure64
RELEASE_ROM_NAME := FissionFailure64-release
BUILD_TYPE ?= debug

N64_MK_PATH := $(N64_INST)/include/n64.mk
ifneq (,$(wildcard $(N64_MK_PATH)))
include $(N64_MK_PATH)
endif

N64_CFLAGS += -Iinclude -Wall -Werror
ifeq ($(BUILD_TYPE),release)
N64_CFLAGS += -DNDEBUG
else
N64_CFLAGS += -O0 -g
endif

.PHONY: all build docker gengfx docker-gengfx rebuild release setup resetup resetup-v ares ares-release cen64 clean help

all: build

build: ##    Create rom.
	@if command -v docker >/dev/null 2>&1; then \
		echo "Building rom inside docker environment..."; \
		$(MAKE) docker; \
	else \
		echo "Building rom..."; \
		$(MAKE) $(ROM_NAME).z64; \
	fi

docker: setup
	@docker run --rm --user $(shell id -u):$(shell id -g) -v ${CURDIR}:/game build make BUILD_TYPE=$(BUILD_TYPE) $(ROM_NAME).z64

gengfx: ##   Generate UI gfx.
	@if command -v docker >/dev/null 2>&1; then \
		echo "Generating gfx inside docker environment..."; \
		$(MAKE) docker-gengfx; \
	else \
		echo "Generating gfx..."; \
		./resources/gfx.sh; \
	fi

docker-gengfx: setup
	@docker run --rm --user $(shell id -u):$(shell id -g) -v ${CURDIR}:/game build ./resources/gfx.sh

rebuild: clean build	##  Erase temp files and create rom.

release: clean ##  Create release rom.
	@$(MAKE) BUILD_TYPE=release build
	@mv -f $(ROM_NAME).z64 $(RELEASE_ROM_NAME).z64
	@echo "    [RELEASE] $(RELEASE_ROM_NAME).z64"

# gfx #
PNGS := $(wildcard resources/gfx/sprites/*.png) $(wildcard resources/gfx/sprites/*/*.png)
SPRITES := $(subst .png,.sprite,$(subst resources/,filesystem/,$(PNGS)))
filesystem/gfx/%.sprite: resources/gfx/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f RGBA16 -o $(dir $@) $<

# sfx #
WAVS := $(wildcard resources/sfx/*.wav)
WAV64S := $(subst .wav,.wav64,$(subst resources/,filesystem/,$(WAVS)))
filesystem/sfx/%.wav64: resources/sfx/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIOCONV] $@"
	@$(N64_AUDIOCONV) --wav-mono --wav-loop false --wav-compress 0 -o $(dir $@) $<

# code #
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
OBJS := $(SRCS:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

-include $(DEPS)

$(ROM_NAME).z64: N64_ROM_TITLE = "FissionFailure64"

# Dependencies - let n64.mk handle the rules
$(BUILD_DIR)/$(ROM_NAME).elf: $(OBJS)
$(ROM_NAME).z64: $(BUILD_DIR)/$(ROM_NAME).dfs

# dfs #
$(BUILD_DIR)/$(ROM_NAME).dfs: $(SPRITES) $(WAV64S)
	@mkdir -p ./filesystem/
	@echo `git rev-parse HEAD` > ./filesystem/hash
	@echo "    [DFS] $@"
	@$(N64_MKDFS) $@ ./filesystem/ >/dev/null

setup:		##    Create dev environment (docker image).
	@docker build --platform linux/amd64 -t build - < Dockerfile

resetup:	##  Force recreate the dev environment (docker image).
	@echo "Rebuilding dev environment in docker..."
	@docker build --platform linux/amd64 -t build --no-cache - < Dockerfile

resetup-v:	##  Force recreate the dev environment (docker image).
	@echo "Rebuilding dev environment in docker..."
	@docker build --platform linux/amd64 -t build --no-cache - < Dockerfile

ares: build		##    Start rom in Ares emulator.
	@echo "Starting ares..."
	@mkdir -p .ares/saves
	$(ARES_BIN) --setting "Paths/Saves=$(CURDIR)/.ares/saves" $(ROM_NAME).z64

ares-release: release	## Start release rom in Ares emulator.
	@echo "Starting ares (release)..."
	@mkdir -p .ares/saves
	$(ARES_BIN) --setting "Paths/Saves=$(CURDIR)/.ares/saves" $(RELEASE_ROM_NAME).z64

cen64:		##    Start rom in CEN64 emulator.
	@echo "Starting cen64..."
	$(CEN64_DIR)/cen64 -multithread -noaudio -is-viewer -controller num=1 $(CEN64_DIR)/pifdata.bin $(ROM_NAME).z64

clean:		##    Cleanup temp files.
	@echo "Cleaning up temp files..."
	rm -rf $(BUILD_DIR) *.z64 *.elf src/*.o *.bin *.dfs filesystem/

help:		##     Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/:.*##/:/'
