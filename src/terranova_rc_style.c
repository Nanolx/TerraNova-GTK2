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

#include <string.h>
 #include "widget-information.h"
#include "terranova_style.h"
#include "terranova_rc_style.h"

#include "animation.h"

static void      terranova_rc_style_init         (terranovaRcStyle      *style);
static void      terranova_rc_style_finalize     (GObject                *object);
static void      terranova_rc_style_class_init   (terranovaRcStyleClass *klass);
static GtkStyle *terranova_rc_style_create_style (GtkRcStyle             *rc_style);
static guint     terranova_rc_style_parse        (GtkRcStyle             *rc_style,
                                                   GtkSettings            *settings,
                                                   GScanner               *scanner);
static void      terranova_rc_style_merge        (GtkRcStyle             *dest,
                                                   GtkRcStyle             *src);


static GtkRcStyleClass *terranova_parent_rc_class;

GType terranova_type_rc_style = 0;

enum
{
	TOKEN_CONTRAST = G_TOKEN_LAST + 1,
	TOKEN_SUNKENMENU,
	TOKEN_MENUBARSTYLE,
	TOKEN_TOOLBARSTYLE,
	TOKEN_MENUITEMSTYLE,
	TOKEN_LISTVIEWITEMSTYLE,
	TOKEN_ANIMATION,
	TOKEN_STYLE,
	TOKEN_RADIUS,
	TOKEN_HINT,
	TOKEN_SCROLLBARSTYLE,
	TOKEN_PROGRESSBARSTYLE,
	TOKEN_PROGRESSBAR_DIRECTION,
	TOKEN_RANDOM_EFFECT,
	TOKEN_RANDOM_OPACITY,
	TOKEN_MW_GRADIENT,
	TOKEN_OLD_SCALES,
	TOKEN_SCALE_TROUGH_SIZE,
	TOKEN_TOOLTIP_STYLE,
	TOKEN_GRIP_OPACITY,
	TOKEN_GRIP_STYLE,
	TOKEN_TROUGH_VISIBLE,
	TOKEN_INDICATOR_STYLE,
	TOKEN_TROUGH_STYLE,
	TOKEN_BUTTON_EFFECT,
	TOKEN_BUTTON_GRADIENT,
	TOKEN_SLIDER_STYLE,
	TOKEN_GRIP_POS,
	TOKEN_DRAW_UNSELECTED,

	TOKEN_CLARIUS,
	TOKEN_AQUARIUS,
	TOKEN_CANDY,

	TOKEN_TRUE,
	TOKEN_FALSE,

	TOKEN_LAST
};

static gchar* terranova_rc_symbols =
	"contrast\0"
	"sunkenmenubar\0"
	"menubarstyle\0"
	"toolbarstyle\0"
	"menuitemstyle\0"
	"listviewitemstyle\0"
	"animation\0"
	"style\0"
	"radius\0"
	"hint\0"
	"scrollbarstyle\0"
	"progressbarstyle\0"
	"progressbar_direction\0"
	"random_effect\0"
	"random_opacity\0"
	"mw_gradient\0"
	"old_scales\0"
	"scale_trough_size\0"
	"tooltip_style\0"
	"grip_opacity\0"
	"grip_style\0"
	"trough_visible\0"
	"indicator_style\0"
	"trough_style\0"
	"button_effect\0"
	"button_gradient\0"
	"slider_style\0"
	"grip_pos\0"
	"draw_unselected\0"

	"CLARIUS\0"
	"AQUARIUS\0"
	"CANDY\0"

	"TRUE\0"
	"FALSE\0";

void
terranova_rc_style_register_type (GTypeModule *module)
{
	static const GTypeInfo object_info =
	{
		sizeof (terranovaRcStyleClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) terranova_rc_style_class_init,
		NULL,           /* class_finalize */
		NULL,           /* class_data */
		sizeof (terranovaRcStyle),
		0,              /* n_preallocs */
		(GInstanceInitFunc) terranova_rc_style_init,
		NULL
	};

	terranova_type_rc_style = g_type_module_register_type (module,
	                                                        GTK_TYPE_RC_STYLE,
	                                                        "terranovaRcStyle",
	                                                        &object_info, 0);
}

