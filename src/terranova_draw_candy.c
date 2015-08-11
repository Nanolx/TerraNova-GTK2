/* Terranova Engine
 * Copyright (C) 2006 Richard Stellingwerff
 * Copyright (C) 2006 Daniel Borgman
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
 */

#include "terranova_draw.h"
#include "terranova_style.h"
#include "terranova_types.h"
#include "terranova_functions.h"

#include "support.h"
#include "ge-support.h"
#include <math.h>

#define M_PI 3.14159265358979323846

#include <cairo.h>

static void
terranova_draw_candy_button (cairo_t *cr,
                        const terranovaColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	double xoffset = 0, yoffset = 0;
	double radius = params->radius;

	CairoColor fill = colors->bg[params->state_type];
	CairoColor border_normal, shadow, border_disabled;

	tn_shade_color(&colors->shade[6], 1.00, &border_normal);
	tn_shade_color(&border_normal, 0.85, &shadow);
	tn_shade_color(&colors->shade[4], 0.80, &border_disabled);

	radius = MIN (radius, MIN ((width-2.0 - xoffset*2.0)/2.0, (height-2.0 - yoffset*2)/2.0));

	cairo_save (cr);

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	if (params->xthickness == 3 || params->ythickness == 3)
	{
		if (params->xthickness == 3)
			xoffset = 1;
		if (params->ythickness == 3)
			yoffset = 1;
	}

	GradientPositions gradient_position;
	gradient_position = terranova_get_gradient_position(params, width, height);

	tn_cairo_rounded_rectangle (cr, xoffset+2, yoffset+2,
						width-(xoffset*2)-2,
						height-(yoffset*2)-2,
						radius, params->corners);

	if (params->disabled)
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2;
		tn_shade_color (&fill, 0.87, &shade1);
		tn_shade_color (&fill, 0.95, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}
	else if (params->prelight)
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2;
		tn_shade_color (&fill, 0.90, &shade1);
		tn_shade_color (&fill, 1.15, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);

		tn_cairo_set_color(cr, &colors->bg[GTK_STATE_SELECTED]);
		tn_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, radius, params->corners);
		cairo_stroke (cr);
	}
	else if (params->active)
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2;
		tn_shade_color (&fill, 0.95, &shade1);
		tn_shade_color (&fill, 1.20, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);

		tn_cairo_set_color(cr, &colors->bg[GTK_STATE_SELECTED]);
		tn_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, radius, params->corners);
		cairo_stroke (cr);
	}
	else
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2;
		tn_shade_color (&fill, 0.97, &shade1);
		tn_shade_color (&fill, 1.10, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}

	terranova_draw_button_effect(cr, params, colors, width, height);

	if (params->disabled)
			tn_cairo_set_color (cr, &border_disabled);
	else
		if (!params->active)
			terranova_draw_border_gradient (cr, &border_normal, 1.32, 0, height);
		else
			tn_cairo_set_color (cr, &border_normal);

	tn_cairo_rounded_rectangle (cr, xoffset + 0.5, yoffset + 0.5,
					width-(xoffset*2)-1, height-(yoffset*2)-1,
					radius, params->corners);
	cairo_stroke (cr);

	terranova_draw_button_grip(cr, params, colors, width, height, xoffset, yoffset, radius);

	if (params->random_effect == 0)
		cairo_restore (cr);
}

