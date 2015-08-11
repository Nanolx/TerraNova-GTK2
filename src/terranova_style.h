/* Terranova theme engine
 * Copyright (C) 2005 Richard Stellingwerff.
 * Copyright (C) 2007 Benjamin Berg
 * Copyright (C) 2007 Andrea Cimitan
 * Copyright (C) 2008 Christopher Bratusek
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
 *
 * Written by Owen Taylor <otaylor@redhat.com>
 * and by Alexander Larsson <alexl@redhat.com>
 * Modified by Richard Stellingwerff <remenic@gmail.com>
 */

#include <gtk/gtk.h>

#ifndef TERRANOVA_STYLE_H
#define TERRANOVA_STYLE_H

#include "animation.h"
#include "terranova_types.h"

typedef struct _terranovaStyle terranovaStyle;
typedef struct _terranovaStyleClass terranovaStyleClass;

TN_INTERNAL extern GType terranova_type_style;

#define TERRANOVA_TYPE_STYLE              terranova_type_style
#define TERRANOVA_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TERRANOVA_TYPE_STYLE, terranovaStyle))
#define TERRANOVA_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TERRANOVA_TYPE_STYLE, terranovaStyleClass))
#define TERRANOVA_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TERRANOVA_TYPE_STYLE))
#define TERRANOVA_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TERRANOVA_TYPE_STYLE))
#define TERRANOVA_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TERRANOVA_TYPE_STYLE, terranovaStyleClass))

struct _terranovaStyle
{
	GtkStyle parent_instance;

	terranovaColors colors;

	terranovaStyles style;

	guint8   menubarstyle;
	guint8   toolbarstyle;
	guint8   scrollbarstyle;
	guint8   progressbarstyle;
	guint8   progressbar_direction;
	guint8   random_effect;
	gboolean animation;
	gfloat   radius;
	gfloat   random_opacity;
	guint8   mw_gradient;
	guint8   scale_trough_size;
	guint8   tooltip_style;
	gfloat   grip_opacity;
	guint8   grip_style;
	guint8   trough_visible;
	guint8   indicator_style;
	guint8   trough_style;
	guint8   button_effect;
	guint8   button_gradient;
	guint8   slider_style;
	guint8   grip_pos;
	guint8   draw_unselected;
	guint8   menuitemstyle;
};

struct _terranovaStyleClass
{
	GtkStyleClass parent_class;

	terranovaStyleFunctions style_functions[TN_NUM_STYLES];
};

TN_INTERNAL void terranova_style_register_type (GTypeModule *module);

#endif /* TERRANOVA_STYLE_H */
