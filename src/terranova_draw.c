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

typedef void (*menubar_draw_proto) (cairo_t *cr,
                                    const terranovaColors *colors,
                                    const WidgetParameters *params,
                                    const MenuBarParameters *menubar,
                                    int x, int y, int width, int height);

static void
terranova_draw_inset (cairo_t          *cr,
                       const CairoColor *bg_color,
                       double x, double y, double w, double h,
                       double radius, uint8 corners)
{
	CairoColor shadow, highlight;

	tn_shade_color (bg_color, 0.94, &shadow);
	tn_shade_color (bg_color, 1.06, &highlight);

	cairo_move_to (cr, x + w + (radius * -0.2928932188), y - (radius * -0.2928932188));

	if (corners & CR_CORNER_TOPRIGHT)
		cairo_arc (cr, x + w - radius, y + radius, radius, G_PI * 1.75, G_PI * 2);
	else
		cairo_line_to (cr, x + w, y);

	if (corners & CR_CORNER_BOTTOMRIGHT)
		cairo_arc (cr, x + w - radius, y + h - radius, radius, 0, G_PI * 0.5);
	else
		cairo_line_to (cr, x + w, y + h);

	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_arc (cr, x + radius, y + h - radius, radius, G_PI * 0.5, G_PI * 0.75);
	else
		cairo_line_to (cr, x, y + h);

	tn_cairo_set_color (cr, &highlight);
	cairo_stroke (cr);

	/* shadow */
	cairo_move_to (cr, x + (radius * 0.2928932188), y + h + (radius * -0.2928932188));

	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_arc (cr, x + radius, y + h - radius, radius, M_PI * 0.75, M_PI);
	else
		cairo_line_to (cr, x, y + h);

	if (corners & CR_CORNER_TOPLEFT)
		cairo_arc (cr, x + radius, y + radius, radius, M_PI, M_PI * 1.5);
	else
		cairo_line_to (cr, x, y);

	if (corners & CR_CORNER_TOPRIGHT)
	    cairo_arc (cr, x + w - radius, y + radius, radius, M_PI * 1.5, M_PI * 1.75);
	else
		cairo_line_to (cr, x + w, y);

	tn_cairo_set_color (cr, &shadow);
	cairo_stroke (cr);
}

static void
terranova_draw_shadow (cairo_t *cr, const terranovaColors *colors, gfloat radius, int width, int height)
{
	CairoColor shadow;
	tn_shade_color (&colors->shade[5], 0.85, &shadow);

	cairo_set_line_width (cr, 1.0);

	cairo_set_source_rgba (cr, shadow.r, shadow.g, shadow.b, 0.1);

	cairo_move_to (cr, width, radius);
	tn_cairo_rounded_corner (cr, width, height, radius, CR_CORNER_BOTTOMRIGHT);
	cairo_line_to (cr, radius, height);

	cairo_stroke (cr);
}

static void
terranova_draw_gripdots (cairo_t *cr, const terranovaColors *colors, int x, int y,
                          int width, int height, int xr, int yr,
                          float contrast)
{
	const CairoColor *dark = &colors->shade[4];
	CairoColor hilight;
	int i, j;
	int xoff, yoff;

	tn_shade_color (dark, 0.85, &hilight);

	for ( i = 0; i < xr; i++ )
	{
		for ( j = 0; j < yr; j++ )
		{
			xoff = x -(xr * 3 / 2) + 3 * i;
			yoff = y -(yr * 3 / 2) + 3 * j;

			cairo_rectangle (cr, width/2+0.75+xoff, height/2+0.75+yoff, 2, 2);
			cairo_set_source_rgba (cr, hilight.r, hilight.g, hilight.b, 0.8+contrast);
			cairo_fill (cr);
			cairo_rectangle (cr, width/2+0.75+xoff, height/2+0.75+yoff, 2, 2);
			cairo_set_source_rgba (cr, dark->r, dark->g, dark->b, 0.8+contrast);
			cairo_fill (cr);
		}
	}
}

