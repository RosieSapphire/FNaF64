#include <stdlib.h>

#include "engine/graphic.h"
#include "engine/util.h"
#include "engine/sfx.h"

#include "game/office.h"
#include "game/camera.h"
#include "game/hallucinations.h"
#include "game/bonnie.h"
#include "game/texture_index.h"
#include "game/golden_freddy.h"

#define GOLDEN_FREDDY_JUMPSCARE_TIMER_START_SCARING 5.f
#define GOLDEN_FREDDY_JUMPSCARE_TIMER_CRASH_GAME 1.5f

static struct graphic golden_freddy_gfx_in_office;
static struct graphic golden_fready_gfx_scare;
static        float   golden_freddy_attempt_timer;
static        float   golden_freddy_jumpscare_timer;

int golden_freddy_state;

void golden_freddy_load(void)
{
        golden_freddy_attempt_timer = 0;
        graphic_load(&golden_freddy_gfx_in_office, TX_GOLDEN_FREDDY_SIT);
        graphic_load(&golden_fready_gfx_scare, TX_GOLDEN_FREDDY_SCARE);
        golden_freddy_jumpscare_timer = 0.0f;
        golden_freddy_state = 0;
}

void golden_freddy_draw_in_room(const float office_turn)
{
        if (golden_freddy_state == 3) {
                rdpq_set_mode_copy(true);
                graphic_draw(golden_freddy_gfx_in_office, 660 + office_turn,
                             478, 270, 260, GFX_FLIP_NONE);
        }
}

void golden_freddy_draw_scare(void)
{
        rdpq_set_mode_copy(true);
        graphic_draw(golden_fready_gfx_scare, 0, 0, 0, 0, GFX_FLIP_NONE);
}

void golden_freddy_update(double dt)
{
        switch(golden_freddy_state) {
        case GOLDEN_FREDDY_STATE_ATTEMPT:
                bool attempt;

                golden_freddy_attempt_timer =
                        wrapf(golden_freddy_attempt_timer + dt,
                              1.0f, &attempt);
                if (attempt && (rand() % 10) == 1) {
                        golden_freddy_state = GOLDEN_FREDDY_STATE_IN_CAM2B;
                }

                return;

        case GOLDEN_FREDDY_STATE_IN_CAM2B:
                if (cam_selected == CAM_2B &&
                    bonnie_cam != CAM_2B &&
                    camera_is_visible) {
                        golden_freddy_state =
                                        GOLDEN_FREDDY_STATE_READY_TO_ATTACK;
                        mixer_ch_set_vol(SFX_CH_FREDDYLAUGH, 1.0f, 1.0f);
                        wav64_play(&sfx_girl_giggle, SFX_CH_FREDDYLAUGH);
                }

                return;

        case GOLDEN_FREDDY_STATE_READY_TO_ATTACK:
                if (!camera_is_visible) {
                        hallucinations_trigger();
                        golden_freddy_state = GOLDEN_FREDDY_STATE_IN_OFFICE;
                        return;
                }

                return;

        case GOLDEN_FREDDY_STATE_IN_OFFICE:
                /*
                 * Flipping the camera up while he's in the office
                 * removes him for the rest of the night.
                 */
                if (camera_is_visible) {
                        golden_freddy_state =
                                GOLDEN_FREDDY_STATE_NEUTRALIZED;
                }

                golden_freddy_jumpscare_timer += dt;
                if (golden_freddy_jumpscare_timer >=
                    GOLDEN_FREDDY_JUMPSCARE_TIMER_START_SCARING) {
                        sfx_stop_all_channels();
                        wav64_play(&sfx_jumpscare_low, SFX_CH_JUMPSCARE);
                        golden_freddy_state = GOLDEN_FREDDY_STATE_JUMPSCARING;
                        golden_freddy_jumpscare_timer = 0.f;
                }
                return;

        case GOLDEN_FREDDY_STATE_JUMPSCARING:
                golden_freddy_jumpscare_timer += dt;
                if (golden_freddy_jumpscare_timer >=
                        GOLDEN_FREDDY_JUMPSCARE_TIMER_CRASH_GAME) {
                        golden_freddy_state = GOLDEN_FREDDY_STATE_CRASH_GAME;
                }
                return;

        case GOLDEN_FREDDY_STATE_CRASH_GAME:
                {
                        uint8_t *crash;

                        crash = NULL;
                        *crash = 69;
                        /* No return because crash. :3 */
                }
        }
}

void golden_freddy_unload(void)
{
        graphic_unload(&golden_fready_gfx_scare);
        graphic_unload(&golden_freddy_gfx_in_office);
}
