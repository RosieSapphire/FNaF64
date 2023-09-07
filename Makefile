GAME=fnaf
CFLAGS=-Wextra -Iinclude
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = $(wildcard src/*.c src/*/*.c)
assets_wav    = $(wildcard assets/*.wav)
assets_ci4    = $(wildcard assets/ci4/*.png)
assets_ci8    = $(wildcard assets/ci8/*.png)
assets_i4     = $(wildcard assets/i4/*.png)
assets_ia4    = $(wildcard assets/ia4/*.png)
assets_custom = $(wildcard assets/custom/*.png)
assets_ttf    = $(wildcard assets/custom/*.ttf)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_wav:%.wav=%.wav64))) \
              $(addprefix filesystem/custom/,$(notdir $(assets_custom:%.png=%.sprite))) \
              $(addprefix filesystem/ci4/,$(notdir $(assets_ci4:%.png=%.sprite))) \
              $(addprefix filesystem/ci8/,$(notdir $(assets_ci8:%.png=%.sprite))) \
              $(addprefix filesystem/i4/,$(notdir $(assets_i4:%.png=%.sprite))) \
              $(addprefix filesystem/ia4/,$(notdir $(assets_ia4:%.png=%.sprite))) \
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

filesystem/ci4/%.sprite: assets/ci4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f CI4 -o filesystem/ci4 "$<"

filesystem/ci8/%.sprite: assets/ci8/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f CI8 -o filesystem/ci8 "$<"

filesystem/i4/%.sprite: assets/i4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f I4 -o filesystem/i4 "$<"

filesystem/ia4/%.sprite: assets/ia4/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -f IA4 -o filesystem/ia4 "$<"

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
