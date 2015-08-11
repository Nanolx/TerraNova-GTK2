/* Terranova theme engine
 * Copyright (C) 2005 Richard Stellingwerff
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
 * Modified by Kulyk Nazar <schamane@myeburg.net>
 */

#include <gtk/gtk.h>
#include "terranova_types.h"

typedef struct _terranovaRcStyle terranovaRcStyle;
typedef struct _terranovaRcStyleClass terranovaRcStyleClass;

TN_INTERNAL extern GType terranova_type_rc_style;

#define TERRANOVA_TYPE_RC_STYLE              terranova_type_rc_style
#define TERRANOVA_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TERRANOVA_TYPE_RC_STYLE, terranovaRcStyle))
#define TERRANOVA_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TERRANOVA_TYPE_RC_STYLE, terranovaRcStyleClass))
#define TERRANOVA_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TERRANOVA_TYPE_RC_STYLE))
#define TERRANOVA_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TERRANOVA_TYPE_RC_STYLE))
#define TERRANOVA_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TERRANOVA_TYPE_RC_STYLE, terranovaRcStyleClass))

typedef enum {
	TN_FLAG_STYLE               = 1 << 0,
	TN_FLAG_CONTRAST            = 1 << 1,
	TN_FLAG_MENUBARSTYLE        = 1 << 2,
	TN_FLAG_TOOLBARSTYLE        = 1 << 3,
	TN_FLAG_ANIMATION           = 1 << 4,
	TN_FLAG_RADIUS              = 1 << 5,
	TN_FLAG_HINT                = 1 << 6,
	TN_FLAG_SCROLLBARSTYLE      = 1 << 7,
	TN_FLAG_PROGRESSBARSTYLE       = 1 << 8,
	TN_FLAG_PROGRESSBAR_DIRECTION     = 1 << 9,
	TN_FLAG_RANDOM_EFFECT       = 1 << 10,
	TN_FLAG_RANDOM_OPACITY      = 1 << 11,
	TN_FLAG_MW_GRADIENT         = 1 << 12,
	TN_FLAG_SCALE_TROUGH_SIZE   = 1 << 13,
	TN_FLAG_TOOLTIP_STYLE       = 1 << 14,
	TN_FLAG_GRIP_OPACITY        = 1 << 15,
	TN_FLAG_GRIP_STYLE          = 1 << 16,
	TN_FLAG_TROUGH_VISIBLE      = 1 << 17,
	TN_FLAG_INDICATOR_STYLE     = 1 << 18,
	TN_FLAG_TROUGH_STYLE        = 1 << 19,
	TN_FLAG_BUTTON_EFFECT       = 1 << 20,
	TN_FLAG_BUTTON_GRADIENT     = 1 << 21,
	TN_FLAG_SLIDER_STYLE        = 1 << 22,
	TN_FLAG_GRIP_POS            = 1 << 23,
	TN_FLAG_DRAW_UNSELECTED     = 1 << 24,
	TN_FLAG_MENUITEMSTYLE       = 1 << 25
} terranovaRcFlags;

struct _terranovaRcStyle
{
	GtkRcStyle parent_instance;

	terranovaRcFlags flags;

	terranovaStyles style;

	double contrast;
	guint8 menubarstyle;
	guint8 toolbarstyle;
	gboolean animation;
	double radius;
	GQuark hint;
	guint8 scrollbarstyle;
	guint8 progressbarstyle;
	guint8 progressbar_direction;
	guint8 random_effect;
	double random_opacity;
	guint8 mw_gradient;
	guint8 scale_trough_size;
	guint8 tooltip_style;
	double grip_opacity;
	guint8 grip_style;
	guint8 trough_visible;
	guint8 indicator_style;
	guint8 trough_style;
	guint8 button_effect;
	guint8 button_gradient;
	guint8 slider_style;
	guint8 grip_pos;
	guint8 draw_unselected;
	guint8 menuitemstyle;
};

struct _terranovaRcStyleClass
{
	GtkRcStyleClass parent_class;
};

TN_INTERNAL void terranova_rc_style_register_type (GTypeModule *module);
