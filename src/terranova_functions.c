/* Terranova Engine
 * Copyright (C) 2010 Christopher Bratusek
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

#include "ge-support.h"
#include "general-support.h"
#include "support.h"
#include <cairo.h>

/*
 * meta func
 */

int
rand()
{
	return 0;
}

/*
 * Functions for drawing the random strips on buttons and tabs
 *
 * engine-options used: random_opacity
 *
 * original function taken from DynDyn engine
 *
 */

void
terranova_draw_claws(cairo_t *cr, int x1, int y1, int x2, int y2)
{
	int d=(x1-x2)/2;
	x2 = x2*1.5;
	y2 = y2*1.5;
	cairo_move_to(cr, x1+1,y1+1);
	cairo_curve_to(cr,x1+d,y1+1,x2,y2,x2,y2);
	cairo_curve_to(cr,x2,y2,x1+d+d/2,y1+1,x1+d,y1);
	cairo_close_path (cr);
	cairo_fill(cr);
}

void
terranova_draw_random_claws(cairo_t *cr,
			const terranovaColors *colors,
			const WidgetParameters *params,
			int xoffset, int yoffset,
			int x, int y, int width, int height, int count)
{

	const CairoColor *random_color = &colors->text[GTK_STATE_NORMAL];
	int i;

	cairo_set_source_rgba(cr, random_color->r, random_color->g, random_color->b, params->random_opacity);

	for (i=0;i<count;i++){
		int x1,x2;
		static GHashTable *ght=NULL;
		if (ght==NULL) ght=g_hash_table_new_full(g_str_hash,g_str_equal,NULL,NULL);

		gchar key[4]={(char)x,(char)y,(char)i,0};
		gchar *obj=g_hash_table_lookup(ght,key);
		if (obj==NULL){
			x1=rand()%width;
			x2=rand()%(width/5)-width/10;
			gchar obj[]={x1,x2,0};
			g_hash_table_insert(ght,g_strdup(key),g_strdup(obj));
		}
		else
		{
			x1=obj[0];
			x2=obj[1];
		}
		int d;
		d=x2*2;
		terranova_draw_claws(cr,x1+xoffset,yoffset,x1+d+xoffset,height/1.75+yoffset);
	}

	cairo_restore (cr);
}

/*
 * Functions for drawing the random bubbles on buttons and tabs
 *
 * engine-options used: random_opacity
 *
 * original function taken from DynDyn engine
 *
 */

void
terranova_draw_bubbles(cairo_t *cr, int x1, int y1, int x2, int y2,
				 int circle_spacing, int circle_radius)
{
	cairo_move_to (cr, x1, y1);
	cairo_arc (cr, x1, y1, circle_radius, 0, G_PI*2);
	x1 += 2*circle_radius*circle_spacing;
	y1 += 2*circle_radius*circle_spacing;
	cairo_close_path (cr);
	cairo_fill (cr);
}

void
terranova_draw_random_bubbles(cairo_t *cr,
				const terranovaColors *colors,
				const WidgetParameters *params, int xoffset, int yoffset, int x,
				int y, int width, int height, int count,
				int circle_spacing, int circle_radius)
{

	const CairoColor *random_color = &colors->text[GTK_STATE_NORMAL];
	int i;

	cairo_set_source_rgba(cr, random_color->r, random_color->g, random_color->b, params->random_opacity);

	for (i=0;i<count;i++){
		int x1,x2;
		static GHashTable *ght=NULL;
		if (ght==NULL) ght=g_hash_table_new_full(g_str_hash,g_str_equal,NULL,NULL);

		gchar key[4]={(char)x,(char)y,(char)i,0};
		gchar *obj=g_hash_table_lookup(ght,key);
		if (obj==NULL){
			x1=rand()%width;
			x2=rand()%(width/5)-width/10;
			gchar obj[]={x1,x2,0};
			g_hash_table_insert(ght,g_strdup(key),g_strdup(obj));
		}
		else
		{
			x1=obj[0];
			x2=obj[1];
		}
		int d=x2*2;
		terranova_draw_bubbles(cr,x1/xoffset,x2/yoffset,x1+d-xoffset,height*0.80-yoffset, circle_spacing, circle_radius);
	}

	cairo_restore (cr);
}

