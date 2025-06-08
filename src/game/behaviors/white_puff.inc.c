// white_puff.inc.c

void bhv_white_puff_1_loop(void) {
    o->oAnimState++;
    cur_obj_scale(3.0f);

    if (o->oTimer > 9)
        o->activeFlags = 0;
}