static void
terranova_draw_candy_entry (cairo_t *cr,
                       const terranovaColors *colors,
                       const WidgetParameters *params,
                       int x, int y, int width, int height)
{
	CairoColor border = colors->shade[params->disabled ? 4 : 6];
	const CairoColor *fill = &colors->bg[params->state_type];
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	if (params->focus)
		border = colors->spot[2];

	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 1.0);

	cairo_pattern_t *pattern;

	if (params->disabled)
	{
		CairoColor shade1, shade2;
		tn_shade_color (fill, 0.87, &shade1);
		tn_shade_color (fill, 0.95, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}
	else
	{
		CairoColor shade1, shade2;
		tn_shade_color (fill, 0.95, &shade1);
		tn_shade_color (fill, 1.20, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}

	cairo_fill(cr);

	/* Draw the inner shadow */
	if (params->focus)
	{
		tn_cairo_set_color (cr, &colors->spot[0]);
		tn_cairo_stroke_rectangle (cr, 2, 2, width-5, height-4);
	}

	if (params->enable_shadow)
	{
		tn_cairo_rounded_rectangle (cr, 1.5, 1.5, width-3, height-3, radius, params->corners);
		terranova_draw_border_gradient (cr, &border, 1.32, -1, height-1);
		cairo_stroke (cr);
	}
	else
	{
		cairo_set_source_rgba (cr, border.r, border.g, border.b, 0.00);
		cairo_stroke (cr);
	}
}

static void
terranova_draw_candy_icon_view_item (cairo_t *cr,
                       const terranovaColors *colors,
                       const WidgetParameters *params,
                       int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];

	cairo_save (cr);

	cairo_translate (cr, x, y);

	cairo_pattern_t *pattern;

	CairoColor shade1, shade2;
	tn_shade_color (fill, 0.94, &shade1);
	tn_shade_color (fill, 1.00, &shade2);

	pattern	= cairo_pattern_create_linear (0, 1, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);

	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle  (cr, 0, 0, width, height, params->radius, CR_CORNER_ALL);

	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	cairo_restore (cr);
}

static void
terranova_draw_candy_list_view_header (cairo_t *cr,
                                  const terranovaColors          *colors,
                                  const WidgetParameters          *params,
                                  const ListViewHeaderParameters  *header,
                                  int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_NORMAL];
	cairo_pattern_t *pattern;

	cairo_translate (cr, x, y);

	CairoColor shade1, shade2;
	tn_shade_color (fill, 0.96, &shade1);
	tn_shade_color (fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
	cairo_set_source (cr, pattern);
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

}

static void
terranova_draw_candy_menubaritem (cairo_t *cr,
                          const terranovaColors          *colors,
                          const WidgetParameters *params,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	tn_shade_color (&border, 1.00, &border);

	CairoColor shade1, shade2;
	tn_shade_color (fill, 0.96, &shade1);
	tn_shade_color (fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);

	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle (cr, x+0.75, y+0.75, width - 1, height - 1, params->radius, params->corners);
	cairo_fill_preserve (cr);
	cairo_pattern_destroy (pattern);

	tn_cairo_set_color (cr, &border);
	cairo_stroke_preserve (cr);
}

static void
terranova_draw_candy_menuitem (cairo_t *cr,
                          const terranovaColors          *colors,
                          const WidgetParameters *params,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	tn_shade_color (&border, 1.05, &border);
	cairo_set_line_width (cr, 1.0);

	CairoColor shade1, shade2;
	tn_shade_color (fill, 0.96, &shade1);
	tn_shade_color (fill, 1.10, &shade2);

	cairo_translate (cr, x, y);
	tn_cairo_rounded_rectangle (cr, 0, 0, width, height, params->radius, params->corners);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);

	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
	cairo_save (cr);

	if (params->menuitemstyle == 1)
	{
		terranova_draw_fill_diagonal_stripes(cr, colors, params, width, height, 1);
	}
	else if (params->menuitemstyle == 2)
	{
		terranova_draw_fill_arrows(cr, colors, params, width, height, 1);
	}
	else if (params->menuitemstyle == 3)
	{
		terranova_draw_fill_vertical_stripes(cr, colors, params, width, height, 1);
	}


	tn_cairo_set_color (cr, &border);
	cairo_stroke_preserve (cr);
}