static void
terranova_draw_button (cairo_t *cr,
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

	radius = MIN (radius, MIN ((width - 2.0 - xoffset * 2.0) / 2.0, (height - 2.0 - yoffset * 2) / 2.0));

	tn_cairo_rounded_rectangle (cr, xoffset+2, yoffset+2,
						width-(xoffset*2)-2,
						height-(yoffset*2)-2,
						radius, params->corners);

	GradientPositions gradient_position;
	gradient_position = terranova_get_gradient_position(params, width, height);

	if (params->disabled)
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (&fill, 1.02, &shade1);
		tn_shade_color (&fill, 0.98, &shade2);
		tn_shade_color (&fill, 0.94, &shade3);
		tn_shade_color (&fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}
	else if (params->prelight)
	{

		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (&fill, 1.10, &shade4);
		tn_shade_color (&fill, 1.00, &shade3);
		tn_shade_color (&fill, 0.90, &shade2);
		tn_shade_color (&fill, 0.85, &shade1);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);

	}
	else if (params->active)
	{

		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (&fill, 1.20, &shade4);
		tn_shade_color (&fill, 1.10, &shade3);
		tn_shade_color (&fill, 1.00, &shade2);
		tn_shade_color (&fill, 0.95, &shade1);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);

	}
	else
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (&fill, 1.20, &shade1);
		tn_shade_color (&fill, 1.10, &shade2);
		tn_shade_color (&fill, 1.00, &shade3);
		tn_shade_color (&fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0,
				gradient_position.width, gradient_position.height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
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
terranova_draw_mw_gradient (cairo_t *cr,
                       const terranovaColors *colors,
                       const WidgetParameters *params,
                       int x, int y, int width, int height)
{
	cairo_pattern_t *pattern;

	const CairoColor *fill = &colors->bg[params->state_type];
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	CairoColor highlight, low, lower, normal;
	tn_shade_color (fill, 1.20, &highlight);
	tn_shade_color (fill, 1.15, &low);
	tn_shade_color (fill, 0.88, &lower);
	tn_shade_color (fill, 1.00, &normal);

	if (params->mw_gradient == 0)
	{
		pattern = cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 1.0, normal.r, normal.g, normal.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	else if (params->mw_gradient == 1)
	{
		pattern = cairo_pattern_create_linear (0, 0, 0, height);

		cairo_pattern_add_color_stop_rgb (pattern, 0.00, highlight.r, highlight.g, highlight.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.45, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.60, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.80, lower.r, lower.g, lower.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}
	else if (params->mw_gradient == 2)
	{
		pattern = cairo_pattern_create_linear (0, 0, width/1.75, height);

		cairo_pattern_add_color_stop_rgb (pattern, 0.00, highlight.r, highlight.g, highlight.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.45, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.60, low.r, low.g, low.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.80, lower.r, lower.g, lower.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill (cr);
		cairo_pattern_destroy (pattern);
	}

	cairo_restore (cr);

}

static void
terranova_draw_entry (cairo_t *cr,
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

	if (params->disabled)
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.02, &shade1);
		tn_shade_color (fill, 0.98, &shade2);
		tn_shade_color (fill, 0.94, &shade3);
		tn_shade_color (fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);
	}
	else
	{
		cairo_pattern_t *pattern;

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.20, &shade1);
		tn_shade_color (fill, 1.10, &shade2);
		tn_shade_color (fill, 1.00, &shade3);
		tn_shade_color (fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
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
terranova_draw_spinbutton (cairo_t *cr,
                            const terranovaColors *colors,
                            const WidgetParameters *params,
                            int x, int y, int width, int height)
{

	params->style_functions->draw_button (cr, colors, params, x, y, width, height);

}

static void
terranova_draw_spinbutton_down (cairo_t *cr,
                                 const terranovaColors *colors,
                                 const WidgetParameters *params,
                                 int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));
	CairoColor shadow;
	tn_shade_color (&colors->bg[GTK_STATE_NORMAL], 0.8, &shadow);

	cairo_translate (cr, x+1, y+1);

	tn_cairo_rounded_rectangle (cr, 1, 1, width-4, height-4, radius, params->corners);

	tn_cairo_set_color (cr, &colors->bg[params->state_type]);

	cairo_fill_preserve (cr);

}

static void
terranova_scale_draw_gradient (cairo_t *cr,
                                const CairoColor *c1,
                                const CairoColor *c2,
                                const CairoColor *c3,
                                const CairoColor *c4,
                                int x, int y, int width, int height,
                                boolean horizontal)
{
	cairo_pattern_t *pattern;

	pattern = cairo_pattern_create_linear (0, 0, horizontal ? 0 :  width, horizontal ? height : 0);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, c1->r, c1->g, c1->b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, c2->r, c2->g, c2->b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, c2->r, c2->g, c2->b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.75, c3->r, c3->g, c3->b);

	cairo_rectangle (cr, x+0.5, y+0.5, width-1, height-1);
	cairo_set_source (cr, pattern);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

	tn_cairo_set_color (cr, c4);
	tn_cairo_stroke_rectangle (cr, x, y, width, height);
}

static void
terranova_draw_scale_trough (cairo_t *cr,
                              const terranovaColors *colors,
                              const WidgetParameters *params,
                              const SliderParameters *slider,
                              int x, int y, int width, int height)
{
	int     trough_width, trough_height;
	double  translate_x, translate_y;

	cairo_save(cr);

	if (slider->horizontal)
	{
		trough_width  = width-3;
		trough_height = params->scale_trough_size;

		translate_x   = x + 0.5;
		translate_y   = y + 0.5 + (height/2) - (params->scale_trough_size/2);
	}
	else
	{
		trough_width  = params->scale_trough_size;
		trough_height = height-6;

		translate_x   = x + 0.5 + (width/2) - (params->scale_trough_size/2) - 1;
		translate_y   = y + 0.5;
	}

	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, translate_x, translate_y);

	if (!slider->fill_level)
		params->style_functions->draw_inset (cr, &params->parentbg, 0, 0, trough_width, trough_height, 0, 0);

	if (!slider->lower && ! slider->fill_level)
		terranova_scale_draw_gradient (cr, &colors->shade[2], /* top */
		                                    &colors->shade[1], /* middle */
		                                    &colors->shade[0], /* bottom */
		                                    &colors->shade[6], /* border */
		                                    0, 0, trough_width, trough_height,
		                                    slider->horizontal);
	else
		terranova_scale_draw_gradient (cr, &colors->spot[1], /* top    */
		                                    &colors->spot[0], /* middle */
		                                    &colors->spot[1], /* bottom */
		                                    &colors->shade[6], /* border */
		                                    0, 0, trough_width, trough_height,
		                                    slider->horizontal);

	cairo_restore(cr);
}

static void
terranova_draw_slider (cairo_t *cr,
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

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.20, &shade1);
		tn_shade_color (fill, 1.10, &shade2);
		tn_shade_color (fill, 1.00, &shade3);
		tn_shade_color (fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);

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
				CairoColor hilight, shade1, shade2, shade3, shade4;
				hilight = colors->bg[GTK_STATE_SELECTED];

				tn_shade_color (&hilight, 1.20, &shade1);
				tn_shade_color (&hilight, 1.10, &shade2);
				tn_shade_color (&hilight, 1.00, &shade3);
				tn_shade_color (&hilight, 0.90, &shade4);

				cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
				cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
				cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
				cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
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
terranova_draw_slider_button (cairo_t *cr,
                               const terranovaColors *colors,
                               const WidgetParameters *params,
                               const SliderParameters *slider,
                               int x, int y, int width, int height)
{
	double radius = MIN (params->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));
	cairo_set_line_width (cr, 1.0);

	if (!slider->horizontal)
		tn_cairo_exchantn_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y);

	params->style_functions->draw_shadow (cr, colors, radius, width-1, height-1);

	params->style_functions->draw_slider (cr, colors, params, 1, 1, width-2, height-1);

	params->style_functions->draw_gripdots (cr, colors, 0, 0, width-3, height-0.5, 3, 2, 0);
}

static void
terranova_draw_progressbar_trough (cairo_t *cr,
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

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.20, &shade1);
		tn_shade_color (fill, 1.10, &shade2);
		tn_shade_color (fill, 1.00, &shade3);
		tn_shade_color (fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width - 1, height - 1, params->radius, params->corners);
		cairo_fill_preserve (cr);
		cairo_pattern_destroy (pattern);

}

