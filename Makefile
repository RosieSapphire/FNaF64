BUILD_DIR := build

include $(N64_INST)/include/n64.mk

TARGET := fnaf
ROM := $(TARGET).z64
ELF := $(BUILD_DIR)/$(TARGET).elf
DFS := $(BUILD_DIR)/$(TARGET).dfs
CFLAGS := -Wextra -Iinclude

INC_DIRS := include include/engine include/game
SRC_DIRS := src src/engine src/game
H_FILES := $(foreach dir,$(INC_DIRS),$(wildcard $(dir)/*.h))
C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
O_FILES := $(C_FILES:%.c=$(BUILD_DIR)/%.o)
ASSETS_WAV    := $(wildcard assets/*.wav)
ASSETS_CI4    := $(wildcard assets/ci4/*.png)
ASSETS_CI8    := $(wildcard assets/ci8/*.png)
ASSETS_I4     := $(wildcard assets/i4/*.png)
ASSETS_IA4    := $(wildcard assets/ia4/*.png)
ASSETS_CUSTOM := $(wildcard assets/custom/*.png)
ASSETS_TTF    := $(wildcard assets/custom/*.ttf)
ASSETS_CONV := $(addprefix filesystem/,$(notdir $(ASSETS_WAV:%.wav=%.wav64))) \
              $(addprefix filesystem/custom/,$(notdir $(ASSETS_CUSTOM:%.png=%.sprite))) \
              $(addprefix filesystem/ci4/,$(notdir $(ASSETS_CI4:%.png=%.sprite))) \
              $(addprefix filesystem/ci8/,$(notdir $(ASSETS_CI8:%.png=%.sprite))) \
              $(addprefix filesystem/i4/,$(notdir $(ASSETS_I4:%.png=%.sprite))) \
              $(addprefix filesystem/ia4/,$(notdir $(ASSETS_IA4:%.png=%.sprite))) \
              $(addprefix filesystem/custom/,$(notdir $(ASSETS_TTF:%.ttf=%.font64)))

AUDIOCONV_FLAGS ?= --wav-compress 1
MKSPRITE_FLAGS ?= --compress 2
MKFONT_FLAGS ?= --compress 2 --monochrome --size 8

final: $(ROM)
$(ROM): N64_ROM_TITLE="FiveNights-55"
$(ROM): N64_ED64ROMCONFIGFLAGS=-w eeprom4k
$(ROM): $(DFS) 

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem $<

filesystem/custom/%.sprite: assets/custom/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE CUSTOM] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem/custom "$<"

filesystem/ci4/%.sprite: assets/ci4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE CI4] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f CI4 -o filesystem/ci4 "$<"

filesystem/ci8/%.sprite: assets/ci8/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE CI8] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f CI8 -o filesystem/ci8 "$<"

filesystem/i4/%.sprite: assets/i4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE I4] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f I4 -o filesystem/i4 "$<"

filesystem/ia4/%.sprite: assets/ia4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE IA4] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f IA4 -o filesystem/ia4 "$<"

filesystem/custom/%.font64: assets/custom/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem/custom "$<"

$(DFS): $(ASSETS_CONV)
$(ELF): $(O_FILES)

format: $(H_FILES) $(C_FILES)
	clang-format -i --style=file $^

clean:
	rm -rf $(BUILD_DIR) $(ROM)
