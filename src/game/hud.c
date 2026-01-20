#include <PR/ultratypes.h>

#include "sm64.h"
#include "actors/common1.h"
#include "gfx_dimensions.h"
#include "game_init.h"
#include "level_update.h"
#include "camera.h"
#include "print.h"
#include "ingame_menu.h"
#include "hud.h"
#include "segment2.h"
#include "area.h"
#include "save_file.h"
#include "print.h"

/* @file hud.c
 * This file implements HUD rendering and power meter animations.
 * That includes stars, lives, coins, power meter, timer
 * cannon reticle, and the unused keys.
 **/

char * digits;

struct PowerMeterHUD {
    s8 animation;
    s16 x;
    s16 y;
    f32 unused;
};

// Stores health segmented value defined by numHealthWedges
// When the HUD is rendered this value is 8, full health.
static s16 sPowerMeterStoredHealth;

static struct PowerMeterHUD sPowerMeterHUD = {
    POWER_METER_HIDDEN,
    140,
    166,
    1.0,
};

// Power Meter timer that keeps counting when it's visible.
// Gets reset when the health is filled and stops counting
// when the power meter is hidden.
s32 sPowerMeterVisibleTimer = 0;

UNUSED static s32 sUnusedHUDValue1 = 0;
UNUSED static s16 sUnusedHUDValue2 = 10;

/**
 * Renders a rgba16 16x16 glyph texture from a table list.
 */
void render_hud_tex_lut(s32 x, s32 y, u8 *texture) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPDisplayList(gDisplayListHead++, &dl_hud_img_load_tex_block);
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 15) << 2, (y + 15) << 2,
                        G_TX_RENDERTILE, 0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders a rgba16 8x8 glyph texture from a table list.
 */
void render_hud_small_tex_lut(s32 x, s32 y, u8 *texture) {
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0,
               G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR,
               G_TX_NOMASK, G_TX_NOLOD);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 3,
               G_TX_NOLOD, G_TX_CLAMP, 3, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, (8 - 1) << G_TEXTURE_IMAGE_FRAC,
                   (8 - 1) << G_TEXTURE_IMAGE_FRAC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 8 * 8 - 1, CALC_DXT(8, G_IM_SIZ_16b_BYTES));
    gSPTextureRectangle(gDisplayListHead++, x << 2, y << 2, (x + 7) << 2, (y + 7) << 2, G_TX_RENDERTILE,
                        0, 0, 4 << 10, 1 << 10);
}

/**
 * Renders power meter health segment texture using a table list.
 */
void render_power_meter_health_segment(s16 numHealthWedges) {
    u8 *(*healthLUT)[];

    healthLUT = segmented_to_virtual(power_meter_health_segments_lut);

    gDPPipeSync(gDisplayListHead++);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[numHealthWedges * 2], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 0, 2, 3, 0);
    g_Tani_LoadTextureImage2(gDisplayListHead++, (*healthLUT)[(numHealthWedges * 2) + 1], G_IM_FMT_RGBA,
                             G_IM_SIZ_16b, 32, 64, 0, 7) gSP1Triangle(gDisplayListHead++, 4, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 4, 6, 7, 0);
}

/**
 * Renders power meter display lists.
 * That includes the "POWER" base and the colored health segment textures.
 */