/*
 * mirror rotate cairo_t
 */

void
terranova_mirror_rotate (cairo_t *cr, double radius, double x, double y,
				boolean mirror_horizontally, boolean mirror_vertically)
{
	cairo_matrix_t matrix_rotate;
	cairo_matrix_t matrix_mirror;
	cairo_matrix_t matrix_result;

	double r_cos = cos(radius);
	double r_sin = sin(radius);

	cairo_matrix_init (&matrix_rotate, r_cos,
	                                   r_sin,
	                                   r_sin,
	                                   r_cos,
	                                   x, y);

	cairo_matrix_init (&matrix_mirror, mirror_horizontally ? -1 : 1,
	                                   0,
	                                   0,
	                                   mirror_vertically ? -1 : 1,
								  0, 0);

	cairo_matrix_multiply (&matrix_result, &matrix_mirror, &matrix_rotate);

	cairo_set_matrix (cr, &matrix_result);
}

/*
 * function for drawing border-gradient
 */

void
terranova_draw_border_gradient (cairo_t *cr, const CairoColor *color, double hilight,
					int width, int height)
{
	cairo_pattern_t *pattern;


	pattern	= cairo_pattern_create_linear (0, 0, width, height);
	cairo_pattern_add_color_stop_rgba (pattern, 0.0, color->r, color->g, color->b, 1.0);
	cairo_pattern_add_color_stop_rgba (pattern, 1.0, color->r, color->g, color->b, 0.5);

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);
}

/*
 * function for drawing highlight and shade
 */

void
terranova_draw_highlight_and_shade (cairo_t *cr, const terranovaColors *colors,
                                     const ShadowParameters *params,
                                     int width, int height, gdouble radius)
{
	CairoColor hilight;
	CairoColor shadow;
	uint8 corners = params->corners;
	double x = 1.0;
	double y = 1.0;

	tn_shade_color (&colors->bg[GTK_STATE_NORMAL], 1.15, &hilight);
	tn_shade_color (&colors->bg[GTK_STATE_NORMAL], 0.90, &shadow);

	width  -= 3;
	height -= 3;

	cairo_save (cr);

	/* Top/Left highlight */
	if (corners & CR_CORNER_BOTTOMLEFT)
		cairo_move_to (cr, x, y+height-radius);
	else
		cairo_move_to (cr, x, y+height);

	tn_cairo_rounded_corner (cr, x, y, radius, corners & CR_CORNER_TOPLEFT);

	if (corners & CR_CORNER_TOPRIGHT)
		cairo_line_to (cr, x+width-radius, y);
	else
		cairo_line_to (cr, x+width, y);

	if (params->shadow & TN_SHADOW_OUT)
		tn_cairo_set_color (cr, &hilight);
	else
		tn_cairo_set_color (cr, &shadow);

	cairo_stroke (cr);

	/* Bottom/Right highlight -- this includes the corners */
	cairo_move_to (cr, x+width-radius, y); /* topright and by radius to the left */
	tn_cairo_rounded_corner (cr, x+width, y, radius, corners & CR_CORNER_TOPRIGHT);
	tn_cairo_rounded_corner (cr, x+width, y+height, radius, corners & CR_CORNER_BOTTOMRIGHT);
	tn_cairo_rounded_corner (cr, x, y+height, radius, corners & CR_CORNER_BOTTOMLEFT);

	if (params->shadow & TN_SHADOW_OUT)
		tn_cairo_set_color (cr, &shadow);
	else
		tn_cairo_set_color (cr, &hilight);

	cairo_stroke (cr);

	cairo_restore (cr);
}

/*
 * function for changing the direction of gradients
 */

typedef struct {

	int width;
	int height;

} GradientPositions;

GradientPositions
terranova_get_gradient_position(const WidgetParameters *params,
					int width, int height)
{

	GradientPositions ret;

	if (params->button_gradient == 1)
	{
		ret.width = width;
		ret.height = 0;
	}
	else
	{
		ret.width = 0;
		ret.height = height;
	}

	return ret;

}