static void
terranova_draw_progressbar_fill (cairo_t *cr,
                                  const terranovaColors *colors,
                                  const WidgetParameters *params,
                                  const ProgressBarParameters *progressbar,
                                  int x, int y, int width, int height,
                                  gint offset)
{

	boolean is_horizontal = progressbar->orientation < 2;

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

	tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);

	/* Draw fill */
	cairo_set_source_rgb (cr, colors->spot[1].r,
	                          colors->spot[1].g,
	                          colors->spot[1].b);
	cairo_fill_preserve (cr);

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (&fill, 1.20, &shade1);
	tn_shade_color (&fill, 1.10, &shade2);
	tn_shade_color (&fill, 1.00, &shade3);
	tn_shade_color (&fill, 0.90, &shade4);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
	cairo_set_source (cr, pattern);
	cairo_fill(cr);
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
terranova_draw_optionmenu (cairo_t *cr,
                            const terranovaColors *colors,
                            const WidgetParameters *params,
                            const OptionMenuParameters *optionmenu,
                            int x, int y, int width, int height)
{
	params->style_functions->draw_button (cr, colors, params, x, y, width, height);
}

static void
terranova_draw_menu_item_separator (cairo_t                   *cr,
                                     const terranovaColors    *colors,
                                     const WidgetParameters *params,
                                     const SeparatorParameters *separator,
                                     int x, int y, int width, int height)
{
	cairo_save (cr);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_BUTT);
	tn_cairo_set_color (cr, &colors->shade[5]);

	if (separator->horizontal)
		cairo_rectangle (cr, x, y, width, 1);
	else
		cairo_rectangle (cr, x, y, 1, height);

	cairo_fill      (cr);

	cairo_restore (cr);
}

static void
terranova_draw_menubar0 (cairo_t *cr,
                          const terranovaColors *colors,
                          const WidgetParameters *params,
                          const MenuBarParameters *menubar,
                          int x, int y, int width, int height)
{
	CairoColor lower;
	CairoColor top;
	cairo_pattern_t *pattern;

	tn_shade_color (&colors->bg[0], 1.1, &top);
	tn_shade_color (&colors->bg[0], 0.9, &lower);

	cairo_translate (cr, x, y);
	cairo_rectangle (cr, 0, 0, width, height);

	/* Draw the gradient */
	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.0, top.r,
	                                                top.g,
	                                                top.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.0, lower.r,
	                                                lower.g,
	                                                lower.b);
	cairo_set_source      (cr, pattern);
	cairo_fill            (cr);
	cairo_stroke (cr);
	cairo_pattern_destroy (pattern);

}

static menubar_draw_proto terranova_menubar_draw[1] =
{
	terranova_draw_menubar0,
};

static void
terranova_draw_menubar (cairo_t *cr,
                         const terranovaColors *colors,
                         const WidgetParameters *params,
                         const MenuBarParameters *menubar,
                         int x, int y, int width, int height)
{
	if (menubar->style < 0 || menubar->style >= G_N_ELEMENTS (terranova_menubar_draw))
		return;

	terranova_menubar_draw[menubar->style](cr, colors, params, menubar,
	                             x, y, width, height);
}

static void
terranova_get_frame_gap_clip (int x, int y, int width, int height,
                               const FrameParameters     *frame,
                               terranovaRectangle *bevel,
                               terranovaRectangle *border)
{
	if (frame->gap_side == TN_GAP_TOP)
	{
		TERRANOVA_RECTANGLE_SET ((*bevel),  1.5 + frame->gap_x,  -0.5,
											 frame->gap_width - 3, 2.0);
		TERRANOVA_RECTANGLE_SET ((*border), 0.5 + frame->gap_x,  -0.5,
											 frame->gap_width - 2, 2.0);
	}
	else if (frame->gap_side == TN_GAP_BOTTOM)
	{
		TERRANOVA_RECTANGLE_SET ((*bevel),  1.5 + frame->gap_x,  height - 2.5,
											 frame->gap_width - 3, 2.0);
		TERRANOVA_RECTANGLE_SET ((*border), 0.5 + frame->gap_x,  height - 1.5,
											 frame->gap_width - 2, 2.0);
	}
	else if (frame->gap_side == TN_GAP_LEFT)
	{
		TERRANOVA_RECTANGLE_SET ((*bevel),  -0.5, 1.5 + frame->gap_x,
											 2.0, frame->gap_width - 3);
		TERRANOVA_RECTANGLE_SET ((*border), -0.5, 0.5 + frame->gap_x,
											 1.0, frame->gap_width - 2);
	}
	else if (frame->gap_side == TN_GAP_RIGHT)
	{
		TERRANOVA_RECTANGLE_SET ((*bevel),  width - 2.5, 1.5 + frame->gap_x,
											 2.0, frame->gap_width - 3);
		TERRANOVA_RECTANGLE_SET ((*border), width - 1.5, 0.5 + frame->gap_x,
											 1.0, frame->gap_width - 2);
	}
}

