#include "engine/graphic.h"
#include "engine/util.h"

#define GFX_GLOBAL_CNT 8

static int gfx_loaded_cnt       = 0;
static int gfx_loaded_cnt_local = 0;

void graphic_load(struct graphic *gfx, const char *path, const bool scale_down)
{
        if (gfx->flags & GFX_IS_LOADED)
                return;
        
        gfx->spr   = sprite_load(path);
        gfx->flags = GFX_IS_LOADED | (scale_down << GFX_SCALE_DOWN_SHIFT);
        
        gfx_loaded_cnt_local = (++gfx_loaded_cnt) - GFX_GLOBAL_CNT;

#ifdef GRAPHIC_DEBUG_ENABLED
        debugf("%d gfx_loaded_cnt (%s)\n", gfx_loaded_cnt_local, path);
#endif
}

void graphics_load(struct graphic *arr, const int cnt,
                   const char **paths, const bool scale_down)
{
        int i;

        for (i = 0; i < cnt; ++i)
                graphic_load(arr + i, paths[i], scale_down);
}

void graphic_unload(struct graphic *gfx)
{
        if (!(gfx->flags & GFX_IS_LOADED))
                return;
        
        sprite_free(gfx->spr);
        gfx->spr = NULL;
        gfx_loaded_cnt_local = (--gfx_loaded_cnt) - GFX_GLOBAL_CNT;
        gfx->flags = 0;
}

void graphics_unload(struct graphic *arr, const int cnt)
{
        int i;

        for (i = 0; i < cnt; ++i)
                graphic_unload(arr + i);
}

void graphic_draw(const struct graphic *gfx, const int px, const int py,
                  const int ox, const int oy, const uint8_t flip_flags)
{
        rdpq_blitparms_t parms;
        int posx, posy, origx, origy;

        posx  = px;
        posy  = py;
        origx = ox;
        origy = oy;

        if (gfx->flags & GFX_SCALE_DOWN) {
                posx  = vcon(posx);
                posy  = vcon(posy);
                origx = vcon(origx);
                origy = vcon(origy);
        }

        parms.tile      = TILE0;
        parms.s0        = 0;
        parms.t0        = 0;
        parms.width     = 0;
        parms.height    = 0;
        parms.flip_x    = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y    = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx        = origx;
        parms.cy        = origy;
        parms.scale_x   = 1.f;
        parms.scale_y   = 1.f;
        parms.theta     = 0.f;
        parms.filtering = false;
        parms.nx        = 0;
        parms.ny        = 0;

        rdpq_sprite_blit(gfx->spr, posx, posy, &parms);
}

void graphic_draw_index_x(const struct graphic *gfx, const int px, const int py,
                          const int w, const int i, const uint8_t flip_flags)
{
        rdpq_blitparms_t parms;
        int posx, posy;

        posx = px;
        posy = py;

        if (gfx->flags & GFX_SCALE_DOWN) {
                posx = vcon(posx);
                posy = vcon(posy);
        }

        parms.tile      = TILE0;
        parms.s0        = w * i;
        parms.t0        = 0;
        parms.width     = w;
        parms.height    = 0;
        parms.flip_x    = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y    = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx        = 0;
        parms.cy        = 0;
        parms.scale_x   = 1.f;
        parms.scale_y   = 1.f;
        parms.theta     = 0.f;
        parms.filtering = false;
        parms.nx        = 0;
        parms.ny        = 0;

        rdpq_sprite_blit(gfx->spr, posx, posy, &parms);
}

void graphic_draw_index_y(const struct graphic *gfx, const int px, const int py,
                          const int h, const int i, const uint8_t flip_flags)
{
        rdpq_blitparms_t parms;
        int posx, posy;

        posx = px;
        posy = py;

        if (gfx->flags & GFX_SCALE_DOWN) {
                posx = vcon(posx);
                posy = vcon(posy);
        }

        parms.tile      = TILE0;
        parms.s0        = 0;
        parms.t0        = h * i;
        parms.width     = 0;
        parms.height    = h;
        parms.flip_x    = (flip_flags & GFX_FLIP_X) >> GFX_FLIP_X_SHIFT;
        parms.flip_y    = (flip_flags & GFX_FLIP_Y) >> GFX_FLIP_Y_SHIFT;
        parms.cx        = 0;
        parms.cy        = 0;
        parms.scale_x   = 1.f;
        parms.scale_y   = 1.f;
        parms.theta     = 0.f;
        parms.filtering = false;
        parms.nx        = 0;
        parms.ny        = 0;

        rdpq_sprite_blit(gfx->spr, posx, posy, &parms);
}
