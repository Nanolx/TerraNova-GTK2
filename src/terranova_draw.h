#ifndef TERRANOVA_DRAW_H
#define TERRANOVA_DRAW_H

#include "terranova_types.h"
#include "terranova_style.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <cairo.h>

TN_INTERNAL void terranova_register_style_clarius  (terranovaStyleFunctions *functions);
TN_INTERNAL void terranova_register_style_aquarius (terranovaStyleFunctions *functions);
TN_INTERNAL void terranova_register_style_candy    (terranovaStyleFunctions *functions);

/* Fallback focus function */
TN_INTERNAL void terranova_draw_focus (cairo_t *cr,
                                        const terranovaColors *colors,
                                        const WidgetParameters *params,
                                        const FocusParameters  *focus,
                                        int x, int y, int width, int height);

#endif /* TERRANOVA_DRAW_H */