static void
terranova_draw_frame            (cairo_t *cr,
                                  const terranovaColors     *colors,
                                  const WidgetParameters     *params,
                                  const FrameParameters      *frame,
                                  int x, int y, int width, int height)
{
	const CairoColor *border = frame->border;
	const CairoColor *dark   = (CairoColor*)&colors->shade[8];
	terranovaRectangle bevel_clip = {0, 0, 0, 0};
	terranovaRectangle frame_clip = {0, 0, 0, 0};
	double radius = MIN (params->radius, MIN ((width - 2.0) / 2.0, (height - 2.0) / 2.0));
	CairoColor hilight;

	tn_shade_color (&colors->bg[GTK_STATE_NORMAL], 1.05, &hilight);

	if (frame->shadow == TN_SHADOW_NONE)
		return;

	if (frame->gap_x != -1)
		terranova_get_frame_gap_clip (x, y, width, height,
		                               frame, &bevel_clip, &frame_clip);

	cairo_set_line_width (cr, 1.0);
	cairo_translate      (cr, x+0.5, y+0.5);

	/* save everything */
	cairo_save (cr);
	/* Set clip for the bevel */
	if (frame->gap_x != -1)
	{
		/* Set clip for gap */
		cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
		cairo_rectangle      (cr, -0.5, -0.5, width, height);
		cairo_rectangle      (cr, bevel_clip.x, bevel_clip.y, bevel_clip.width, bevel_clip.height);
		cairo_clip           (cr);
	}

	/* Draw the bevel */
	if (frame->shadow == TN_SHADOW_ETCHED_IN || frame->shadow == TN_SHADOW_ETCHED_OUT)
	{
		tn_cairo_set_color (cr, &hilight);
		if (frame->shadow == TN_SHADOW_ETCHED_IN)
			tn_cairo_rounded_rectangle (cr, 0, 0, width-2, height-2, radius, params->corners);
		else
			tn_cairo_rounded_rectangle (cr, 0, 0, width-2, height-2, radius, params->corners);
		cairo_stroke (cr);
	}
	else if (frame->shadow != TN_SHADOW_NONE)
	{
		ShadowParameters shadow;
		shadow.corners = params->corners;
		shadow.shadow  = frame->shadow;
		terranova_draw_highlight_and_shade (cr, colors, &shadow, width, height, 0);
	}

	/* restore the previous clip region */
	cairo_restore    (cr);
	cairo_save       (cr);
	if (frame->gap_x != -1)
	{
		/* Set clip for gap */
		cairo_set_fill_rule  (cr, CAIRO_FILL_RULE_EVEN_ODD);
		cairo_rectangle      (cr, -0.5, -0.5, width, height);
		cairo_rectangle      (cr, frame_clip.x, frame_clip.y, frame_clip.width, frame_clip.height);
		cairo_clip           (cr);
	}

	/* Draw frame */
	if (frame->shadow == TN_SHADOW_ETCHED_IN || frame->shadow == TN_SHADOW_ETCHED_OUT)
	{
		tn_cairo_set_color (cr, dark);
		if (frame->shadow == TN_SHADOW_ETCHED_IN)
			tn_cairo_rounded_rectangle (cr, 0, 0, width-2, height-2, radius, params->corners);
		else
			tn_cairo_rounded_rectangle (cr, 1, 1, width-2, height-2, radius, params->corners);
	}
	else
	{
		tn_cairo_set_color (cr, border);
		tn_cairo_rounded_rectangle (cr, 0, 0, width-1, height-1, radius, params->corners);
	}
	cairo_stroke (cr);

	cairo_restore (cr);
}

static void
terranova_draw_tab (cairo_t *cr,
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

			CairoColor shade1, shade2, shade3, shade4;
			tn_shade_color (fill, 1.20, &shade1);
			tn_shade_color (fill, 1.10, &shade2);
			tn_shade_color (fill, 1.00, &shade3);
			tn_shade_color (fill, 0.90, &shade4);

			cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
			cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
			cairo_set_source (cr, pattern);
			cairo_fill(cr);
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

			CairoColor shade1, shade2, shade3, shade4;
			tn_shade_color (stripe_fill, 1.20, &shade1);
			tn_shade_color (stripe_fill, 1.10, &shade2);
			tn_shade_color (stripe_fill, 1.00, &shade3);
			tn_shade_color (stripe_fill, 0.90, &shade4);

			cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
			cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
			cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
			cairo_set_source (cr, pattern);
			cairo_fill(cr);
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
terranova_draw_list_view_header (cairo_t *cr,
                                  const terranovaColors          *colors,
                                  const WidgetParameters          *params,
                                  const ListViewHeaderParameters  *header,
                                  int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_NORMAL];
	cairo_pattern_t *pattern;

	cairo_translate (cr, x, y);

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (fill, 1.20, &shade1);
	tn_shade_color (fill, 1.10, &shade2);
	tn_shade_color (fill, 1.00, &shade3);
	tn_shade_color (fill, 0.90, &shade4);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
	cairo_set_source (cr, pattern);
	cairo_rectangle (cr, 0, 0, width, height);
	cairo_fill (cr);
	cairo_pattern_destroy (pattern);

}

static void
terranova_draw_toolbar (cairo_t *cr,
                         const terranovaColors          *colors,
                         const WidgetParameters *params,
                         const ToolbarParameters         *toolbar,
                         int x, int y, int width, int height)
{
	cairo_set_line_width (cr, 1.0);
	cairo_translate (cr, x, y);

	if (toolbar->style == 0)
	{
		cairo_pattern_t *pattern;
		CairoColor top, lower;

		tn_shade_color (&colors->bg[0], 0.9, &top);
		tn_shade_color (&colors->bg[0], 1.25, &lower);

		pattern = cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.0, top.r,
								top.g,
								top.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.9, lower.r,
								lower.g,
								lower.b);
		cairo_set_source (cr, pattern);
		cairo_rectangle (cr, 0, 0, width, height);
		cairo_fill (cr);

		cairo_pattern_destroy (pattern);
	}
}

static void
terranova_draw_menuitem (cairo_t *cr,
                          const terranovaColors          *colors,
                          const WidgetParameters *params,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	double radius = MIN (params->radius, MIN ((width - 1.0) / 2.0, (height - 1.0) / 2.0));

	tn_shade_color (&border, 1.05, &border);
	cairo_set_line_width (cr, 1.0);

	cairo_translate (cr, x, y);
	tn_cairo_rounded_rectangle (cr, 0, 0, width, height, params->radius, params->corners);

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (fill, 1.20, &shade1);
	tn_shade_color (fill, 1.10, &shade2);
	tn_shade_color (fill, 1.00, &shade3);
	tn_shade_color (fill, 0.90, &shade4);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);

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
terranova_draw_menubaritem (cairo_t *cr,
                          const terranovaColors          *colors,
                          const WidgetParameters *params,
                          int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];
	CairoColor border = colors->spot[2];
	cairo_pattern_t *pattern;

	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	tn_shade_color (&border, 1.00, &border);

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (fill, 1.20, &shade1);
	tn_shade_color (fill, 1.10, &shade2);
	tn_shade_color (fill, 1.05, &shade3);
	tn_shade_color (fill, 0.95, &shade4);

	pattern	= cairo_pattern_create_linear (x, y, x, y + height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle (cr, x+0.75, y+0.75, width - 1, height - 1, radius, params->corners);
	cairo_fill_preserve (cr);
	cairo_pattern_destroy (pattern);

	tn_cairo_set_color (cr, &border);
	cairo_stroke_preserve (cr);
}

