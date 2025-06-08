// == dialog ==
// (defines en_dialog_table etc.)

#define DEFINE_DIALOG(id, _1, _2, _3, _4, str) \
    static const u8 dialog_text_ ## id[] = { str };

#include "dialogs.h"

#undef DEFINE_DIALOG
#define DEFINE_DIALOG(id, unused, linesPerBox, leftOffset, width, _) \
    static const struct DialogEntry dialog_entry_ ## id = { \
        unused, linesPerBox, leftOffset, width, dialog_text_ ## id \
    };

#include "dialogs.h"

#undef DEFINE_DIALOG
#define DEFINE_DIALOG(id, _1, _2, _3, _4, _5) &dialog_entry_ ## id,

const struct DialogEntry *const seg2_dialog_table[] = {
#include "dialogs.h"
    NULL
};
