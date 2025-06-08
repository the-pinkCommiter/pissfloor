// ground_particles.inc.c

void bhv_pound_white_puffs_init(void) {
    clear_particle_flags(ACTIVE_PARTICLE_MIST_CIRCLE);
    spawn_mist_from_global();
}

static struct SpawnParticlesInfo sGlobalMistParticles = {
    /* bhvParam:        */ 3,
    /* count:           */ 20,
    /* model:           */ MODEL_MIST,
    /* offsetY:         */ 20,
    /* forwardVelBase:  */ 10,
    /* forwardVelRange: */ 5,
    /* velYBase:        */ 0,
    /* velYRange:       */ 0,
    /* gravity:         */ 0,
    /* dragStrength:    */ 30,
    /* sizeBase:        */ 30.0f,
    /* sizeRange:       */ 1.5f,
};

void spawn_mist_from_global(void) {
    cur_obj_spawn_particles(&sGlobalMistParticles);
}

static s16 sSmokeMovementParams[] = { 2, -8, 1, 4 };

void spawn_smoke_with_velocity(void) {
    struct Object *smoke = spawn_object_with_scale(o, MODEL_SMOKE, bhvWhitePuffSmoke2, 1.0f);

    smoke->oForwardVel = sSmokeMovementParams[0];
    smoke->oVelY = sSmokeMovementParams[1];
    smoke->oGravity = sSmokeMovementParams[2];

    obj_translate_xyz_random(smoke, sSmokeMovementParams[3]);
}

// TODO Fix name
void clear_particle_flags(u32 flags) {
    o->parentObj->oActiveParticleFlags &=
        flags ^ 0xFFFFFFFF; // Clear the flags given (could just be ~flags)
}