static void
terranova_draw_selected_cell (cairo_t                  *cr,
	                       const terranovaColors   *colors,
	                       const WidgetParameters   *params,
	                       int x, int y, int width, int height)
{
	CairoColor shade1, shade2, shade3, shade4;
	cairo_save (cr);

	cairo_translate (cr, x, y);

	cairo_pattern_t *pattern;

	CairoColor fill = colors->bg[params->state_type];
	tn_shade_color (&fill, 1.10, &shade1);
	tn_shade_color (&fill, 1.00, &shade2);
	tn_shade_color (&fill, 0.90, &shade3);
	tn_shade_color (&fill, 0.85, &shade4);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);

	cairo_set_source (cr, pattern);
	cairo_rectangle  (cr, 0, 0, width, height);
	cairo_fill       (cr);
	cairo_pattern_destroy (pattern);

	cairo_restore (cr);
}

static void
terranova_draw_unselected_cell (cairo_t                  *cr,
	                       const terranovaColors   *colors,
	                       const WidgetParameters   *params,
	                       int x, int y, int width, int height)
{

	if (params->draw_unselected == 1)
	{

		CairoColor shade1, shade2, shade3, shade4;
		cairo_save (cr);

		cairo_translate (cr, x, y);

		CairoColor fill = colors->bg[params->state_type];

		cairo_pattern_t *pattern;
		tn_shade_color (&fill, 1.20, &shade1);
		tn_shade_color (&fill, 1.10, &shade2);
		tn_shade_color (&fill, 1.02, &shade3);
		tn_shade_color (&fill, 0.93, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);

		cairo_set_source (cr, pattern);
		cairo_rectangle  (cr, 0, 0, width, height);
		cairo_fill       (cr);
		cairo_pattern_destroy (pattern);
		cairo_restore (cr);
	}

}

static void
terranova_draw_scrollbar_trough (cairo_t *cr,
                                  const terranovaColors           *colors,
                                  const WidgetParameters *params,
                                  const ScrollBarParameters        *scrollbar,
                                  int x, int y, int width, int height)
{

	if (params->trough_visible == 0)
	{
	}
	else
	{
	const CairoColor *bg     = &colors->shade[2];
	const CairoColor *border = &colors->shade[5];
	const CairoColor *fill   = &colors->bg[params->state_type];
	CairoColor        bg_shade;
	cairo_pattern_t *pattern;

	tn_shade_color (bg, 0.95, &bg_shade);

	cairo_set_line_width (cr, 1.0);

	if (scrollbar->horizontal)
		tn_cairo_exchantn_axis (cr, &x, &y, &width, &height);

	cairo_translate (cr, x, y-1);

	if (params->trough_style == 1)
	{
		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.20, &shade1);
		tn_shade_color (fill, 1.10, &shade2);
		tn_shade_color (fill, 1.05, &shade3);
		tn_shade_color (fill, 0.95, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, params->radius, CR_CORNER_NONE);
		cairo_fill_preserve (cr);
		cairo_pattern_destroy (pattern);

		cairo_rectangle (cr, 0, 0, width-0.5, height);
		tn_cairo_set_color (cr, border);
		cairo_stroke (cr);
	}
	else if (params->trough_style == 0)
	{
		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (fill, 1.20, &shade1);
		tn_shade_color (fill, 1.10, &shade2);
		tn_shade_color (fill, 1.00, &shade3);
		tn_shade_color (fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.60, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.40, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_set_source (cr, pattern);
		tn_cairo_rounded_rectangle (cr, 0, 0, width, height, params->radius, CR_CORNER_NONE);
		cairo_fill_preserve (cr);
		cairo_pattern_destroy (pattern);

		cairo_rectangle (cr, 0, 0, width-0.5, height);
		tn_cairo_set_color (cr, border);
		cairo_stroke (cr);
	}
	}

}

static void
terranova_draw_scrollbar_stepper (cairo_t *cr,
                                   const terranovaColors            *colors,
                                   const WidgetParameters *params,
                                   const ScrollBarParameters        *scrollbar,
                                   const ScrollBarStepperParameters *stepper,
                                   int x, int y, int width, int height)
{

/* we want "transparent" steppers, therefore leave empty! */

}

static void
terranova_draw_scrollbar_slider (cairo_t *cr,
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

		CairoColor shade1, shade2, shade3, shade4;
		tn_shade_color (&fill, 1.20, &shade1);
		tn_shade_color (&fill, 1.10, &shade2);
		tn_shade_color (&fill, 1.00, &shade3);
		tn_shade_color (&fill, 0.90, &shade4);

		pattern	= cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
		cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
		cairo_rectangle (cr, 0, 0, width, height);
		cairo_set_source (cr, pattern);
		cairo_fill(cr);
		cairo_pattern_destroy (pattern);

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
terranova_draw_statusbar (cairo_t *cr,
                           const terranovaColors          *colors,
                           const WidgetParameters *params,
                           int x, int y, int width, int height)
{
	const CairoColor *dark = &colors->shade[3];
	CairoColor hilight;

	tn_shade_color (dark, 1.4, &hilight);

	cairo_set_line_width  (cr, 1);
	cairo_translate       (cr, x, y+0.5);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	tn_cairo_set_color    (cr, dark);
	cairo_stroke          (cr);

	cairo_translate       (cr, 0, 1);
	cairo_move_to         (cr, 0, 0);
	cairo_line_to         (cr, width, 0);
	tn_cairo_set_color    (cr, &hilight);
	cairo_stroke          (cr);
}

static void
terranova_draw_calendar (cairo_t *cr,
                           const terranovaColors          *colors,
                           const WidgetParameters *params,
                           int x, int y, int width, int height)
{
	CairoColor fill = colors->bg[params->state_type];

	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	cairo_pattern_t *pattern;

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (&fill, 1.20, &shade1);
	tn_shade_color (&fill, 1.10, &shade2);
	tn_shade_color (&fill, 1.00, &shade3);
	tn_shade_color (&fill, 0.90, &shade4);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);
	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle (cr, 0, 0, width, height, radius, params->corners);
	cairo_fill(cr);
	cairo_pattern_destroy (pattern);
	cairo_restore(cr);
}

