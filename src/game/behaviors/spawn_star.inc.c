// spawn_default_star.c.inc

/* TO-DO: clean this up */

static struct ObjectHitbox sCollectStarHitbox = {
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_collect_star_init(void) {
    s8 sp1F;
    u8 sp1E;

    sp1F = (o->oBhvParams >> 24) & 0xFF;
    sp1E = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    if (sp1E & (1 << sp1F)) {
        o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_TRANSPARENT_STAR];
    } else {
        o->header.gfx.sharedChild = gLoadedGraphNodes[MODEL_STAR];
    }

    obj_set_hitbox(o, &sCollectStarHitbox);
}

void bhv_collect_star_loop(void) {
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        mark_obj_for_deletion(o);
        o->oInteractStatus = 0;
    }
}

struct Object *spawn_star(struct Object *sp30, f32 sp34, f32 sp38, f32 sp3C) {
    sp30 = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, sp34, sp38, sp3C, 0, 0, 0);
    sp30->oBhvParams = o->oBhvParams;
    sp30->oFaceAnglePitch = 0;
    sp30->oFaceAngleRoll = 0;
    return sp30;
}

void spawn_default_star(f32 sp20, f32 sp24, f32 sp28) {
    struct Object *sp1C;
    sp1C = spawn_star(sp1C = NULL, sp20, sp24, sp28);
    sp1C->oBhvParams2ndByte = 0;
}