static void
terranova_draw_candy_progressbar_fill (cairo_t *cr,
                                  const terranovaColors *colors,
                                  const WidgetParameters *params,
                                  const ProgressBarParameters *progressbar,
                                  int x, int y, int width, int height,
                                  gint offset)
{

	boolean          is_horizontal = progressbar->orientation < 2;
	double           tile_pos = 0;
	double           stroke_width;
	int		 x_step;
	double           arrow_width;
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	cairo_pattern_t *pattern;
	CairoColor fill = colors->bg[GTK_STATE_SELECTED];

	cairo_rectangle (cr, x, y, width, height);
	cairo_clip (cr);
	cairo_new_path (cr);

	if (is_horizontal)
	{
		if (progressbar->orientation == TN_ORIENTATION_LEFT_TO_RIGHT)
			terranova_mirror_rotate (cr, 0, x, y, FALSE, FALSE);
		else
			terranova_mirror_rotate (cr, 0, x+width, y, TRUE, FALSE);
	}
	else
	{
		int tmp = height;
		height  = width;
		width   = tmp;

		if (progressbar->orientation == TN_ORIENTATION_TOP_TO_BOTTOM)
			terranova_mirror_rotate (cr, G_PI/2, x, y, FALSE, FALSE);
		else
			terranova_mirror_rotate (cr, G_PI/2, x, y+width, TRUE, FALSE);
	}

	stroke_width = height*1.25;

	x_step = (((float)stroke_width/10)*offset);

	arrow_width = stroke_width/2;

	tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);

	/* Draw fill */
	cairo_set_source_rgb (cr, colors->spot[1].r,
	                          colors->spot[1].g,
	                          colors->spot[1].b);
	cairo_fill_preserve (cr);

	CairoColor shade1, shade2;
	tn_shade_color (&fill, 0.96, &shade1);
	tn_shade_color (&fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (0, 1, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle (cr, 0, 0, width - 1, height - 1, params->radius, params->corners);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	if (progressbar->progressbarstyle == 0)
	{
		terranova_draw_fill_diagonal_stripes(cr, colors, params, width, height, offset);
	}
	else if (progressbar->progressbarstyle == 1)
	{
		terranova_draw_fill_arrows(cr, colors, params, width, height, offset);
	}
	else if (progressbar->progressbarstyle == 2)
	{
		terranova_draw_fill_vertical_stripes(cr, colors, params, width, height, offset);
	}


	/* Left shadow */
	cairo_translate (cr, width, 0);
	pattern = cairo_pattern_create_linear (0, 0, 3, 0);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0., 0., 0., 0.1);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 0., 0., 0., 0.);
	tn_cairo_rounded_rectangle (cr, 0, 0, 3, height, radius, params->corners);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	/* End of bar line */
	cairo_move_to (cr, -0.5, 0);
	cairo_line_to (cr, -0.5, 0);
	cairo_set_source_rgba (cr, colors->spot[2].r,
	                           colors->spot[2].g,
	                           colors->spot[2].b,
						0.5);
	cairo_stroke (cr);

}