static void
terranova_draw_menu_frame (cairo_t *cr,
                            const terranovaColors          *colors,
                            const WidgetParameters *params,
                            int x, int y, int width, int height)
{
	const CairoColor *border = &colors->shade[5];
	cairo_translate      (cr, x, y);
	cairo_set_line_width (cr, 1);

	double radius = MIN (params->radius, MIN ((width - 4.0) / 2.0, (height - 4.0) / 2.0));

	tn_cairo_rounded_rectangle (cr, x+0.75, y+0.75, width - 1, height - 1, radius, params->corners);

	terranova_draw_mw_gradient(cr, colors, params, 0, 0, width, height);

	tn_cairo_set_color (cr, border);
	tn_cairo_stroke_rectangle (cr, 0.5, 0.5, width-1, height-1);
}

static void
terranova_draw_tooltip (cairo_t *cr,
                         const terranovaColors          *colors,
                         const WidgetParameters *params,
                         int x, int y, int width, int height)
{
	const CairoColor *stripe_fill = &colors->bg[GTK_STATE_SELECTED];
	const CairoColor *fill = &colors->bg[GTK_STATE_NORMAL];
	CairoColor shadow, hilight, middle;
	cairo_pattern_t *pattern;

	cairo_translate (cr, x, y);

	if (params->tooltip_style == 0)
	{

	cairo_rectangle(cr, 0, 0, width, height);

	tn_shade_color (fill, 1.10, &middle);
	tn_shade_color (fill, 0.90, &shadow);
	tn_shade_color (fill, 1.35, &hilight);

	pattern = cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.25,        hilight.r, hilight.g, hilight.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.5,        middle.r, middle.g, middle.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.75,        shadow.r, shadow.g, shadow.b);
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

static void
terranova_draw_handle (cairo_t *cr,
                        const terranovaColors          *colors,
                        const WidgetParameters          *params,
                        const HandleParameters          *handle,
                        int x, int y, int width, int height)
{
	const CairoColor *fill  = &colors->bg[params->state_type];
	int num_bars = 6; /* shut up gcc warnings */
	int num_columns;

	switch (handle->type)
	{
		case TN_HANDLE_TOOLBAR:
			num_bars    = 4;
			num_columns = 2;
		break;
		case TN_HANDLE_SPLITTER:
			num_bars    = 8;
			num_columns = 2;
		break;
	}

	if (params->prelight)
	{
		cairo_rectangle (cr, x, y, width, height);
		tn_cairo_set_color (cr, fill);
		cairo_fill (cr);
	}

	cairo_translate (cr, x+0.5, y+0.5);

	cairo_set_line_width (cr, 1);

	if (handle->horizontal)
	{
		params->style_functions->draw_gripdots (cr, colors, 0, 0, width, height, num_bars, num_columns, 0.1);
	}
	else
	{
		params->style_functions->draw_gripdots (cr, colors, 0, 0, width, height, num_columns, num_bars, 0.1);
	}
}

static void
terranova_draw_resize_grip (cairo_t *cr,
                             const terranovaColors          *colors,
                             const WidgetParameters *params,
                             const ResizeGripParameters      *grip,
                             int x, int y, int width, int height)
{
	const CairoColor *dark   = &colors->shade[4];
	CairoColor hilight;
	int lx, ly;
	int x_down;
	int y_down;
	int dots;

	tn_shade_color (dark, 0.75, &hilight);

	dots = MIN (width - 2, height - 2) / 2.4;

	cairo_save (cr);

	switch (grip->edge)
	{
		case TN_WINDOW_EDGE_NORTH_EAST:
			x_down = 0;
			y_down = 0;
			cairo_translate (cr, x + width - 3*dots + 2, y + 1);
		break;
		case TN_WINDOW_EDGE_SOUTH_EAST:
			x_down = 0;
			y_down = 1;
			cairo_translate (cr, x + width - 3*dots + 2, y + height - 3*dots + 2);
		break;
		case TN_WINDOW_EDGE_SOUTH_WEST:
			x_down = 1;
			y_down = 1;
			cairo_translate (cr, x + 1, y + height - 3*dots + 2);
		break;
		case TN_WINDOW_EDGE_NORTH_WEST:
			x_down = 1;
			y_down = 0;
			cairo_translate (cr, x + 1, y + 1);
		break;
		default:
			/* Not implemented. */
			return;
	}

	for (lx = 0; lx < dots; lx++) /* horizontally */
	{
		for (ly = 0; ly <= lx; ly++) /* vertically */
		{
			int mx, my;
			mx = x_down * dots + (1 - x_down * 2) * lx - x_down;
			my = y_down * dots + (1 - y_down * 2) * ly - y_down;

			tn_cairo_set_color (cr, &hilight);
			cairo_rectangle (cr, mx*3-3, my*3-3, 2, 2);
			cairo_fill (cr);

			tn_cairo_set_color (cr, &hilight);
			cairo_rectangle (cr, mx*3-3, my*3-3, 2, 2);
			cairo_fill (cr);
		}
	}

	cairo_restore (cr);
}

static void
terranova_draw_radiobutton (cairo_t *cr,
                             const terranovaColors  *colors,
                             const WidgetParameters *params,
                             const CheckboxParameters *checkbox,
                             int x, int y, int width, int height)
{
	const CairoColor *border;
	const CairoColor *dot;
	CairoColor dark_dot;
	CairoColor shadow;
	CairoColor highlight;
	cairo_pattern_t *pt;
	gboolean inconsistent;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;

	if (params->disabled)
	{
		border = &colors->shade[5];
		dot    = &colors->bg[GTK_STATE_INSENSITIVE];
		dark_dot = colors->bg[GTK_STATE_INSENSITIVE];
	}
	else
	{
		border = &colors->shade[6];
		dot    = &colors->bg[GTK_STATE_SELECTED];
		tn_shade_color(&colors->bg[GTK_STATE_SELECTED], 0.50, &dark_dot);
	}

	tn_shade_color (&params->parentbg, 0.9, &shadow);
	tn_shade_color (&params->parentbg, 1.1, &highlight);

	pt = cairo_pattern_create_linear (0, 0, 13, 13);
	cairo_pattern_add_color_stop_rgb (pt, 1.0, highlight.r, highlight.g, highlight.b);

	cairo_translate (cr, x, y);

	cairo_set_line_width (cr, 1);
	cairo_arc       (cr, 7, 7, 6, 0, G_PI*2);
	cairo_set_source (cr, pt);
	cairo_stroke (cr);
	cairo_pattern_destroy (pt);

	cairo_set_line_width (cr, 1);

	cairo_arc       (cr, 7, 7, 5.75, 0, G_PI*2);

	if (!params->disabled)
	{
		tn_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}

	tn_cairo_set_color (cr, border);
	cairo_stroke (cr);

	if (draw_bullet)
	{
		if (inconsistent)
		{
			cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
			cairo_set_line_width (cr, 4);

			cairo_move_to(cr, 5, 7);
			cairo_line_to(cr, 9, 7);

			tn_cairo_set_color (cr, &dark_dot);
			cairo_stroke (cr);
		}
		else
		{
			cairo_arc (cr, 7, 7, 3, 0, G_PI*2);
			tn_cairo_set_color (cr, dot);
			cairo_fill (cr);

			cairo_arc (cr, 6, 6, 1, 0, G_PI*2);
			cairo_set_source_rgba (cr, highlight.r, highlight.g, highlight.b, 0.5);
			cairo_fill (cr);
		}
	}
}

static void
terranova_draw_checkbox (cairo_t *cr,
                          const terranovaColors  *colors,
                          const WidgetParameters *params,
                          const CheckboxParameters *checkbox,
                          int x, int y, int width, int height)
{
	const CairoColor *border;
	const CairoColor *dot;
	CairoColor dark_dot;
	gboolean inconsistent = FALSE;
	gboolean draw_bullet = (checkbox->shadow_type == GTK_SHADOW_IN);

	inconsistent = (checkbox->shadow_type == GTK_SHADOW_ETCHED_IN);
	draw_bullet |= inconsistent;

	if (params->disabled)
	{
		border = &colors->shade[5];
		dot    = &colors->bg[GTK_STATE_INSENSITIVE];
		dark_dot = colors->bg[GTK_STATE_INSENSITIVE];
	}
	else
	{
		border = &colors->shade[6];
		dot    = &colors->bg[GTK_STATE_SELECTED];
		tn_shade_color(&colors->bg[GTK_STATE_SELECTED], 0.50, &dark_dot);
	}

	cairo_translate (cr, x, y);

	if (params->xthickness > 2 && params->ythickness > 2)
	{
		params->style_functions->draw_inset (cr, &params->parentbg, 0.5, 0.5, width-1, height-1, 1, CR_CORNER_ALL);

		tn_cairo_rounded_rectangle (cr, 1.5, 1.5, width-3, height-3, (params->radius > 0)? 1 : 0, CR_CORNER_ALL);
	}
	else
	{
		tn_cairo_rounded_rectangle (cr, 0.5, 0.5, width-1, height-1, (params->radius > 0)? 1 : 0, CR_CORNER_ALL);
	}

	if (!params->disabled)
	{
		tn_cairo_set_color (cr, &colors->base[0]);
		cairo_fill_preserve (cr);
	}

	tn_cairo_set_color (cr, border);
	cairo_stroke (cr);

	if (draw_bullet)
	{
		if (inconsistent)
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, height*0.5);
			cairo_line_to (cr, width-3, height*0.5);

			tn_cairo_set_color (cr, &dark_dot);
			cairo_stroke (cr);

		}
		else if (params->indicator_style == 2)
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, 3);
			cairo_line_to (cr, width-3, height-3);

			cairo_move_to (cr, 3, height-3);
			cairo_line_to (cr, width-3, 3);

			tn_cairo_set_color (cr, dot);
			cairo_stroke (cr);
		}
		else if (params->indicator_style == 1)
		{
			cairo_set_line_width (cr, 2.0);
			cairo_move_to (cr, 3, height*0.5);
			cairo_line_to (cr, width-3, height*0.5);

			cairo_move_to (cr, width/2+0.5, 0+3);
			cairo_line_to (cr, width/2+0.5, height-3);

			tn_cairo_set_color (cr, dot);
			cairo_stroke (cr);
		}
		else if (params->indicator_style == 0)
		{
			cairo_translate (cr, -4, 0);
			cairo_move_to (cr, 5, 10);
			cairo_rel_line_to (cr, 5, 2);
			cairo_rel_curve_to (cr, 1.4, -7.0, -0.6, -1.95, 5.0, -12.0);
			cairo_rel_curve_to (cr, -5, 5, -5, 5, -6.0, 9.3);
			cairo_rel_line_to (cr, -2.7, -2.0);

			tn_cairo_set_color (cr, dot);
			cairo_fill (cr);
		}
	}
}