static void
terranova_rc_style_init (terranovaRcStyle *terranova_rc)
{
	terranova_rc->style = TN_STYLE_CLARIUS;

	terranova_rc->flags = 0;

	terranova_rc->contrast = 1.0;
	terranova_rc->menubarstyle = 0;
	terranova_rc->toolbarstyle = 0;
	terranova_rc->animation = FALSE;
	terranova_rc->radius = 1.2;
	terranova_rc->hint = 0;
	terranova_rc->scrollbarstyle = 0;
	terranova_rc->progressbarstyle = 0;
	terranova_rc->progressbar_direction = 0;
	terranova_rc->random_effect = 0;
	terranova_rc->random_opacity = 0.2;
	terranova_rc->mw_gradient = 0;
	terranova_rc->scale_trough_size = 5;
	terranova_rc->tooltip_style = 0;
	terranova_rc->grip_opacity = 0.5;
	terranova_rc->grip_style = 0;
	terranova_rc->trough_visible = 1;
	terranova_rc->indicator_style = 0;
	terranova_rc->trough_style = 0;
	terranova_rc->button_effect = 0;
	terranova_rc->button_gradient = 0;
	terranova_rc->slider_style = 0;
	terranova_rc->grip_pos = 1;
	terranova_rc->draw_unselected = 0;
	terranova_rc->menuitemstyle = 0;
}

static void
terranova_rc_style_finalize (GObject *object)
{
	/* cleanup all the animation stuff */
	terranova_animation_cleanup ();

	if (G_OBJECT_CLASS (terranova_parent_rc_class)->finalize != NULL)
		G_OBJECT_CLASS (terranova_parent_rc_class)->finalize(object);
}


static void
terranova_rc_style_class_init (terranovaRcStyleClass *klass)
{
	GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS (klass);
	GObjectClass    *g_object_class = G_OBJECT_CLASS (klass);

	terranova_parent_rc_class = g_type_class_peek_parent (klass);

	rc_style_class->parse = terranova_rc_style_parse;
	rc_style_class->create_style = terranova_rc_style_create_style;
	rc_style_class->merge = terranova_rc_style_merge;

	g_object_class->finalize = terranova_rc_style_finalize;
}

static guint
terranova_gtk2_rc_parse_boolean (GtkSettings *settings,
                                  GScanner     *scanner,
                                  gboolean *retval)
{
	guint token;
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token == TOKEN_TRUE)
	   *retval = TRUE;
	else if (token == TOKEN_FALSE)
	   *retval = FALSE;
	else
	   return TOKEN_TRUE;

	return G_TOKEN_NONE;
}

static guint
terranova_gtk2_rc_parse_double (GtkSettings  *settings,
                                 GScanner     *scanner,
                                 gdouble      *val)
{
	guint token;

	/* Skip 'blah' */
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_FLOAT)
	   return G_TOKEN_FLOAT;

	*val = scanner->value.v_float;

	return G_TOKEN_NONE;
}

static guint
terranova_gtk2_rc_parse_int (GtkSettings  *settings,
                              GScanner     *scanner,
                              guint8       *integer)
{
	guint token;

	/* Skip option name */
	token = g_scanner_get_next_token(scanner);

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_INT)
	   return G_TOKEN_INT;

	*integer = scanner->value.v_int;

	return G_TOKEN_NONE;
}