static void
terranova_draw_candy_progressbar_trough (cairo_t *cr,
                                    const terranovaColors *colors,
                                    const WidgetParameters *params,
                                    int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[6];
	cairo_pattern_t *pattern;
	const CairoColor *fill   = &colors->bg[params->state_type];
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	cairo_save (cr);

	cairo_set_line_width (cr, 1.0);

	/* Create trough box */
	tn_cairo_rounded_rectangle (cr, x+1, y+1, width-2, height-2, radius, params->corners);
	tn_cairo_set_color (cr, &colors->shade[1]);
	cairo_fill (cr);

	/* Draw border */
	tn_cairo_rounded_rectangle (cr, x+0.5, y+0.5, width-1, height-1, radius, params->corners);
	tn_cairo_set_color (cr, border);
	cairo_stroke (cr);

	/* clip the corners of the shadows */
	tn_cairo_rounded_rectangle (cr, x+1, y+1, width-2, height-2, radius, params->corners);
	cairo_clip (cr);

	if (height > width)
	{
		int tmp = height;
		height  = width;
		width   = tmp;

		terranova_mirror_rotate (cr, G_PI/2, x, y, FALSE, FALSE);
	}

	CairoColor shade1, shade2;
	tn_shade_color (fill, 0.96, &shade1);
	tn_shade_color (fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (x, y + 1, x, y + height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);


	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle (cr, 0, 0, width - 1, height - 1, params->radius, params->corners);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

}

static void
terranova_draw_candy_scrollbar_slider (cairo_t *cr,
                                   const terranovaColors          *colors,
                                   const WidgetParameters *params,
                                   const ScrollBarParameters       *scrollbar,
                                   int x, int y, int width, int height)
{

	CairoColor fill = colors->bg[GTK_STATE_SELECTED];

	CairoColor hilight;

	double tile_pos = 0;
	double stroke_width;
	int x_step;
	int offset;
	double arrow_width;
	CairoColor shading;

	if (scrollbar->horizontal)
	{
		cairo_translate (cr, x, y);
	}
	else
	{
		int tmp = height;
		terranova_mirror_rotate (cr, G_PI/2, x, y, FALSE, FALSE);
		height = width;
		width = tmp;
	}

	if (params->prelight)
		tn_shade_color (&fill, 1.1, &fill);

	cairo_set_line_width (cr, 1);

	tn_shade_color (&fill, 1.45, &hilight);

	cairo_pattern_t *pattern;

	CairoColor shade1, shade2;
	tn_shade_color (&fill, 0.96, &shade1);
	tn_shade_color (&fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (0, 1, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
	/* exchange rectangles when uncommenting slider-border code
	cairo_rectangle (cr, 3, 2, width-6, height-4); */
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	/* slider-border
	CairoColor *border  = (CairoColor*)&colors->shade[6];

	cairo_rectangle (cr, 2.5, 1.5, width-5, height-3);
	cairo_set_source_rgb (cr, hilight.r, hilight.g, hilight.b);
	cairo_stroke (cr);

	tn_cairo_rounded_rectangle (cr, 1.5, 0.5, width-3, height-1, 2, params->corners);
	cairo_set_source_rgb (cr, border->r, border->g, border->b);
	cairo_stroke (cr); */

	if (scrollbar->style == 1)
	{
		stroke_width = 25;
		offset = 1;
		x_step = (((float)stroke_width/10)*offset);
		arrow_width = stroke_width/2;
		while (tile_pos <= width+x_step)
		{
			cairo_move_to (cr, stroke_width/2-x_step, 0);
			cairo_line_to (cr, stroke_width-x_step, height/2);
			cairo_line_to (cr, stroke_width/2-x_step, height);
			cairo_line_to (cr, stroke_width/2-x_step-arrow_width, height);
			cairo_line_to (cr, stroke_width-x_step-arrow_width, height/2);
			cairo_line_to (cr, stroke_width/2-x_step-arrow_width, 0);

			cairo_translate (cr, stroke_width, 0);
			tile_pos += stroke_width;
		}

		cairo_set_source_rgba (cr, colors->spot[2].r,
					colors->spot[2].g,
					colors->spot[2].b,
					 0.15);
		cairo_fill (cr);
		cairo_restore (cr);
		cairo_set_source_rgba (cr, colors->spot[0].r, colors->spot[0].g, colors->spot[0].b, 0.5);
		cairo_rectangle (cr, 0.5, 0.5, width-2, height-1);
		cairo_stroke (cr);
		cairo_reset_clip (cr);
	}
	else if (scrollbar->style == 0)
	{
		double stroke_width = 25;

		x_step = (((float)stroke_width/10)*2);

		/* Draw strokes */
		while (tile_pos <= width+x_step)
		{
			cairo_move_to (cr, stroke_width/2-x_step, 0);
			cairo_line_to (cr, stroke_width-x_step,   0);
			cairo_line_to (cr, stroke_width/2-x_step-2, height);
			cairo_line_to (cr, -x_step, height);
			cairo_translate (cr, stroke_width, 0);
			tile_pos += stroke_width;
		}

		cairo_set_source_rgba (cr, colors->spot[2].r,
						colors->spot[2].g,
						colors->spot[2].b,
						0.15);

		cairo_fill (cr);

		cairo_restore (cr);

		tn_shade_color (&colors->bg[GTK_STATE_SELECTED], 1.00, &shading);

		cairo_set_source_rgba (cr, shading.r, shading.g, shading.b, 0.5);
		cairo_rectangle (cr, 0.5, 0.5, width-2, height-1);
		cairo_stroke (cr);

		cairo_reset_clip (cr);
	}
}

static void
terranova_draw_candy_selected_cell (cairo_t                  *cr,
	                       const terranovaColors   *colors,
	                       const WidgetParameters   *params,
	                       int x, int y, int width, int height)
{
	cairo_save (cr);

	cairo_translate (cr, x, y);

	cairo_pattern_t *pattern;

	CairoColor fill = colors->bg[params->state_type];
	CairoColor shade1, shade2;
	tn_shade_color (&fill, 0.96, &shade1);
	tn_shade_color (&fill, 1.10, &shade2);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);

	cairo_set_source (cr, pattern);
	cairo_rectangle  (cr, 0, 0, width, height);
	cairo_fill       (cr);
	cairo_pattern_destroy (pattern);

	cairo_restore (cr);
}

static void
terranova_draw_candy_slider (cairo_t *cr,
                        const terranovaColors *colors,
                        const WidgetParameters *params,
                        int x, int y, int width, int height)
{
	CairoColor border = colors->shade[params->disabled ? 4 : 6];
	const CairoColor *fill   = &colors->bg[params->state_type];
	double radius = MIN (params->radius, MIN ((width - 1.0) / 2.0, (height - 1.0) / 2.0));

	cairo_pattern_t *pattern;

	cairo_set_line_width (cr, 1.0);
	cairo_translate      (cr, x, y);

	if (params->prelight)
		border = colors->spot[2];

	/* fill the widget */
	tn_cairo_rounded_rectangle (cr, 1.0, 1.0, width-2, height-2, radius, params->corners);

	/* Fake light */
	if (!params->disabled)
	{
		const CairoColor *top = &colors->shade[0];
		const CairoColor *bot = &colors->shade[2];

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0,  top->r, top->g, top->b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.5,  top->r, top->g, top->b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.5,  bot->r, bot->g, bot->b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0,  bot->r, bot->g, bot->b);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	else
	{
		tn_cairo_set_color (cr, fill);
		cairo_rectangle    (cr, 0.5, 0.5, width-2, height-2);
		cairo_fill         (cr);
	}

	/* Set the clip */
	cairo_save (cr);
	cairo_rectangle (cr, 1.0, 1.0, 6, height-2);
	cairo_rectangle (cr, width-7.0, 0.5, 6, height-2);
	cairo_clip (cr);

	cairo_new_path (cr);

	/* Draw the handles */
	tn_cairo_rounded_rectangle (cr, 1.0, 1.0, width-1, height-1, radius, params->corners);
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	if (params->slider_style == 1)
	{
		if (params->prelight || params->active)
		{
			CairoColor highlight, low;
			tn_shade_color (fill, 1.10, &highlight);
			tn_shade_color (fill, 0.90, &low);
			cairo_pattern_add_color_stop_rgb (pattern, 0.0, highlight.r, highlight.g, highlight.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, highlight.r, highlight.g, highlight.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, low.r, low.g, low.b);
			cairo_pattern_add_color_stop_rgb (pattern, 1.0, low.r, low.g, low.b);
			cairo_set_source (cr, pattern);
		}
	}
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	cairo_reset_clip (cr);

	/* Draw the border */
	tn_cairo_inner_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);

	if (params->prelight || params->disabled)
		tn_cairo_set_color (cr, &border);
	else
		terranova_draw_border_gradient (cr, &border, 1.2, 0, height);
	cairo_stroke (cr);

	/* Draw handle lines */
	if (width > 14)
	{
		cairo_move_to (cr, x+6, 0.5);
		cairo_line_to (cr, x+6, height-0.5);

		cairo_move_to (cr, x+width-7, 0.5);
		cairo_line_to (cr, x+width-7, height-0.5);

		cairo_set_line_width (cr, 1.0);
		cairo_set_source_rgba (cr, border.r,
		                           border.g,
		                           border.b,
	                           0.3);
		cairo_stroke (cr);
	}

}

static void
terranova_draw_candy_tab (cairo_t *cr,
                     const terranovaColors *colors,
                     const WidgetParameters *params,
                     const TabParameters    *tab,
                     int x, int y, int width, int height)
{
	const CairoColor    *border       = &colors->shade[5];
	const CairoColor    *stripe_fill   = &colors->spot[1];
	const CairoColor    *stripe_border = &colors->spot[2];
	const CairoColor    *fill;
	CairoColor           hilight;

	cairo_pattern_t     *pattern;

	double               radius;
	double               strip_size;
	double               length;

	radius = MIN (params->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));

	/* Set clip */
	cairo_rectangle      (cr, x, y, width, height);
	cairo_clip           (cr);
	cairo_new_path       (cr);

	/* Translate and set line width */
	cairo_set_line_width (cr, 1.0);
	cairo_translate      (cr, x+0.5, y+0.5);


	/* Make the tabs slightly bigger than they should be, to create a gap */
	/* And calculate the strip size too, while you're at it */
	if (tab->gap_side == TN_GAP_TOP || tab->gap_side == TN_GAP_BOTTOM)
	{
		height += 3.0;
		length = height;
	 	strip_size = 2.0/height; /* 2 pixel high strip */

		if (tab->gap_side == TN_GAP_TOP)
			cairo_translate (cr, 0.0, -3.0); /* gap at the other side */
	}
	else
	{
		width += 3.0;
		length = width;
	 	strip_size = 2.0/width;

		if (tab->gap_side == TN_GAP_LEFT)
			cairo_translate (cr, -3.0, 0.0); /* gap at the other side */
	}

	/* Set the fill color */
	fill = &colors->bg[params->state_type];

	/* Set tab shape */
	tn_cairo_rounded_rectangle (cr, 0, 0, width-1, height-1,
	                            radius, params->corners);

	/* Draw fill */
	tn_shade_color (fill, 1.25, &hilight);
	tn_cairo_set_color (cr, &hilight);
	cairo_fill   (cr);



	/* Draw highlight */
	if (!params->active)
	{
		ShadowParameters shadow;

		shadow.shadow  = TN_SHADOW_OUT;
		shadow.corners = params->corners;

		terranova_draw_highlight_and_shade (cr, colors, &shadow, width, height, radius);
	}

	if (params->active)
	{
		switch (tab->gap_side)
		{
			case TN_GAP_TOP:
				pattern = cairo_pattern_create_linear (0, height-2, 0, 0);
				break;
			case TN_GAP_BOTTOM:
				pattern = cairo_pattern_create_linear (0, 0, 0, height);
				break;
			case TN_GAP_LEFT:
				pattern = cairo_pattern_create_linear (width-2, 0, 0, 0);
				break;
			case TN_GAP_RIGHT:
				pattern = cairo_pattern_create_linear (0, 0, width-2, 0);
				break;
			default:
				pattern = NULL;
		}

		tn_cairo_rounded_rectangle (cr, 1, 1, width-3, height-1, radius, params->corners);

		CairoColor low, middle;

		tn_shade_color (fill, 0.96, &middle);
		tn_shade_color (fill, 1.15, &low);

		cairo_pattern_add_color_stop_rgb (pattern, 0.00, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.90, low.r, low.g, low.b);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);

	}
	else
	{
		/* Draw shade */
		switch (tab->gap_side)
		{
			case TN_GAP_TOP:
				pattern = cairo_pattern_create_linear (0, height-2, 0, 0);
				break;
			case TN_GAP_BOTTOM:
				pattern = cairo_pattern_create_linear (0, 0, 0, height);
				break;
			case TN_GAP_LEFT:
				pattern = cairo_pattern_create_linear (width-2, 0, 0, 0);
				break;
			case TN_GAP_RIGHT:
				pattern = cairo_pattern_create_linear (0, 0, width, 0);
				break;
			default:
				pattern = NULL;
		}
		tn_cairo_rounded_rectangle (cr, 1, 1, width-3, height-1, radius, params->corners);

		CairoColor low, middle;

		tn_shade_color (stripe_fill, 0.96, &middle);
		tn_shade_color (stripe_fill, 1.15, &low);

		cairo_pattern_add_color_stop_rgb (pattern, 0.00, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.90, low.r, low.g, low.b);
		cairo_set_source (cr, pattern);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}

	tn_cairo_rounded_rectangle (cr, 0, 0, width-1, height-1, radius, params->corners);

	if (params->active)
	{
		tn_cairo_set_color (cr, border);
		cairo_stroke (cr);
	}
	else
	{
		switch (tab->gap_side)
		{
			case TN_GAP_TOP:
				pattern = cairo_pattern_create_linear (2, height-2, 2, 2);
				break;
			case TN_GAP_BOTTOM:
				pattern = cairo_pattern_create_linear (2, 2, 2, height);
				break;
			case TN_GAP_LEFT:
				pattern = cairo_pattern_create_linear (width-2, 2, 2, 2);
				break;
			case TN_GAP_RIGHT:
				pattern = cairo_pattern_create_linear (2, 2, width, 2);
				break;
			default:
				pattern = NULL;
		}

		cairo_pattern_add_color_stop_rgb (pattern, 0.0, stripe_border->r, stripe_border->g, stripe_border->b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.8, border->r,        border->g,        border->b);
		cairo_set_source (cr, pattern);
		cairo_stroke (cr);

	}
}

