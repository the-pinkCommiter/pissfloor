
#if !defined(AREAMAP_H)
#define AREAMAP_H

#include <PR/ultratypes.h>

#include "types.h"

struct AreaMapData {
    u8 *image_upper;	/* upper half */
    u8 *image_lower;	/* lower half */
    float map_x;    /* map X Position */
    float xo_g;		/* arrow x offset */
    float zo_g;		/* arrow y offset */
    float side_g;	/* map size */
};

Gfx *AreaMap(s32 callContext, struct GraphNode *node, UNUSED Mat4 *c);

#endif /* AREAMAP_H */
