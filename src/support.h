#ifndef SUPPORT_H
#define SUPPORT_H

#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#include "terranova_types.h"

#define RADIO_SIZE 13
#define CHECK_SIZE 13

TN_INTERNAL void              terranova_treeview_get_header_index (GtkTreeView  *tv,
                                                 GtkWidget    *header,
                                                 gint         *column_index,
                                                 gint         *columns,
                                                 gboolean     *resizable);

TN_INTERNAL void              terranova_get_parent_bg      (const GtkWidget *widget,
                                                 CairoColor      *color);

TN_INTERNAL terranovaStepper terranova_scrollbar_get_stepper         (GtkWidget       *widget,
                                                 GdkRectangle    *stepper);
TN_INTERNAL terranovaStepper terranova_scrollbar_visible_steppers    (GtkWidget       *widget);
TN_INTERNAL terranovaJunction terranova_scrollbar_get_junction       (GtkWidget    *widget);

TN_INTERNAL void terranova_set_toolbar_parameters (ToolbarParameters *toolbar, GtkWidget *widget, GdkWindow *window, gint x, gint y);
TN_INTERNAL void terranova_get_notebook_tab_position (GtkWidget *widget, gboolean *start, gboolean *end);

#endif /* SUPPORT_H */