static guint
terranova_gtk2_rc_parse_style (GtkSettings      *settings,
                                GScanner         *scanner,
                                terranovaStyles *style)
{
	guint token;

	/* Skip 'style' */
	token = g_scanner_get_next_token (scanner);

	token = g_scanner_get_next_token (scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
	   return G_TOKEN_EQUAL_SIGN;

	token = g_scanner_get_next_token (scanner);

	switch (token)
	{
		case TOKEN_CLARIUS:
		   *style = TN_STYLE_CLARIUS;
		   break;
		case TOKEN_AQUARIUS:
		   *style = TN_STYLE_AQUARIUS;
		   break;
		case TOKEN_CANDY:
		   *style = TN_STYLE_CANDY;
		   break;
		default:
		   return TOKEN_CLARIUS;
	}

	return G_TOKEN_NONE;
}

static guint
terranova_rc_style_parse (GtkRcStyle *rc_style,
                           GtkSettings  *settings,
                           GScanner   *scanner)
{
	static GQuark scope_id = 0;
	terranovaRcStyle *terranova_style = TERRANOVA_RC_STYLE (rc_style);

	guint old_scope;
	guint token;

	/* Set up a new scope in this scanner. */

	if (!scope_id)
	   scope_id = g_quark_from_string("terranova_theme_engine");

	/* If we bail out due to errors, we *don't* reset the scope, so the
	* error messaging code can make sense of our tokens.
	*/
	old_scope = g_scanner_set_scope(scanner, scope_id);

	/* Now check if we already added our symbols to this scope
	* (in some previous call to terranova_rc_style_parse for the
	* same scanner.
	*/
	if (!g_scanner_lookup_symbol(scanner, terranova_rc_symbols)) {
		gchar *current_symbol = terranova_rc_symbols;
		gint i = G_TOKEN_LAST + 1;

		/* Add our symbols */
		while ((current_symbol[0] != '\0') && (i < TOKEN_LAST)) {
			g_scanner_scope_add_symbol(scanner, scope_id, current_symbol, GINT_TO_POINTER (i));

			current_symbol += strlen(current_symbol) + 1;
			i++;
		}
		g_assert (i == TOKEN_LAST && current_symbol[0] == '\0');
	}

	/* We're ready to go, now parse the top level */

	token = g_scanner_peek_next_token(scanner);
	while (token != G_TOKEN_RIGHT_CURLY)
	{
		switch (token)
		{
			case TOKEN_CONTRAST:
				token = terranova_gtk2_rc_parse_double (settings, scanner, &terranova_style->contrast);
				terranova_style->flags |= TN_FLAG_CONTRAST;
				break;
			case TOKEN_MENUBARSTYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->menubarstyle);
				terranova_style->flags |= TN_FLAG_MENUBARSTYLE;
				break;
			case TOKEN_TOOLBARSTYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->toolbarstyle);
				terranova_style->flags |= TN_FLAG_TOOLBARSTYLE;
				break;
			case TOKEN_ANIMATION:
				token = terranova_gtk2_rc_parse_boolean (settings, scanner, &terranova_style->animation);
				terranova_style->flags |= TN_FLAG_ANIMATION;
				break;
			case TOKEN_STYLE:
				token = terranova_gtk2_rc_parse_style (settings, scanner, &terranova_style->style);
				terranova_style->flags |= TN_FLAG_STYLE;
				break;
			case TOKEN_RADIUS:
				token = terranova_gtk2_rc_parse_double (settings, scanner, &terranova_style->radius);
				terranova_style->flags |= TN_FLAG_RADIUS;
				break;
			case TOKEN_HINT:
				token = tn_rc_parse_hint (scanner, &terranova_style->hint);
				terranova_style->flags |= TN_FLAG_HINT;
				break;
			case TOKEN_SCROLLBARSTYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->scrollbarstyle);
				terranova_style->flags |= TN_FLAG_SCROLLBARSTYLE;
				break;
			case TOKEN_PROGRESSBARSTYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->progressbarstyle);
				terranova_style->flags |= TN_FLAG_PROGRESSBARSTYLE;
				break;
			case TOKEN_PROGRESSBAR_DIRECTION:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->progressbar_direction);
				terranova_style->flags |= TN_FLAG_PROGRESSBAR_DIRECTION;
				break;
			case TOKEN_RANDOM_EFFECT:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->random_effect);
				terranova_style->flags |= TN_FLAG_RANDOM_EFFECT;
				break;
			case TOKEN_RANDOM_OPACITY:
				token = terranova_gtk2_rc_parse_double (settings, scanner, &terranova_style->random_opacity);
				terranova_style->flags |= TN_FLAG_RANDOM_OPACITY;
				break;
			case TOKEN_MW_GRADIENT:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->mw_gradient);
				terranova_style->flags |= TN_FLAG_MW_GRADIENT;
				break;
			case TOKEN_SCALE_TROUGH_SIZE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->scale_trough_size);
				terranova_style->flags |= TN_FLAG_SCALE_TROUGH_SIZE;
				break;
			case TOKEN_TOOLTIP_STYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->tooltip_style);
				terranova_style->flags |= TN_FLAG_TOOLTIP_STYLE;
				break;
			case TOKEN_GRIP_OPACITY:
				token = terranova_gtk2_rc_parse_double (settings, scanner, &terranova_style->grip_opacity);
				terranova_style->flags |= TN_FLAG_GRIP_OPACITY;
				break;
			case TOKEN_GRIP_STYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->grip_style);
				terranova_style->flags |= TN_FLAG_GRIP_STYLE;
				break;
			case TOKEN_TROUGH_VISIBLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->trough_visible);
				terranova_style->flags |= TN_FLAG_TROUGH_VISIBLE;
				break;
			case TOKEN_INDICATOR_STYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->indicator_style);
				terranova_style->flags |= TN_FLAG_INDICATOR_STYLE;
				break;
			case TOKEN_TROUGH_STYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->trough_style);
				terranova_style->flags |= TN_FLAG_TROUGH_STYLE;
				break;
			case TOKEN_BUTTON_EFFECT:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->button_effect);
				terranova_style->flags |= TN_FLAG_BUTTON_EFFECT;
				break;
			case TOKEN_BUTTON_GRADIENT:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->button_gradient);
				terranova_style->flags |= TN_FLAG_BUTTON_GRADIENT;
				break;
			case TOKEN_SLIDER_STYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->slider_style);
				terranova_style->flags |= TN_FLAG_SLIDER_STYLE;
				break;
			case TOKEN_GRIP_POS:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->grip_pos);
				terranova_style->flags |= TN_FLAG_GRIP_POS;
				break;
			case TOKEN_DRAW_UNSELECTED:
				token = terranova_gtk2_rc_parse_int (settings, scanner, & terranova_style->draw_unselected);
				terranova_style->flags |= TN_FLAG_DRAW_UNSELECTED;
				break;
			case TOKEN_MENUITEMSTYLE:
				token = terranova_gtk2_rc_parse_int (settings, scanner, &terranova_style->menuitemstyle);
				terranova_style->flags |= TN_FLAG_MENUITEMSTYLE;
				break;
			default:
				g_scanner_get_next_token(scanner);
				token = G_TOKEN_RIGHT_CURLY;
				break;
		}

		if (token != G_TOKEN_NONE)
			return token;

		token = g_scanner_peek_next_token(scanner);
	}

	g_scanner_get_next_token(scanner);

	g_scanner_set_scope(scanner, old_scope);

	return G_TOKEN_NONE;
}