static void
terranova_draw_normal_arrow (cairo_t *cr, const CairoColor *color,
                              double x, double y, double width, double height)
{
	double arrow_width;
	double arrow_height;
	double line_width_2;

	cairo_save (cr);

	arrow_width = MIN (height * 2.0 + MAX (1.0, ceil (height * 2.0 / 6.0 * 2.0) / 2.0) / 2.0, width);
	line_width_2 = MAX (1.0, ceil (arrow_width / 6.0 * 2.0) / 2.0) / 2.0;
	arrow_height = arrow_width / 2.0 + line_width_2;

	cairo_translate (cr, x, y - arrow_height / 2.0);

	cairo_move_to (cr, -arrow_width / 2.0, line_width_2);
	cairo_line_to (cr, -arrow_width / 2.0 + line_width_2, 0);
	cairo_arc_negative (cr, 0, arrow_height - 4*line_width_2 - 2*line_width_2 * sqrt(2), 2*line_width_2, G_PI_2 + G_PI_4, G_PI_4);
	cairo_line_to (cr, arrow_width / 2.0 - line_width_2, 0);
	cairo_line_to (cr, arrow_width / 2.0, line_width_2);
	cairo_line_to (cr, 0, arrow_height);
	cairo_close_path (cr);

	tn_cairo_set_color (cr, color);
	cairo_fill (cr);

	cairo_restore (cr);
}

static void
terranova_draw_combo_arrow (cairo_t *cr, const CairoColor *color,
                             double x, double y, double width, double height)
{
	double arrow_width = MIN (height * 2 / 3.0, width);
	double arrow_height = arrow_width / 1.5;
	double gap_size = arrow_height;

	cairo_save (cr);
	cairo_translate (cr, x, y - (arrow_height + gap_size) / 2.0);
	terranova_draw_normal_arrow (cr, color, 0, 0, arrow_width, arrow_height);
	cairo_restore (cr);
	terranova_draw_normal_arrow (cr, color, x, y + (arrow_height + gap_size) / 2.0, arrow_width, arrow_height);
}

