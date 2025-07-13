#include <stdlib.h>

#include "engine/graphic.h"
#include "engine/sfx.h"

#include "game/texture_index.h"
#include "game/golden_freddy_scare.h"

#define GOLDFRED_SCARE_TIMER_CRASH 1.f

bool                  golden_freddy_is_scaring;

static struct graphic golden_freddy_gfx_scare;
static        float   golden_freddy_scare_timer;
static        bool    golden_freddy_was_scaring;
static        bool    golden_freddy_scare_is_loaded = false;

void golden_freddy_scare_load(void)
{
        if (golden_freddy_scare_is_loaded)
                return;

        graphic_load(&golden_freddy_gfx_scare, TX_GOLDEN_FREDDY_SCARE, true);
        golden_freddy_scare_timer = 0.f;
        golden_freddy_is_scaring  = false;
        golden_freddy_was_scaring = false;

        golden_freddy_scare_is_loaded = true;
}

void golden_freddy_scare_draw(void)
{
        rdpq_set_mode_copy(true);
        graphic_draw(&golden_freddy_gfx_scare, 0, 0, 0, 0, GFX_FLIP_NONE);
}

void golden_freddy_scare_update(const float dt)
{
        debugf("%f\n", golden_freddy_scare_timer);

        if (!golden_freddy_was_scaring) {
                sfx_stop_all_channels();
                mixer_ch_set_vol(SFX_CH_JUMPSCARE, .8f, .8f);
                wav64_play(&sfx_jumpscare_low, SFX_CH_JUMPSCARE);
                golden_freddy_was_scaring = true;
        }

        golden_freddy_scare_timer += dt;
        if (golden_freddy_scare_timer < GOLDFRED_SCARE_TIMER_CRASH)
                return;

        uint8_t *crash;

        crash = NULL;
        *crash = 69;
}

void golden_freddy_scare_unload(void)
{
        if (!golden_freddy_scare_is_loaded)
                return;

        graphic_unload(&golden_freddy_gfx_scare);

        golden_freddy_scare_is_loaded = false;
}