/*
 * function for drawing some stuff on buttons
 *
 * engine-options used: button_effect
 *
 */

void
terranova_draw_button_effect(cairo_t *cr, const WidgetParameters *params,
				const terranovaColors *colors, int width, int height)
{

	const CairoColor *stripe_fill = &colors->bg[GTK_STATE_SELECTED];
	CairoColor hilight, middle, shadow;
	cairo_pattern_t *pattern;

	if (params->button_effect == 1)
	{

		tn_shade_color (stripe_fill, 1.10, &middle);
		tn_shade_color (stripe_fill, 0.90, &shadow);
		tn_shade_color (stripe_fill, 1.35, &hilight);

		pattern = cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.25, hilight.r, hilight.g, hilight.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.05, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.75, shadow.r, shadow.g, shadow.b);
		cairo_set_source (cr, pattern);
		cairo_pattern_destroy (pattern);

		cairo_move_to  (cr, 1, 1);
		cairo_line_to  (cr, width, 1);
		cairo_curve_to (cr, width, 1, 1, 1, 1, height);
		cairo_close_path(cr);
		cairo_fill(cr);

	}
	else if (params->button_effect == 2)
	{

		tn_shade_color (stripe_fill, 1.10, &middle);
		tn_shade_color (stripe_fill, 0.90, &shadow);
		tn_shade_color (stripe_fill, 1.35, &hilight);

		pattern = cairo_pattern_create_linear (0, 0, 0, height);
		cairo_pattern_add_color_stop_rgb (pattern, 0.25, hilight.r, hilight.g, hilight.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.05, middle.r, middle.g, middle.b);
		cairo_pattern_add_color_stop_rgb (pattern, 0.75, shadow.r, shadow.g, shadow.b);
		cairo_set_source (cr, pattern);
		cairo_pattern_destroy (pattern);

		cairo_move_to  (cr, 3, height-20);
		cairo_line_to  (cr, 3, height-3);
		cairo_line_to  (cr, 20, height-3);
		cairo_line_to  (cr, 8, height-9);
		cairo_line_to  (cr, 3, height-20);
		cairo_close_path(cr);
		cairo_fill(cr);

		cairo_move_to (cr, width-20, 3);
		cairo_line_to (cr, width-3, 3);
		cairo_line_to (cr, width-3, 20);
		cairo_line_to (cr, width-8, 9);
		cairo_line_to (cr, width-20, 3);
		cairo_close_path(cr);
		cairo_fill(cr);
	}

}

/*
 * another function for drawing some stuff on buttons
 *
 * engine-options used: grip_style grip_opacity
 *
 */