static void
_terranova_draw_arrow (cairo_t *cr, const CairoColor *color,
                        terranovaDirection dir, terranovaArrowType type,
                        double x, double y, double width, double height)
{
	double rotate;

	if (dir == TN_DIRECTION_LEFT)
		rotate = G_PI*1.5;
	else if (dir == TN_DIRECTION_RIGHT)
		rotate = G_PI*0.5;
	else if (dir == TN_DIRECTION_UP)
		rotate = G_PI;
	else if (dir == TN_DIRECTION_DOWN)
		rotate = 0;
	else
		return;

	if (type == TN_ARROW_NORMAL)
	{
		cairo_translate (cr, x, y);
		cairo_rotate (cr, -rotate);
		terranova_draw_normal_arrow (cr, color, 0, 0, width, height);
	}
	else if (type == TN_ARROW_COMBO)
	{
		cairo_translate (cr, x, y);
		terranova_draw_combo_arrow (cr, color, 0, 0, width, height);
	}
}

static void
terranova_draw_arrow (cairo_t *cr,
                       const terranovaColors *colors,
                       const WidgetParameters *params,
                       const ArrowParameters  *arrow,
                       int x, int y, int width, int height)
{
	const CairoColor *color = &colors->fg[params->state_type];

	gdouble tx, ty;

	tx = x + width/2.0;
	ty = y + height/2.0;

	if (params->disabled)
	{
		_terranova_draw_arrow (cr, &colors->shade[0],
		                        arrow->direction, arrow->type,
		                        tx+0.5, ty+0.5, width * 0.5, height * 0.5);
	}

	cairo_identity_matrix (cr);

	_terranova_draw_arrow (cr, color, arrow->direction, arrow->type,
	                        tx, ty, width * 0.5, height * 0.5);

}

static void
terranova_draw_icon_view_item (cairo_t *cr,
                       const terranovaColors *colors,
                       const WidgetParameters *params,
                       int x, int y, int width, int height)
{
	const CairoColor *fill = &colors->bg[GTK_STATE_SELECTED];

	cairo_save (cr);

	cairo_translate (cr, x, y);

	cairo_pattern_t *pattern;

	CairoColor shade1, shade2, shade3, shade4;
	tn_shade_color (fill, 1.20, &shade1);
	tn_shade_color (fill, 1.10, &shade2);
	tn_shade_color (fill, 1.00, &shade3);
	tn_shade_color (fill, 0.90, &shade4);

	pattern	= cairo_pattern_create_linear (0, 0, 0, height);
	cairo_pattern_add_color_stop_rgb (pattern, 0.00, shade1.r, shade1.g, shade1.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade2.r, shade2.g, shade2.b);
	cairo_pattern_add_color_stop_rgb (pattern, 0.50, shade3.r, shade3.g, shade3.b);
	cairo_pattern_add_color_stop_rgb (pattern, 1.00, shade4.r, shade4.g, shade4.b);

	cairo_set_source (cr, pattern);
	tn_cairo_rounded_rectangle  (cr, 0, 0, width, height, params->radius, CR_CORNER_ALL);

	cairo_fill (cr);
	cairo_pattern_destroy (pattern);
	cairo_restore (cr);
}

static void
terranova_draw_separator (cairo_t *cr,
                           const terranovaColors     *colors,
                           const WidgetParameters *params,
                           const SeparatorParameters  *separator,
                           int x, int y, int width, int height)
{

/* We don't want separators to be drawn, therefore leave emtpy */

}

void
terranova_register_style_clarius (terranovaStyleFunctions *functions)
{
	g_assert (functions);

	functions->draw_button             = terranova_draw_button;
	functions->draw_scale_trough       = terranova_draw_scale_trough;
	functions->draw_progressbar_trough = terranova_draw_progressbar_trough;
	functions->draw_progressbar_fill   = terranova_draw_progressbar_fill;
	functions->draw_slider_button      = terranova_draw_slider_button;
	functions->draw_entry              = terranova_draw_entry;
	functions->draw_mw_gradient        = terranova_draw_mw_gradient;
	functions->draw_spinbutton         = terranova_draw_spinbutton;
	functions->draw_spinbutton_down    = terranova_draw_spinbutton_down;
	functions->draw_optionmenu         = terranova_draw_optionmenu;
	functions->draw_inset              = terranova_draw_inset;
	functions->draw_menubar            = terranova_draw_menubar;
	functions->draw_tab                = terranova_draw_tab;
	functions->draw_frame              = terranova_draw_frame;
	functions->draw_separator          = terranova_draw_separator;
	functions->draw_menu_item_separator = terranova_draw_menu_item_separator;
	functions->draw_list_view_header   = terranova_draw_list_view_header;
	functions->draw_toolbar            = terranova_draw_toolbar;
	functions->draw_menuitem           = terranova_draw_menuitem;
	functions->draw_menubaritem        = terranova_draw_menubaritem;
	functions->draw_selected_cell      = terranova_draw_selected_cell;
	functions->draw_scrollbar_stepper  = terranova_draw_scrollbar_stepper;
	functions->draw_scrollbar_slider   = terranova_draw_scrollbar_slider;
	functions->draw_scrollbar_trough   = terranova_draw_scrollbar_trough;
	functions->draw_statusbar          = terranova_draw_statusbar;
	functions->draw_menu_frame         = terranova_draw_menu_frame;
	functions->draw_tooltip            = terranova_draw_tooltip;
	functions->draw_handle             = terranova_draw_handle;
	functions->draw_resize_grip        = terranova_draw_resize_grip;
	functions->draw_arrow              = terranova_draw_arrow;
	functions->draw_checkbox           = terranova_draw_checkbox;
	functions->draw_radiobutton        = terranova_draw_radiobutton;
	functions->draw_shadow             = terranova_draw_shadow;
	functions->draw_slider             = terranova_draw_slider;
	functions->draw_gripdots           = terranova_draw_gripdots;
	functions->draw_icon_view_item     = terranova_draw_icon_view_item;
	functions->draw_calendar           = terranova_draw_calendar;
	functions->draw_unselected_cell    = terranova_draw_unselected_cell;
}