static void
terranova_draw_candy_unselected_cell (cairo_t                  *cr,
	                       const terranovaColors   *colors,
	                       const WidgetParameters   *params,
	                       int x, int y, int width, int height)
{

	if (params->draw_unselected == 1)
	{
		CairoColor shade1, shade2;
		cairo_save (cr);

		cairo_translate (cr, x, y);

		CairoColor fill = colors->bg[params->state_type];

		cairo_pattern_t *pattern;
		tn_shade_color (&fill, 0.94, &shade1);
		tn_shade_color (&fill, 1.00, &shade2);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);

		cairo_set_source (cr, pattern);
		cairo_rectangle  (cr, 0, 0, width, height);
		cairo_fill       (cr);
		cairo_pattern_destroy (pattern);

		cairo_restore (cr);
	}

}

static void
terranova_draw_candy_tooltip (cairo_t *cr,
                         const terranovaColors          *colors,
                         const WidgetParameters *params,
                         int x, int y, int width, int height)
{
	const CairoColor *stripe_fill = &colors->bg[GTK_STATE_SELECTED];
	const CairoColor *fill = &colors->bg[GTK_STATE_NORMAL];
	CairoColor shade1, shade2, middle, shadow, hilight;
	cairo_pattern_t *pattern;

	cairo_translate (cr, x, y);

	if (params->tooltip_style == 0)
	{

	cairo_rectangle(cr, 0, 0, width, height);

	tn_shade_color (fill, 0.96, &shade1);
	tn_shade_color (fill, 1.10, &shade2);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.35, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.65, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade2.r, shade2.g, shade2.b);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_fill (cr);

	tn_shade_color (stripe_fill, 1.10, &middle);
	tn_shade_color (stripe_fill, 0.90, &shadow);
	tn_shade_color (stripe_fill, 1.35, &hilight);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.25,        hilight.r, hilight.g, hilight.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.5,        middle.r, middle.g, middle.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.75,        shadow.r, shadow.g, shadow.b);
	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_move_to  (cr, 0, 0);
	cairo_line_to  (cr, width, 0);
	cairo_curve_to (cr, width, 0, 0, 0, 0, height);
	cairo_close_path (cr);

	}
	else if (params->tooltip_style == 1)
	{

		tn_cairo_set_color (cr, &colors->bg[params->state_type]);
		cairo_rectangle (cr, 0, 0, width, height);

	}

	cairo_fill(cr);

	tn_cairo_set_color(cr, &colors->text[GTK_STATE_NORMAL]);
	cairo_rectangle (cr, 0.5, 0.5, width - 1, height - 1);
	cairo_stroke (cr);

}

void
terranova_register_style_candy (terranovaStyleFunctions *functions)
{
	g_assert (functions);

	functions->draw_button 				= terranova_draw_candy_button;
	functions->draw_entry				= terranova_draw_candy_entry;
	functions->draw_icon_view_item			= terranova_draw_candy_icon_view_item;
	functions->draw_list_view_header		= terranova_draw_candy_list_view_header;
	functions->draw_menubaritem			= terranova_draw_candy_menubaritem;
	functions->draw_menuitem			= terranova_draw_candy_menuitem;
	functions->draw_progressbar_fill		= terranova_draw_candy_progressbar_fill;
	functions->draw_progressbar_trough		= terranova_draw_candy_progressbar_trough;
	functions->draw_scrollbar_slider		= terranova_draw_candy_scrollbar_slider;
	functions->draw_selected_cell			= terranova_draw_candy_selected_cell;
	functions->draw_slider				= terranova_draw_candy_slider;
	functions->draw_tab				= terranova_draw_candy_tab;
	functions->draw_unselected_cell			= terranova_draw_candy_unselected_cell;
	functions->draw_tooltip				= terranova_draw_candy_tooltip;

}
