// 0x16000B10
const GeoLayout red_flame_shadow_geo[] = {
    GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x50, 20),
    GEO_OPEN_NODE(),
        GEO_BRANCH_AND_LINK(red_flame_geo),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

// 0x16000B2C
const GeoLayout red_flame_geo[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(8, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3B0),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3C8),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3E0),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3F8),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B410),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B428),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B440),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B458),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

// 0x16000B8C
const GeoLayout yellow_flame_geo[] = {
    GEO_NODE_START(),
    GEO_OPEN_NODE(),
        GEO_SWITCH_CASE(15, geo_switch_anim_state),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow01),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow02),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow03),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow04),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow05),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow06),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow07),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow08),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow09),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow10),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow11),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow12),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow13),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow14),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, RCP_FireC_Yellow15),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
