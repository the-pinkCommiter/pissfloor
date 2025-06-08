// rolling_log.inc.c

// why are the falling platforms and rolling logs grouped
// together? seems strange, but it also cooresponds to the
// behavior script grouping if the same file is assumed.
// hypothesis is that the object in the middle here used to be
// a rolling log of another variation.

void volcano_act_1(void) {
    o->oRollingLogUnkF4 += 4.0f;
    o->oAngleVelPitch += o->oRollingLogUnkF4;
    o->oFaceAnglePitch -= o->oAngleVelPitch;

    if (o->oFaceAnglePitch < -0x4000) {
        o->oFaceAnglePitch = -0x4000;
        o->oAngleVelPitch = 0;
        o->oRollingLogUnkF4 = 0;
        o->oAction = 2;
        cur_obj_play_sound_2(SOUND_GENERAL_BIG_POUND);
        set_camera_shake_from_point(SHAKE_POS_LARGE, o->oPosX, o->oPosY, o->oPosZ);
    }
}

void volcano_act_3(void) {
    o->oAngleVelPitch = 0x90;
    o->oFaceAnglePitch += o->oAngleVelPitch;

    if (o->oFaceAnglePitch > 0) {
        o->oFaceAnglePitch = 0;
    }

    if (o->oTimer == 200) {
        o->oAction = 0;
    }
}

void bhv_volcano_trap_loop(void) {
    switch (o->oAction) {
        case 0:
            if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 1000)) {
                o->oAction = 1;
                cur_obj_play_sound_2(SOUND_GENERAL_QUIET_POUND2);
            }
            break;

        case 1:
            volcano_act_1();
            break;

        case 2:
            if (o->oTimer < 8) {
                o->oPosY = o->oHomeY + sins(o->oTimer * 0x1000) * 10.0f;
            }
            if (o->oTimer == 50) {
                cur_obj_play_sound_2(SOUND_GENERAL_UNK45);
                o->oAction = 3;
            }
            break;

        case 3:
            volcano_act_3();
            break;
    }
}
