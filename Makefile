GAME=fnaf
CFLAGS=-Wextra -Iinclude
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = $(wildcard src/*.c src/*/*.c)
assets_wav =        $(wildcard assets/*.wav)
assets_png =        $(wildcard assets/*.png)
assets_png_custom = $(wildcard assets/custom/*.png)
assets_ttf =        $(wildcard assets/custom/*.ttf)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64))) \
              $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
              $(addprefix filesystem/custom/,$(notdir $(assets_png_custom:%.png=%.sprite))) \
              $(addprefix filesystem/custom/,$(notdir $(assets_ttf:%.ttf=%.font64)))

AUDIOCONV_FLAGS=# --wav-compress 1
MKSPRITE_FLAGS=-c 1
MKFONT_FLAGS=--size 8

all: $(GAME).z64

filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o filesystem $<

filesystem/custom/%.sprite: assets/custom/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem/custom "$<"

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"

filesystem/custom/%.font64: assets/custom/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem/custom "$<"

$(BUILD_DIR)/$(GAME).dfs: $(assets_conv)
$(BUILD_DIR)/$(GAME).elf: $(src:%.c=$(BUILD_DIR)/%.o)

$(GAME).z64: N64_ROM_TITLE="FiveNights-55"
$(GAME).z64: $(BUILD_DIR)/$(GAME).dfs 
$(GAME).z64: N64_ED64ROMCONFIGFLAGS=-w eeprom4k

clean:
	rm -rf $(BUILD_DIR) $(GAME).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