void
terranova_draw_button_grip(cairo_t *cr, const WidgetParameters *params,
				const terranovaColors *colors, int width, int height,
				int xoffset, int yoffset, double radius)
{

	if (params->grip_style == 0)
	{
		if ((params->active && !params->disabled) || params->prelight)
		{

			cairo_set_line_width (cr, 2.0);

			const CairoColor *pre_color = &colors->bg[GTK_STATE_SELECTED];

			if (params->active)
			{
				cairo_set_source_rgba(cr, pre_color->r, pre_color->g, pre_color->b, 	params->grip_opacity*2);
			}
			else if (params->prelight)
			{
				if (params->grip_opacity >= 0.8)
				{
					cairo_set_source_rgba(cr, pre_color->r, pre_color->g, pre_color->b, params->grip_opacity/2);
				}
				else
				{
					cairo_set_source_rgba(cr, pre_color->r, pre_color->g, pre_color->b, params->grip_opacity);
				}
			}

			tn_cairo_rounded_rectangle (cr, xoffset, yoffset, width-(xoffset*2),
						height-(yoffset*2), radius, params->corners);
			cairo_stroke (cr);

		}
	}
	else if (params->grip_style == 1 && ! params->button_effect == 1)
	{
		if (params->active && !params->disabled)
		{
			const CairoColor *grip_color = &colors->bg[GTK_STATE_SELECTED];
			cairo_set_source_rgba(cr, grip_color->r, grip_color->g, grip_color->b, params->grip_opacity*2);
		}
		else if (params->prelight)
		{
			const CairoColor *grip_color = &colors->bg[GTK_STATE_SELECTED];
			if (params->grip_opacity >= 0.8)
				cairo_set_source_rgba(cr, grip_color->r, grip_color->g, grip_color->b, params->grip_opacity/2);
			else
				cairo_set_source_rgba(cr, grip_color->r, grip_color->g, grip_color->b, params->grip_opacity);
		}
		else
		{
			const CairoColor *grip_color = &colors->text[GTK_STATE_NORMAL];
			cairo_set_source_rgba(cr, grip_color->r, grip_color->g, grip_color->b, params->grip_opacity);
		}

		if (params->grip_pos == 0)
		{
			cairo_move_to (cr, width-6, height-6);
			cairo_line_to (cr, width-13, height-6);
			cairo_line_to (cr, width-6, height-13);
			cairo_close_path (cr);
			cairo_fill(cr);
		}
		else if (params->grip_pos == 1)
		{
			cairo_move_to (cr, 6, height-6);
			cairo_line_to (cr, 13, height-6);
			cairo_line_to (cr, 6, height-13);
			cairo_close_path (cr);
			cairo_fill(cr);
		}
		else if (params->grip_pos == 2)
		{
			cairo_move_to (cr, 6, 6);
			cairo_line_to (cr, 13, 6);
			cairo_line_to (cr, 6, 13);
			cairo_close_path (cr);
			cairo_fill(cr);
		}
		else if (params->grip_pos == 3)
		{
			cairo_move_to (cr, width-6, 6);
			cairo_line_to (cr, width-13, 6);
			cairo_line_to (cr, width-6, 13);
			cairo_close_path (cr);
			cairo_fill(cr);
		}
	}


}

/*
 * Function for drawing stripes on fills
 */

void
terranova_draw_fill_diagonal_stripes(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset)
{

	double tile_pos;
	int    x_step;
	double stroke_width;
	double arrow_width;

	tile_pos = 0;
	stroke_width = height*1.25;
	x_step = (((float)stroke_width/10)*offset);
	arrow_width = stroke_width/2;

	while (tile_pos <= width+x_step)
	{
		cairo_move_to (cr, stroke_width/2-x_step, 0);
		cairo_line_to (cr, stroke_width-x_step,   0);
		cairo_line_to (cr, stroke_width/2-x_step, height);
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

	cairo_rectangle (cr, 0.5, 0.5, width-2, height-1);
	cairo_stroke (cr);

	cairo_reset_clip (cr);

}

/*
 * Function for drawing stripes on fills
 */

void
terranova_draw_fill_vertical_stripes(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset)
{

	double tile_pos;
	int    x_step;
	double stroke_width;
	double arrow_width;

	tile_pos = 0;
	stroke_width = height*1.25;
	x_step = (((float)stroke_width/10)*offset);
	arrow_width = stroke_width/2;

	while (tile_pos <= width+x_step)
	{
		cairo_move_to (cr, stroke_width/2-x_step, 0);
		cairo_line_to (cr, stroke_width-x_step,   0);
		cairo_line_to (cr, stroke_width-x_step, height);
		cairo_line_to (cr, stroke_width/2-x_step, height);

		cairo_translate (cr, stroke_width, 0);
		tile_pos += stroke_width;
	}

	cairo_set_source_rgba (cr, colors->spot[2].r,
	                           colors->spot[2].g,
	                           colors->spot[2].b,
	                           0.15);

	cairo_fill (cr);

	cairo_restore (cr);

	cairo_rectangle (cr, 0.5, 0.5, width-2, height-1);
	cairo_stroke (cr);

	cairo_reset_clip (cr);

}

/*
 * Function for drawing arrows on fills
 */

void
terranova_draw_fill_arrows(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset)
{
	double tile_pos;
	double stroke_width;
	double arrow_width;
	int    x_step;

	tile_pos = 0;
	stroke_width = height*1.25;
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

	cairo_rectangle (cr, 0.5, 0.5, width-2, height-1);
	cairo_stroke (cr);

	cairo_reset_clip (cr);

}