static void
terranova_rc_style_merge (GtkRcStyle *dest,
                           GtkRcStyle *src)
{
	terranovaRcStyle *dest_w, *src_w;
	terranovaRcFlags flags;

	terranova_parent_rc_class->merge (dest, src);

	if (!TERRANOVA_IS_RC_STYLE (src))
		return;

	src_w = TERRANOVA_RC_STYLE (src);
	dest_w = TERRANOVA_RC_STYLE (dest);

	flags = (~dest_w->flags) & src_w->flags;

	if (flags & TN_FLAG_STYLE)
		dest_w->style = src_w->style;
	if (flags & TN_FLAG_CONTRAST)
		dest_w->contrast = src_w->contrast;
	if (flags & TN_FLAG_MENUBARSTYLE)
		dest_w->menubarstyle = src_w->menubarstyle;
	if (flags & TN_FLAG_TOOLBARSTYLE)
		dest_w->toolbarstyle = src_w->toolbarstyle;
	if (flags & TN_FLAG_ANIMATION)
		dest_w->animation = src_w->animation;
	if (flags & TN_FLAG_RADIUS)
		dest_w->radius = src_w->radius;
	if (flags & TN_FLAG_HINT)
		dest_w->hint = src_w->hint;
	if (flags & TN_FLAG_SCROLLBARSTYLE)
		dest_w->scrollbarstyle = src_w->scrollbarstyle;
	if (flags & TN_FLAG_PROGRESSBARSTYLE)
		dest_w->progressbarstyle = src_w->progressbarstyle;
	if (flags & TN_FLAG_PROGRESSBAR_DIRECTION)
		dest_w->progressbar_direction = src_w->progressbar_direction;
	if (flags & TN_FLAG_RANDOM_EFFECT)
		dest_w->random_effect = src_w->random_effect;
	if (flags & TN_FLAG_RANDOM_OPACITY)
		dest_w->random_opacity = src_w->random_opacity;
	if (flags & TN_FLAG_MW_GRADIENT)
		dest_w->mw_gradient = src_w->mw_gradient;
	if (flags & TN_FLAG_SCALE_TROUGH_SIZE)
		dest_w->scale_trough_size = src_w->scale_trough_size;
	if (flags & TN_FLAG_TOOLTIP_STYLE)
		dest_w->tooltip_style = src_w->tooltip_style;
	if (flags & TN_FLAG_GRIP_OPACITY)
		dest_w->grip_opacity = src_w->grip_opacity;
	if (flags & TN_FLAG_GRIP_STYLE)
		dest_w->grip_style = src_w->grip_style;
	if (flags & TN_FLAG_TROUGH_VISIBLE)
		dest_w->trough_visible = src_w->trough_visible;
	if (flags & TN_FLAG_INDICATOR_STYLE)
		dest_w->indicator_style = src_w->indicator_style;
	if (flags & TN_FLAG_TROUGH_STYLE)
		dest_w->trough_style = src_w->trough_style;
	if (flags & TN_FLAG_BUTTON_EFFECT)
		dest_w->button_effect = src_w->button_effect;
	if (flags & TN_FLAG_BUTTON_GRADIENT)
		dest_w->button_gradient = src_w->button_gradient;
	if (flags & TN_FLAG_SLIDER_STYLE)
		dest_w->slider_style = src_w->slider_style;
	if (flags & TN_FLAG_GRIP_POS)
		dest_w->grip_pos = src_w->grip_pos;
	if (flags & TN_FLAG_DRAW_UNSELECTED)
		dest_w->draw_unselected = src_w->draw_unselected;
	if (flags & TN_FLAG_MENUITEMSTYLE)
		dest_w->menuitemstyle = src_w->menuitemstyle;

	dest_w->flags |= src_w->flags;
}


/* Create an empty style suitable to this RC style
 */
static GtkStyle *
terranova_rc_style_create_style (GtkRcStyle *rc_style)
{
	return GTK_STYLE (g_object_new (TERRANOVA_TYPE_STYLE, NULL));
}