void render_dl_power_meter(s16 numHealthWedges) {
    Mtx *translateMtx;
    Mtx *scaleMtx;

    translateMtx = alloc_display_list(sizeof(Mtx));
    scaleMtx = alloc_display_list(sizeof(Mtx));

    if (translateMtx == NULL || scaleMtx == NULL) {
        return;
    }

    guTranslate(translateMtx, (f32) sPowerMeterHUD.x, (f32) sPowerMeterHUD.y, 0);
    guScale(scaleMtx, 1.0f, 1.0f, 1.0f);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(translateMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(scaleMtx++),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(gDisplayListHead++, &dl_power_meter_base);

    render_power_meter_health_segment(numHealthWedges);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

/**
 * Power meter animation called when there's less than 8 health segments
 * Checks its timer to later change into deemphasizing mode.
 */
void animate_power_meter_emphasized(void) {
    s16 hudDisplayFlags = gHudDisplay.flags;

    if (!(hudDisplayFlags & HUD_DISPLAY_FLAG_EMPHASIZE_POWER)) {
        if (sPowerMeterVisibleTimer == 45.0) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
        }
    } else {
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Power meter animation called after emphasized mode.
 * Moves power meter y pos speed until it's at 200 to be visible.
 */
static void animate_power_meter_deemphasizing(void) {
    s16 speed = 5;

    if (sPowerMeterHUD.y > 180) {
        speed = 3;
    }

    if (sPowerMeterHUD.y > 190) {
        speed = 2;
    }

    if (sPowerMeterHUD.y > 195) {
        speed = 1;
    }

    sPowerMeterHUD.y += speed;

    if (sPowerMeterHUD.y > 200) {
        sPowerMeterHUD.y = 200;
        sPowerMeterHUD.animation = POWER_METER_VISIBLE;
    }
}

/**
 * Power meter animation called when there's 8 health segments.
 * Moves power meter y pos quickly until it's at 301 to be hidden.
 */
static void animate_power_meter_hiding(void) {
    sPowerMeterHUD.y += 20;
    if (sPowerMeterHUD.y > 300) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Handles power meter actions depending of the health segments values.
 */
void handle_power_meter_actions(s16 numHealthWedges) {
    // Show power meter if health is not full, less than 8
    if (numHealthWedges < 8 && sPowerMeterStoredHealth == 8
        && sPowerMeterHUD.animation == POWER_METER_HIDDEN) {
        sPowerMeterHUD.animation = POWER_METER_EMPHASIZED;
        sPowerMeterHUD.y = 166;
    }

    // Show power meter if health is full, has 8
    if (numHealthWedges == 8 && sPowerMeterStoredHealth == 7) {
        sPowerMeterVisibleTimer = 0;
    }

    // After health is full, hide power meter
    if (numHealthWedges == 8 && sPowerMeterVisibleTimer > 45.0) {
        sPowerMeterHUD.animation = POWER_METER_HIDING;
    }

    // Update to match health value
    sPowerMeterStoredHealth = numHealthWedges;

    // If Mario is swimming, keep power meter visible
    if (gPlayerCameraState->action & ACT_FLAG_SWIMMING) {
        if (sPowerMeterHUD.animation == POWER_METER_HIDDEN
            || sPowerMeterHUD.animation == POWER_METER_EMPHASIZED) {
            sPowerMeterHUD.animation = POWER_METER_DEEMPHASIZING;
            sPowerMeterHUD.y = 166;
        }
        sPowerMeterVisibleTimer = 0;
    }
}

/**
 * Renders the power meter that shows when Mario is in underwater
 * or has taken damage and has less than 8 health segments.
 * And calls a power meter animation function depending of the value defined.
 */
void render_hud_power_meter(void) {
    s16 shownHealthWedges = gHudDisplay.wedges;

    if (sPowerMeterHUD.animation != POWER_METER_HIDING) {
        handle_power_meter_actions(shownHealthWedges);
    }

    if (sPowerMeterHUD.animation == POWER_METER_HIDDEN) {
        return;
    }

    switch (sPowerMeterHUD.animation) {
        case POWER_METER_EMPHASIZED:
            animate_power_meter_emphasized();
            break;
        case POWER_METER_DEEMPHASIZING:
            animate_power_meter_deemphasizing();
            break;
        case POWER_METER_HIDING:
            animate_power_meter_hiding();
            break;
        default:
            break;
    }

    render_dl_power_meter(shownHealthWedges);

    sPowerMeterVisibleTimer++;
}

#define HUD_TOP_Y 210

/*
note:
castle: invisible hud
wf: inverted hud
lll: final hud
ccm: shosh
ddd: n/a
bowser: n/a
*/

void render_digits(void) {
    if (gCurrLevelNum == LEVEL_LLL) {
        digits = "%d";
    } else {
        digits = "%02d";
    }
}

/**
 * Renders the amount of lives Mario has.
 */
void render_hud_mario_lives(void) {
    render_digits();
    
    print_text(22, HUD_TOP_Y, ","); // 'Mario Head' glyph
    print_text(38, HUD_TOP_Y, "*"); // 'X' glyph
    print_text_fmt_int(54, HUD_TOP_Y, digits, gHudDisplay.lives);
}

/**
 * Renders the amount of coins collected.
 */
void render_hud_coins(void) {
    // x positions
    s16 coin_icon;
    s16 coin_x;
    s16 coin_disp;

    // y position
    s16 coin_y;

    render_digits();
    
    if (gCurrLevelNum == LEVEL_WF) {
        coin_icon = 242;
        coin_x = 258;
        coin_disp = 271;
        digits = "%02d";
    } else if (gCurrLevelNum == LEVEL_LLL | gCurrLevelNum == LEVEL_CCM) {
        coin_icon = 170;
        coin_x = 187;
        coin_disp = 200;
    } else if (gCurrLevelNum == COURSE_NONE) {
        coin_icon = 22;
        coin_x = 38;
        coin_disp = 54;
        coin_y = 193;
    } else {
        coin_icon = 242;
        coin_x = 258;
        coin_disp = 271;
    }

    if (gCurrLevelNum == LEVEL_CCM) {
        coin_y = 193;
    } else {
        coin_y = 210;
    }
    
    print_text(coin_icon, coin_y, "+"); // 'Coin' glyph
    print_text(coin_x, coin_y, "*"); // 'X' glyph
    print_text_fmt_int(coin_disp, coin_y, digits, gHudDisplay.coins);
}

/**
 * Renders the amount of stars collected.
 * Disables "X" glyph when Mario has 100 stars or more.
 */
void render_hud_stars(void) {
    s16 star_icon;
    s16 star_x;
    s16 star_disp;
    s16 star_final_pos = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(73);

    render_digits();
    
    if (gCurrLevelNum == LEVEL_WF | gCurrLevelNum == LEVEL_CCM) {
        star_icon = 170;
        star_x = 186;
        star_disp = 200;
    } else if (gCurrLevelNum == LEVEL_LLL) {
        star_icon = star_final_pos;
        star_x = star_final_pos + 16;
        star_disp = star_final_pos + 32;
    } else {
        star_icon = 171;
        star_x = 187;
        star_disp = 201;
    }

    print_text(star_icon, 210, "-"); // 'Star' glyph
    print_text(star_x, 210, "*"); // 'X' glyph
    print_text_fmt_int(star_disp, 210, digits, gHudDisplay.stars);
}


/**
 * Render HUD strings using hudDisplayFlags with it's render functions,
 * excluding the cannon reticle which detects a camera preset for it.
 */
void render_hud(void) {
    s16 hudDisplayFlags = gHudDisplay.flags;

    if (hudDisplayFlags == HUD_DISPLAY_NONE) {
        sPowerMeterHUD.animation = POWER_METER_HIDDEN;
        sPowerMeterStoredHealth = 8;
        sPowerMeterVisibleTimer = 0;
    } else {
        create_dl_ortho_matrix();

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_LIVES) {
            render_hud_mario_lives();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_COIN_COUNT | gCurrLevelNum != LEVEL_CASTLE_GROUNDS) {
            render_hud_coins();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_STAR_COUNT | gCurrLevelNum >= LEVEL_MIN) {
            render_hud_stars();
        }

        if (hudDisplayFlags & HUD_DISPLAY_FLAG_POWER) {
            render_hud_power_meter();
        }
    }
}
