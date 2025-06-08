// sound_birds.inc.c

void bhv_birds_sound_loop(void) {
    if (gCamera->mode == CAMERA_MODE_BEHIND_MARIO) {
        return;
    }

    play_sound(SOUND_OBJ2_BIRD_CHIRP1, gGlobalSoundSource);
}
