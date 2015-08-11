/* Terranova Engine
 *
 * Copyright (C) 2006 Kulyk Nazar <schamane@myeburg.net>
 * Copyright (C) 2006 Benjamin Berg <benjamin@sipsolutions.net>
 * Copyright (C) 2008 Christopher Bratusek <nano-master@gmx.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <gtk/gtk.h>
#include "ge-support.h"

#define TN_IS_ANI_PROGRESS_BAR(widget) TN_IS_PROGRESS_BAR(widget) && widget->allocation.x != -1 && widget->allocation.y != -1
#define ANIMATION_DELAY 100
#define CHECK_ANIMATION_TIME 0.5

TN_INTERNAL void     terranova_animation_progressbar_add (GtkWidget *progressbar);
TN_INTERNAL void     terranova_animation_connect_checkbox (GtkWidget *widget);
TN_INTERNAL gboolean terranova_animation_is_animated (GtkWidget *widget);
TN_INTERNAL gdouble  terranova_animation_elapsed (gpointer data);
TN_INTERNAL void     terranova_animation_cleanup ();
