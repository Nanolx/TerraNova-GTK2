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
 */

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <string.h>

#include "ge-support.h"
#include "terranova_style.h"
#include "terranova_rc_style.h"
#include "terranova_draw.h"
#include "terranova_functions.h"
#include "support.h"

#include "../config.h"

/* #define DEBUG 1 */

#define DETAIL(xx)   ((detail) && (!strcmp(xx, detail)))
#define CHECK_HINT(xx) (tn_check_hint ((xx), TERRANOVA_RC_STYLE ((style)->rc_style)->hint, widget))

#define DRAW_ARGS    GtkStyle       *style, \
                     GdkWindow      *window, \
                     GtkStateType    state_type, \
                     GtkShadowType   shadow_type, \
                     GdkRectangle   *area, \
                     GtkWidget      *widget, \
                     const gchar    *detail, \
                     gint            x, \
                     gint            y, \
                     gint            width, \
                     gint            height

#include "animation.h"

#define STYLE_FUNCTION(function) (terranova_style_class->style_functions[TERRANOVA_STYLE (style)->style].function)

static terranovaStyleClass *terranova_style_class;
static GtkStyleClass *terranova_parent_class;

static void
terranova_set_widget_parameters (const GtkWidget      *widget,
                                  const GtkStyle       *style,
                                  GtkStateType          state_type,
                                  WidgetParameters     *params)
{
	params->style_functions = &(terranova_style_class->style_functions[TERRANOVA_STYLE (style)->style]);

	if (widget && TN_IS_ENTRY (widget))
		state_type = gtk_widget_get_state (widget);

	params->active        = (state_type == GTK_STATE_ACTIVE);
	params->prelight      = (state_type == GTK_STATE_PRELIGHT);
	params->disabled      = (state_type == GTK_STATE_INSENSITIVE);
	params->state_type    = (terranovaStateType)state_type;
	params->corners       = CR_CORNER_ALL;
	params->ltr           = tn_widget_is_ltr ((GtkWidget*)widget);
	params->focus         = widget && gtk_widget_has_focus (widget);
	params->is_default    = widget && GE_WIDGET_HAS_DEFAULT (widget);
	params->enable_shadow = FALSE;
	params->radius        = TERRANOVA_STYLE (style)->radius;
	params->random_effect = TERRANOVA_STYLE (style)->random_effect;
	params->random_opacity = TERRANOVA_STYLE (style)->random_opacity;
	params->mw_gradient   = TERRANOVA_STYLE (style)->mw_gradient;
	params->scale_trough_size = TERRANOVA_STYLE (style)->scale_trough_size;
	params->tooltip_style = TERRANOVA_STYLE (style)->tooltip_style;
	params->xthickness    = style->xthickness;
	params->ythickness    = style->ythickness;
	params->parentbg      = TERRANOVA_STYLE (style)->colors.bg[state_type];
	terranova_get_parent_bg (widget, &params->parentbg);
	params->grip_opacity  = TERRANOVA_STYLE (style)->grip_opacity;
	params->grip_style    = TERRANOVA_STYLE (style)->grip_style;
	params->trough_visible = TERRANOVA_STYLE (style)->trough_visible;
	params->indicator_style = TERRANOVA_STYLE (style)->indicator_style;
	params->trough_style  = TERRANOVA_STYLE (style)->trough_style;
	params->button_effect  = TERRANOVA_STYLE (style)->button_effect;
	params->button_gradient = TERRANOVA_STYLE (style)->button_gradient;
	params->slider_style = TERRANOVA_STYLE (style)->slider_style;
	params->grip_pos     = TERRANOVA_STYLE (style)->grip_pos;
	params->draw_unselected = TERRANOVA_STYLE (style)->draw_unselected;
	params->menuitemstyle = TERRANOVA_STYLE (style)->menuitemstyle;
}

static void
terranova_style_draw_flat_box (DRAW_ARGS)
{
	if (detail &&
	    state_type == GTK_STATE_SELECTED && (
	    !strncmp ("cell_even", detail, 9) ||
	    !strncmp ("cell_odd", detail, 8)))
	{
		WidgetParameters params;
		terranovaStyle  *terranova_style;
		terranovaColors *colors;
		cairo_t          *cr;

		CHECK_ARGS
		SANITIZE_SIZE

		terranova_style = TERRANOVA_STYLE (style);
		terranova_set_widget_parameters (widget, style, state_type, &params);
		colors = &terranova_style->colors;
		cr = tn_gdk_drawable_to_cairo (window, area);

		STYLE_FUNCTION (draw_selected_cell) (cr, colors, &params, x, y, width, height);

		cairo_destroy (cr);
	}
	else if (detail &&
	    state_type == GTK_STATE_NORMAL && (
	    !strncmp ("cell_even", detail, 9) ||
	    !strncmp ("cell_odd", detail, 8)))
	{

		WidgetParameters params;
		terranovaStyle  *terranova_style;
		terranovaColors *colors;
		cairo_t          *cr;

		CHECK_ARGS
		SANITIZE_SIZE

		terranova_style = TERRANOVA_STYLE (style);
		terranova_set_widget_parameters (widget, style, state_type, &params);
		colors = &terranova_style->colors;
		cr = tn_gdk_drawable_to_cairo (window, area);

		STYLE_FUNCTION (draw_unselected_cell) (cr, colors, &params, x, y, width, height);

		cairo_destroy (cr);

	}
	else if (DETAIL ("icon_view_item"))
	{
		WidgetParameters params;
		terranovaStyle  *terranova_style;
		terranovaColors *colors;
		cairo_t          *cr;

		CHECK_ARGS
		SANITIZE_SIZE

		terranova_style = TERRANOVA_STYLE (style);
		terranova_set_widget_parameters (widget, style, state_type, &params);
		colors = &terranova_style->colors;
		cr = tn_gdk_drawable_to_cairo (window, area);

		STYLE_FUNCTION (draw_icon_view_item) (cr, colors, &params, x, y, width, height);

		cairo_destroy (cr);
	}
	else if (DETAIL ("tooltip"))
	{
		WidgetParameters params;
		terranovaStyle  *terranova_style;
		terranovaColors *colors;
		cairo_t          *cr;

		CHECK_ARGS
		SANITIZE_SIZE

		terranova_style = TERRANOVA_STYLE (style);
		terranova_set_widget_parameters (widget, style, state_type, &params);
		colors = &terranova_style->colors;
		cr = tn_gdk_drawable_to_cairo (window, area);

		STYLE_FUNCTION (draw_tooltip) (cr, colors, &params, x, y, width, height);

		cairo_destroy (cr);
	}
	else if (TN_IS_ENTRY(widget))
	{
		terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
		terranovaColors *colors = &terranova_style->colors;
		cairo_t          *cr     = tn_gdk_drawable_to_cairo (window, area);

		CHECK_ARGS
		SANITIZE_SIZE

		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		STYLE_FUNCTION (draw_entry) (cr, colors, &params, x-2, y-2, width+4, height+4);
	}
	else if (TN_IS_WINDOW(widget))
	{
		terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
		terranovaColors *colors = &terranova_style->colors;
		cairo_t          *cr     = tn_gdk_drawable_to_cairo (window, area);

		CHECK_ARGS
		SANITIZE_SIZE

		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		STYLE_FUNCTION (draw_mw_gradient) (cr, colors, &params, x, y, width, height);
	}
	else
	{
		terranova_parent_class->draw_flat_box (style, window, state_type,
		                                        shadow_type,
		                                        area, widget, detail,
		                                        x, y, width, height);
	}
}

static void
terranova_style_draw_shadow (DRAW_ARGS)
{
	terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	cairo_t          *cr     = tn_gdk_drawable_to_cairo (window, area);

	CHECK_ARGS
	SANITIZE_SIZE

	if ((DETAIL ("entry") && !CHECK_HINT (GE_HINT_TREEVIEW)) ||
	    (DETAIL ("frame") && CHECK_HINT (GE_HINT_COMBOBOX_ENTRY)))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		if (TN_IS_ENTRY(widget))
			params.enable_shadow = TRUE;

		if (!TN_IS_BIN(widget) || !TN_IS_BIN(widget->parent))
		STYLE_FUNCTION (draw_entry) (cr, &terranova_style->colors, &params,
		                             x, y, width, height);
	}
	else if (DETAIL ("frame") && CHECK_HINT (GE_HINT_STATUSBAR))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		gtk_style_apply_default_background (style, window, FALSE, state_type,
		                                    area, x, y, width, height);
		if (shadow_type != GTK_SHADOW_NONE)
			STYLE_FUNCTION (draw_statusbar) (cr, colors, &params,
			                                 x, y, width, height);
	}
	else if (DETAIL ("frame"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		frame.shadow  = shadow_type;
		frame.gap_x   = -1;  /* No gap will be drawn */
		frame.border  = &colors->shade[4];

		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CR_CORNER_NONE;

		if (widget && !g_str_equal ("XfcePanelWindow", gtk_widget_get_name (gtk_widget_get_toplevel (widget))))
			STYLE_FUNCTION(draw_frame) (cr, colors, &params, &frame,
			                            x, y, width, height);
	}
	else if (DETAIL ("scrolled_window") || DETAIL ("viewport") || detail == NULL)
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

	}
	else if (DETAIL ("calendar"))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		/*gtk_style_apply_default_background (style, window, FALSE, state_type,
		                                    area, x, y, width, height);*/

		STYLE_FUNCTION (draw_calendar) (cr, colors, &params, x, y, width, height);
	}
	else
	{
		WidgetParameters params;
		FrameParameters frame;

		frame.shadow = shadow_type;
		frame.gap_x  = -1;
		frame.border = &colors->shade[5];
		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CR_CORNER_ALL;

		STYLE_FUNCTION(draw_frame) (cr, colors, &params, &frame, x, y, width, height);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_box_gap (DRAW_ARGS,
                               GtkPositionType gap_side,
                               gint            gap_x,
                               gint            gap_width)
{
	terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	cairo_t          *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);

	if (DETAIL ("notebook"))
	{
		WidgetParameters params;
		FrameParameters  frame;
		gboolean start, end;

		frame.shadow    = shadow_type;
		frame.gap_side  = gap_side;
		frame.gap_x     = gap_x;
		frame.gap_width = gap_width;
		frame.border    = &colors->shade[5];

		terranova_set_widget_parameters (widget, style, state_type, &params);

		terranova_get_notebook_tab_position (widget, &start, &end);

		params.corners = CR_CORNER_ALL;
		switch (gap_side)
		{
			case GTK_POS_TOP:
				if (tn_widget_is_ltr (widget))
				{
					if (start)
						params.corners ^= CR_CORNER_TOPLEFT;
					if (end)
						params.corners ^= CR_CORNER_TOPRIGHT;
				}
				else
				{
					if (start)
						params.corners ^= CR_CORNER_TOPRIGHT;
					if (end)
						params.corners ^= CR_CORNER_TOPLEFT;
				}
			break;
			case GTK_POS_BOTTOM:
				if (tn_widget_is_ltr (widget))
				{
					if (start)
						params.corners ^= CR_CORNER_BOTTOMLEFT;
					if (end)
						params.corners ^= CR_CORNER_BOTTOMRIGHT;
				}
				else
				{
					if (start)
						params.corners ^= CR_CORNER_BOTTOMRIGHT;
					if (end)
						params.corners ^= CR_CORNER_BOTTOMLEFT;
				}
			break;
			case GTK_POS_LEFT:
				if (start)
					params.corners ^= CR_CORNER_TOPLEFT;
				if (end)
					params.corners ^= CR_CORNER_BOTTOMLEFT;
			break;
			case GTK_POS_RIGHT:
				if (start)
					params.corners ^= CR_CORNER_TOPRIGHT;
				if (end)
					params.corners ^= CR_CORNER_BOTTOMRIGHT;
			break;
		}

		STYLE_FUNCTION(draw_frame) (cr, colors, &params, &frame,
		                            x, y, width, height);
	}
	else
	{
		terranova_parent_class->draw_box_gap (style, window, state_type, shadow_type,
		                                       area, widget, detail,
		                                       x, y, width, height,
		                                       gap_side, gap_x, gap_width);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_extension (DRAW_ARGS, GtkPositionType gap_side)
{
	terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	cairo_t          *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);

	if (DETAIL ("tab"))
	{
		WidgetParameters params;
		TabParameters    tab;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		tab.gap_side = (terranovaGapSide)gap_side;

		switch (gap_side)
		{
			case TN_GAP_TOP:
				params.corners = CR_CORNER_BOTTOMLEFT | CR_CORNER_BOTTOMRIGHT;
				break;
			case TN_GAP_BOTTOM:
				params.corners = CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT;
				break;
			case TN_GAP_LEFT:
				params.corners = CR_CORNER_TOPRIGHT | CR_CORNER_BOTTOMRIGHT;
				break;
			case TN_GAP_RIGHT:
				params.corners = CR_CORNER_TOPLEFT | CR_CORNER_BOTTOMLEFT;
				break;
		}

		STYLE_FUNCTION(draw_tab) (cr, colors, &params, &tab,
		                          x, y, width, height);

		if (params.random_effect == 1)
		{
			terranova_draw_random_claws (cr,&terranova_style->colors,&params,1,1,x,y,width,height,(width*2.75)/50);
		}
		else if (params.random_effect == 2)
		{
			terranova_draw_random_bubbles (cr,&terranova_style->colors,&params,1,1,x*1.5,y/1.5,width-4,height-4,(width*2)/150, 6, 4);
			terranova_draw_random_bubbles (cr,&terranova_style->colors, &params,1,1,x*2,y*2,width-4,height-4,(width*2)/2250, 4, 2);
			terranova_draw_random_bubbles (cr,&terranova_style->colors, &params,1,1,x/1.5,y*1.5,width-4,height-4,(width*2)/225, 8, 6);
		}
	}
	else
	{
		terranova_parent_class->draw_extension (style, window, state_type, shadow_type, area,
		                                         widget, detail, x, y, width, height,
		                                         gap_side);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_handle (DRAW_ARGS, GtkOrientation orientation)
{
	terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	cairo_t          *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);

	if (DETAIL ("handlebox"))
	{
		WidgetParameters params;
		HandleParameters handle;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		handle.type = TN_HANDLE_TOOLBAR;
		handle.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);

		STYLE_FUNCTION(draw_handle) (cr, colors, &params, &handle,
		                             x, y, width, height);
	}
	else if (DETAIL ("paned"))
	{
		WidgetParameters params;
		HandleParameters handle;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		handle.type = TN_HANDLE_SPLITTER;
		handle.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);

		STYLE_FUNCTION(draw_handle) (cr, colors, &params, &handle,
		                             x, y, width, height);
	}
	else
	{
		WidgetParameters params;
		HandleParameters handle;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		handle.type = TN_HANDLE_TOOLBAR;
		handle.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);

		STYLE_FUNCTION(draw_handle) (cr, colors, &params, &handle,
		                             x, y, width, height);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_box (DRAW_ARGS)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	cairo_t *cr;

	cr     = tn_gdk_drawable_to_cairo (window, area);
	colors = &terranova_style->colors;

	CHECK_ARGS
	SANITIZE_SIZE

	if (DETAIL ("menubar"))
	{
		WidgetParameters params;
		MenuBarParameters menubar;
		gboolean horizontal;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		menubar.style = terranova_style->menubarstyle;

		horizontal = height < 2*width;
		/* This is not that great. Ideally we would have a nice vertical menubar. */
		if ((shadow_type != GTK_SHADOW_NONE) && horizontal)
			STYLE_FUNCTION(draw_menubar) (cr, colors, &params, &menubar,
			                              x, y, width, height);
	}
	else if (DETAIL ("button") && CHECK_HINT (GE_HINT_TREEVIEW_HEADER))
	{
		WidgetParameters params;
		ListViewHeaderParameters header;

		gint columns, column_index;
		gboolean resizable = TRUE;

		columns = 3;
		column_index = 1;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		params.corners = CR_CORNER_NONE;

		if (TN_IS_TREE_VIEW (widget->parent))
		{
			terranova_treeview_get_header_index (GTK_TREE_VIEW(widget->parent),
			                                      widget, &column_index, &columns,
			                                      &resizable);
		}
		/*else if (TN_IS_CLIST (widget->parent))
		{
			terranova_clist_get_header_index (GTK_CLIST(widget->parent),
			                                   widget, &column_index, &columns);
		}*/

		header.resizable = resizable;

		if (column_index == 0)
			header.order = params.ltr ? TN_ORDER_FIRST : TN_ORDER_LAST;
		else if (column_index == columns-1)
			header.order = params.ltr ? TN_ORDER_LAST : TN_ORDER_FIRST;
		else
			header.order = TN_ORDER_MIDDLE;

		gtk_style_apply_default_background (style, window, FALSE, state_type, area, x, y, width, height);

		STYLE_FUNCTION(draw_list_view_header) (cr, colors, &params, &header,
		                                       x, y, width, height);
	}
	else if (DETAIL ("button") || DETAIL ("buttondefault"))
	{
		WidgetParameters params;
		ShadowParameters shadow = { CR_CORNER_ALL, TN_SHADOW_NONE } ;
		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.active = shadow_type == GTK_SHADOW_IN;

		if (CHECK_HINT (GE_HINT_COMBOBOX_ENTRY))
		{
			if (params.ltr)
				params.corners = CR_CORNER_TOPRIGHT | CR_CORNER_BOTTOMRIGHT;
			else
				params.corners = CR_CORNER_TOPLEFT | CR_CORNER_BOTTOMLEFT;

			shadow.shadow = TN_SHADOW_IN;

			if (params.ltr)
				x--;
			width++;
		}
		else
		{
			params.corners = CR_CORNER_ALL;
		}

		STYLE_FUNCTION(draw_button) (cr, &terranova_style->colors, &params,
		                             x, y, width+0.75, height+0.75);

		if (params.random_effect == 1)
		{
			terranova_draw_random_claws (cr,&terranova_style->colors,&params,1,1,x,y,width-4,height-4,(width*2.75)/50);
		}
		else if (params.random_effect == 2)
		{
			terranova_draw_random_bubbles (cr,&terranova_style->colors,&params,1,1,x*1.5,y/1.5,width-4,height-4,(width*2)/150, 6, 4);
			terranova_draw_random_bubbles (cr,&terranova_style->colors, &params,1,1,x*2,y*2,width-4,height-4,(width*2)/2250, 4, 2);
			terranova_draw_random_bubbles (cr,&terranova_style->colors, &params,1,1,x/1.5,y*1.5,width-4,height-4,(width*2)/225, 8, 6);
		}

	}
	else if (DETAIL ("spinbutton_up") || DETAIL ("spinbutton_down"))
	{
		if (state_type == GTK_STATE_ACTIVE)
		{
			WidgetParameters params;
			terranova_set_widget_parameters (widget, style, state_type, &params);

			if (style->xthickness == 3)
			{
				width++;
				if (params.ltr)
					x--;
			}

			if (DETAIL ("spinbutton_up"))
			{
				height+=2;
				if (params.ltr)
					params.corners = CR_CORNER_TOPRIGHT;
				else
					params.corners = CR_CORNER_TOPLEFT;
			}
			else
			{
				if (params.ltr)
					params.corners = CR_CORNER_BOTTOMRIGHT;
				else
					params.corners = CR_CORNER_BOTTOMLEFT;
			}

			STYLE_FUNCTION(draw_spinbutton_down) (cr, &terranova_style->colors, &params, x, y, width, height);
		}
	}
	else if (DETAIL ("spinbutton"))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		if (params.ltr)
			params.corners = CR_CORNER_TOPRIGHT | CR_CORNER_BOTTOMRIGHT;
		else
			params.corners = CR_CORNER_TOPLEFT | CR_CORNER_BOTTOMLEFT;

		if (style->xthickness == 3)
		{
			if (params.ltr)
				x--;
			width++;
		}

		STYLE_FUNCTION(draw_spinbutton) (cr, &terranova_style->colors, &params,
		                                 x, y, width, height);
	}
	else if (detail && g_str_has_prefix (detail, "trough") && CHECK_HINT (GE_HINT_SCALE))
	{
		WidgetParameters params;
		SliderParameters slider;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners    = CR_CORNER_NONE;

		slider.lower = DETAIL ("trough-lower");
		slider.fill_level = DETAIL ("trough-fill-level") || DETAIL ("trough-fill-level-full");

		if (CHECK_HINT (GE_HINT_HSCALE))
			slider.horizontal = TRUE;
		else if (CHECK_HINT (GE_HINT_VSCALE))
			slider.horizontal = FALSE;
		else /* Fallback based on the size... */
			slider.horizontal = width >= height;

		STYLE_FUNCTION(draw_scale_trough) (cr, &terranova_style->colors,
		                                   &params, &slider,
		                                   x, y, width, height);
	}
	else if (DETAIL ("trough") && CHECK_HINT (GE_HINT_PROGRESSBAR))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		STYLE_FUNCTION(draw_progressbar_trough) (cr, colors, &params,
		                                         x, y, width, height);
	}
	else if (DETAIL ("trough") && CHECK_HINT (GE_HINT_SCROLLBAR))
	{
		WidgetParameters params;
		ScrollBarParameters scrollbar;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CR_CORNER_ALL;

		scrollbar.horizontal = TRUE;
		scrollbar.junction   = terranova_scrollbar_get_junction (widget);


		if (CHECK_HINT (GE_HINT_HSCROLLBAR))
			scrollbar.horizontal = TRUE;
		else if (CHECK_HINT (GE_HINT_VSCROLLBAR))
			scrollbar.horizontal = FALSE;
		else /* Fallback based on the size  ... */
			scrollbar.horizontal = width >= height;

		/* What is this about? */
		if (scrollbar.horizontal)
		{
			x += 2;
			width -= 4;
		}
		else
		{
			y += 2;
			height -= 4;
		}

		STYLE_FUNCTION(draw_scrollbar_trough) (cr, colors, &params, &scrollbar,
		                                       x, y, width, height);
	}
	else if (DETAIL ("bar"))
	{
		WidgetParameters      params;
		ProgressBarParameters progressbar;
		gdouble               elapsed = 0.0;

		if(terranova_style->animation && TN_IS_PROGRESS_BAR (widget))
			terranova_animation_progressbar_add (widget);

		elapsed = terranova_animation_elapsed (widget);

		terranova_set_widget_parameters (widget, style, state_type, &params);

		if (widget && TN_IS_PROGRESS_BAR (widget))
		{
			progressbar.orientation = gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (widget));
			progressbar.value = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(widget));
		}
		else
		{
			progressbar.orientation = TN_ORIENTATION_LEFT_TO_RIGHT;
			progressbar.value = 0;
			progressbar.pulsing = FALSE;
		}

		if (!params.ltr)
		{
			if (progressbar.orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
				progressbar.orientation = GTK_PROGRESS_RIGHT_TO_LEFT;
			else if (progressbar.orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
				progressbar.orientation = GTK_PROGRESS_LEFT_TO_RIGHT;
		}

		/* Following is a hack to have a larger clip area, the one passed in
		 * does not allow for the shadow. */
		if (area)
		{
			GdkRectangle tmp = *area;
			if (!progressbar.pulsing)
			{
				switch (progressbar.orientation)
				{
					case GTK_PROGRESS_RIGHT_TO_LEFT:
						tmp.x -= 1;
					case GTK_PROGRESS_LEFT_TO_RIGHT:
						tmp.width += 1;
						break;
					case GTK_PROGRESS_BOTTOM_TO_TOP:
						tmp.y -= 1;
					case GTK_PROGRESS_TOP_TO_BOTTOM:
						tmp.height += 1;
						break;
				}
			}
			else
			{
				if (progressbar.orientation == GTK_PROGRESS_RIGHT_TO_LEFT ||
				    progressbar.orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
				{
					tmp.x -= 1;
					tmp.width += 2;
				}
				else
				{
					tmp.y -= 1;
					tmp.height += 2;
				}
			}

			cairo_reset_clip (cr);
			gdk_cairo_rectangle (cr, &tmp);
			cairo_clip (cr);
		}

		progressbar.progressbar_direction = terranova_style->progressbar_direction;
		progressbar.progressbarstyle = terranova_style->progressbarstyle;

		if (progressbar.progressbar_direction == 0)
		{
			STYLE_FUNCTION(draw_progressbar_fill) (cr, colors, &params, &progressbar,
		                                       x, y, width, height,
		                                       10 - (int)(elapsed * 10.0) % 10);
		}
		else if (progressbar.progressbar_direction == 1)
		{
			STYLE_FUNCTION(draw_progressbar_fill) (cr, colors, &params, &progressbar,
		                                       x, y, width, height,
		                                       10 + (int)(elapsed * 10.0) % 10);
		}

	}
	else if (DETAIL ("optionmenu"))
	{
		WidgetParameters params;
		OptionMenuParameters optionmenu;

		GtkRequisition indicator_size;
		GtkBorder indicator_spacing;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		tn_option_menu_get_props (widget, &indicator_size, &indicator_spacing);

		if (tn_widget_is_ltr (widget))
			optionmenu.linepos = width - (indicator_size.width + indicator_spacing.left + indicator_spacing.right) - 1;
		else
			optionmenu.linepos = (indicator_size.width + indicator_spacing.left + indicator_spacing.right) + 1;

		STYLE_FUNCTION(draw_optionmenu) (cr, colors, &params, &optionmenu,
		                                 x, y, width, height);
	}
	else if (DETAIL ("menuitem"))
	{
		WidgetParameters params;
		terranova_set_widget_parameters (widget, style, state_type, &params);

		if (CHECK_HINT (GE_HINT_MENUBAR))
		{
			params.corners = CR_CORNER_TOPLEFT | CR_CORNER_TOPRIGHT;
			height += 1;
			STYLE_FUNCTION(draw_menubaritem) (cr, colors, &params, x, y, width, height);
		}
		else
		{
			params.corners = CR_CORNER_ALL;
			STYLE_FUNCTION(draw_menuitem) (cr, colors, &params, x, y, width, height);
		}
	}
	else if (DETAIL ("hscrollbar") || DETAIL ("vscrollbar")) /* This can't be "stepper" for scrollbars ... */
	{
		WidgetParameters    params;
		ScrollBarParameters scrollbar;
		ScrollBarStepperParameters stepper;
		GdkRectangle this_rectangle;

		this_rectangle.x = x;
		this_rectangle.y = y;
		this_rectangle.width  = width;
		this_rectangle.height = height;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CR_CORNER_NONE;

		scrollbar.style = terranova_style->scrollbarstyle;
		scrollbar.horizontal = TRUE;
		scrollbar.junction   = terranova_scrollbar_get_junction (widget);

		scrollbar.horizontal = DETAIL ("hscrollbar");

		stepper.stepper = terranova_scrollbar_get_stepper (widget, &this_rectangle);

		STYLE_FUNCTION(draw_scrollbar_stepper) (cr, colors, &params, &scrollbar, &stepper,
		                                        x, y, width, height);
	}
	else if (DETAIL ("toolbar") || DETAIL ("handlebox_bin") || DETAIL ("dockitem_bin"))
	{
		WidgetParameters  params;
		ToolbarParameters toolbar;
		gboolean horizontal;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		terranova_set_toolbar_parameters (&toolbar, widget, window, x, y);

		toolbar.style = terranova_style->toolbarstyle;

		if ((DETAIL ("handlebox_bin") || DETAIL ("dockitem_bin")) && TN_IS_BIN (widget))
		{
			GtkWidget* child = gtk_bin_get_child ((GtkBin*) widget);
			/* This is to draw the correct shadow on the handlebox.
			 * We need to draw it here, as otherwise the handle will not get the
			 * background. */
			if (TN_IS_TOOLBAR (child))
				gtk_widget_style_get (child, "shadow-type", &shadow_type, NULL);
		}

		horizontal = height < 2*width;
		/* This is not that great. Ideally we would have a nice vertical toolbar. */
		if ((shadow_type != GTK_SHADOW_NONE) && horizontal)
			STYLE_FUNCTION(draw_toolbar) (cr, colors, &params, &toolbar, x, y, width, height);
	}
	else if (DETAIL ("menu"))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		STYLE_FUNCTION(draw_menu_frame) (cr, colors, &params, x, y, width, height);
	}
	else if (DETAIL ("hseparator") || DETAIL ("vseparator"))
	{
		WidgetParameters params;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		gchar *new_detail = (gchar*) detail;
		/* Draw a normal separator, we just use this because it gives more control
		 * over sizing (currently). */

		/* This isn't nice ... but it seems like the best cleanest way to me right now.
		 * It will get slightly nicer in the future hopefully. */
		if (TN_IS_MENU_ITEM (widget))
			new_detail = "menuitem";

		if (DETAIL ("hseparator"))
		{
			gtk_paint_hline (style, window, state_type, area, widget, new_detail,
			                 x, x + width - 1, y + height/2);
		}
		else
			gtk_paint_vline (style, window, state_type, area, widget, new_detail,
		                 y, y + height - 1, x + width/2);

	}
	else
	{
		terranova_parent_class->draw_box (style, window, state_type, shadow_type, area,
		                                   widget, detail, x, y, width, height);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_slider (DRAW_ARGS, GtkOrientation orientation)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	cairo_t *cr;

	cr     = tn_gdk_drawable_to_cairo (window, area);
	colors = &terranova_style->colors;

	CHECK_ARGS
	SANITIZE_SIZE

	if (DETAIL ("hscale") || DETAIL ("vscale"))
	{
		WidgetParameters params;
		SliderParameters slider;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		slider.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);
		slider.lower = FALSE;
		slider.fill_level = FALSE;

		STYLE_FUNCTION(draw_slider_button) (cr, &terranova_style->colors,
		                                    &params, &slider,
		                                    x, y, width, height);
	}
	else if (DETAIL ("slider"))
	{
		WidgetParameters    params;
		ScrollBarParameters scrollbar;

		terranova_set_widget_parameters (widget, style, state_type, &params);
		params.corners = CR_CORNER_NONE;

		scrollbar.horizontal = (orientation == GTK_ORIENTATION_HORIZONTAL);
		scrollbar.junction   = terranova_scrollbar_get_junction (widget);

		scrollbar.style = terranova_style->scrollbarstyle;


		STYLE_FUNCTION(draw_scrollbar_slider) (cr, colors, &params, &scrollbar,
		                                       x, y, width, height);
	}
	else
	{
		terranova_parent_class->draw_slider (style, window, state_type, shadow_type, area,
		                                      widget, detail, x, y, width, height, orientation);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_option (DRAW_ARGS)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	WidgetParameters params;
	CheckboxParameters checkbox;
	cairo_t *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);
	colors = &terranova_style->colors;

	checkbox.shadow_type = shadow_type;
	checkbox.in_menu = (widget && GTK_IS_MENU(widget->parent));

	terranova_set_widget_parameters (widget, style, state_type, &params);

	STYLE_FUNCTION(draw_radiobutton) (cr, colors, &params, &checkbox, x, y, width, height);

	cairo_destroy (cr);
}

static void
terranova_style_draw_check (DRAW_ARGS)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	WidgetParameters params;
	CheckboxParameters checkbox;
	cairo_t *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);

	terranova_set_widget_parameters (widget, style, state_type, &params);

	params.corners = CR_CORNER_ALL;

	checkbox.shadow_type = shadow_type;
	checkbox.in_cell = DETAIL("cellcheck");

	checkbox.in_menu = (widget && widget->parent && GTK_IS_MENU(widget->parent));

	STYLE_FUNCTION(draw_checkbox) (cr, &terranova_style->colors, &params, &checkbox,
	                               x, y, width, height);

	cairo_destroy (cr);
}

static void
terranova_style_draw_vline (GtkStyle               *style,
                             GdkWindow              *window,
                             GtkStateType            state_type,
                             GdkRectangle           *area,
                             GtkWidget              *widget,
                             const gchar            *detail,
                             gint                    y1,
                             gint                    y2,
                             gint                    x)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	SeparatorParameters separator = { FALSE };
	cairo_t *cr;

	CHECK_ARGS

	colors = &terranova_style->colors;

	cr = tn_gdk_drawable_to_cairo (window, area);

	if (TN_IS_COMBO_BOX (widget->parent->parent->parent))
	{
	}
	else
	{
	STYLE_FUNCTION(draw_separator) (cr, colors, NULL, &separator,
	                                x, y1, 2, y2-y1+1);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_hline (GtkStyle               *style,
                             GdkWindow              *window,
                             GtkStateType            state_type,
                             GdkRectangle           *area,
                             GtkWidget              *widget,
                             const gchar            *detail,
                             gint                    x1,
                             gint                    x2,
                             gint                    y)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	cairo_t *cr;
	SeparatorParameters separator;

	CHECK_ARGS

	colors = &terranova_style->colors;

	cr = tn_gdk_drawable_to_cairo (window, area);

	separator.horizontal = TRUE;

	if (!DETAIL ("menuitem"))
		STYLE_FUNCTION(draw_separator) (cr, colors, NULL, &separator,
		                                x1, y, x2-x1+1, 2);
	else
		STYLE_FUNCTION(draw_menu_item_separator) (cr, colors, NULL, &separator,
		                                          x1, y, x2-x1+1, 2);

	cairo_destroy (cr);
}

static void
terranova_style_draw_shadow_gap (DRAW_ARGS,
                                  GtkPositionType gap_side,
                                  gint            gap_x,
                                  gint            gap_width)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	const terranovaColors *colors;
	cairo_t *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr     = tn_gdk_drawable_to_cairo (window, area);
	colors = &terranova_style->colors;

	if (DETAIL ("frame"))
	{
		WidgetParameters params;
		FrameParameters  frame;

		frame.shadow    = shadow_type;
		frame.gap_side  = gap_side;
		frame.gap_x     = gap_x;
		frame.gap_width = gap_width;
		frame.border    = &colors->shade[5];

		terranova_set_widget_parameters (widget, style, state_type, &params);

		params.corners = CR_CORNER_ALL;

		STYLE_FUNCTION(draw_frame) (cr, colors, &params, &frame,
		                            x, y, width, height);
	}
	else
	{
		terranova_parent_class->draw_shadow_gap (style, window, state_type, shadow_type, area,
		                                          widget, detail, x, y, width, height,
		                                          gap_side, gap_x, gap_width);
	}

	cairo_destroy (cr);
}

static void
terranova_style_draw_resize_grip (GtkStyle       *style,
                                   GdkWindow      *window,
                                   GtkStateType    state_type,
                                   GdkRectangle   *area,
                                   GtkWidget      *widget,
                                   const gchar    *detail,
                                   GdkWindowEdge   edge,
                                   gint            x,
                                   gint            y,
                                   gint            width,
                                   gint            height)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	cairo_t *cr;
	WidgetParameters params;
	ResizeGripParameters grip;

	CHECK_ARGS
	SANITIZE_SIZE

	grip.edge = (terranovaWindowEdge)edge;

	g_return_if_fail (window != NULL);

	cr = tn_gdk_drawable_to_cairo (window, area);

	terranova_set_widget_parameters (widget, style, state_type, &params);

	STYLE_FUNCTION(draw_resize_grip) (cr, colors, &params, &grip,
	                                  x, y, width, height);

	cairo_destroy (cr);
}

static void
terranova_style_draw_tab (DRAW_ARGS)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	WidgetParameters params;
	ArrowParameters  arrow;
	cairo_t *cr;

	CHECK_ARGS
	SANITIZE_SIZE

	cr = tn_gdk_drawable_to_cairo (window, area);

	terranova_set_widget_parameters (widget, style, state_type, &params);
	arrow.type      = TN_ARROW_COMBO;
	arrow.direction = TN_DIRECTION_DOWN;

	STYLE_FUNCTION(draw_arrow) (cr, colors, &params, &arrow, x, y, width, height);

	cairo_destroy (cr);
}

static void
terranova_style_draw_expander (GtkStyle * style,
			    GdkWindow * window,
			    GtkStateType state_type,
			    GdkRectangle * area,
			    GtkWidget * widget,
			    const gchar * detail,
			    gint width,
			    gint height,
			    GtkExpanderStyle expander_style)
{
	cairo_t *cr;
	gint expander_size;
	cr = gdk_cairo_create (window);
	expander_size = 8;

	CHECK_ARGS
	SANITIZE_SIZE

	cairo_translate (cr, width, height);

	switch (expander_style) {
		case GTK_EXPANDER_COLLAPSED:
			cairo_move_to (cr, 0, -4);
			cairo_line_to (cr, expander_size, expander_size/4-2);
			cairo_line_to (cr, 0, expander_size/2);
			cairo_close_path (cr);

			gdk_cairo_set_source_color (cr, &style->fg[GTK_STATE_NORMAL]);
		break;
		case GTK_EXPANDER_SEMI_COLLAPSED:
			cairo_move_to (cr, 0, -4);
			cairo_line_to (cr, expander_size, expander_size/4-2);
			cairo_line_to (cr, 0, expander_size/2);
			cairo_close_path (cr);

			gdk_cairo_set_source_color (cr, &style->fg[GTK_STATE_NORMAL]);
		break;
		case GTK_EXPANDER_SEMI_EXPANDED:
			cairo_move_to (cr, 0, -4);
			cairo_line_to (cr, expander_size, -4);
			cairo_line_to (cr, expander_size/2, -4+expander_size);
			cairo_close_path (cr);

			gdk_cairo_set_source_color (cr, &style->bg[GTK_STATE_SELECTED]);
		break;
		case GTK_EXPANDER_EXPANDED:
			cairo_move_to (cr, 0, -4);
			cairo_line_to (cr, expander_size, -4);
			cairo_line_to (cr, expander_size/2, -4+expander_size);
			cairo_close_path (cr);

			gdk_cairo_set_source_color (cr, &style->bg[GTK_STATE_SELECTED]);
		break;
		default:
			g_assert_not_reached ();
	}

	cairo_set_line_width (cr, 1.0);

	cairo_fill_preserve (cr);

	cairo_destroy (cr);
}

static void
terranova_style_draw_arrow (GtkStyle  *style,
                             GdkWindow     *window,
                             GtkStateType   state_type,
                             GtkShadowType  shadow,
                             GdkRectangle  *area,
                             GtkWidget     *widget,
                             const gchar   *detail,
                             GtkArrowType   arrow_type,
                             gboolean       fill,
                             gint           x,
                             gint           y,
                             gint           width,
                             gint           height)
{
	terranovaStyle  *terranova_style = TERRANOVA_STYLE (style);
	terranovaColors *colors = &terranova_style->colors;
	WidgetParameters params;
	ArrowParameters  arrow;
	cairo_t *cr = tn_gdk_drawable_to_cairo (window, area);

	CHECK_ARGS
	SANITIZE_SIZE

	if (arrow_type == GTK_ARROW_NONE)
	{
		cairo_destroy (cr);
		return;
	}

	terranova_set_widget_parameters (widget, style, state_type, &params);
	arrow.type = TN_ARROW_NORMAL;
	arrow.direction = (terranovaDirection)arrow_type;

	if (widget && widget->parent && TN_IS_COMBO (widget->parent->parent))
	{
		x += 1;
		width -= 2;
	}

	if (tn_is_combo_box (widget, FALSE) && !tn_is_combo_box_entry (widget))
	{
		arrow.type = TN_ARROW_COMBO;
		STYLE_FUNCTION(draw_arrow) (cr, colors, &params, &arrow, x+1, y, width, height);
	}
	else if (TN_IS_MENU(widget->parent) && GTK_MINOR_VERSION >= 13)
	{
		STYLE_FUNCTION(draw_arrow) (cr, colors, &params, &arrow, x, y, width-2, height-2);
	}
	else
	{
		STYLE_FUNCTION(draw_arrow) (cr, colors, &params, &arrow, x, y, width, height);
	}

	cairo_destroy (cr);
}

static void
terranova_style_init_from_rc (GtkStyle * style,
                               GtkRcStyle * rc_style)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);

	terranova_parent_class->init_from_rc (style, rc_style);

	g_assert ((TERRANOVA_RC_STYLE (rc_style)->style >= 0) && (TERRANOVA_RC_STYLE (rc_style)->style < TN_NUM_STYLES));
	terranova_style->style               = TERRANOVA_RC_STYLE (rc_style)->style;

	terranova_style->menubarstyle        = TERRANOVA_RC_STYLE (rc_style)->menubarstyle;
	terranova_style->toolbarstyle        = TERRANOVA_RC_STYLE (rc_style)->toolbarstyle;
	terranova_style->scrollbarstyle      = TERRANOVA_RC_STYLE (rc_style)->scrollbarstyle;
	terranova_style->progressbarstyle       = TERRANOVA_RC_STYLE (rc_style)->progressbarstyle;
	terranova_style->progressbar_direction     = TERRANOVA_RC_STYLE (rc_style)->progressbar_direction;
	terranova_style->random_effect       = TERRANOVA_RC_STYLE (rc_style)->random_effect;
	terranova_style->random_opacity      = CLAMP (TERRANOVA_RC_STYLE (rc_style)->random_opacity, 0.0, 1.0);
	terranova_style->animation           = TERRANOVA_RC_STYLE (rc_style)->animation;
	terranova_style->mw_gradient         = TERRANOVA_RC_STYLE (rc_style)->mw_gradient;
	terranova_style->scale_trough_size   = TERRANOVA_RC_STYLE (rc_style)->scale_trough_size;
	terranova_style->tooltip_style       = TERRANOVA_RC_STYLE (rc_style)->tooltip_style;
	terranova_style->radius              = CLAMP (TERRANOVA_RC_STYLE (rc_style)->radius, 0.0, 10.0);
	terranova_style->grip_opacity        = CLAMP (TERRANOVA_RC_STYLE (rc_style)->grip_opacity, 0.0, 1.0);
	terranova_style->grip_style          = TERRANOVA_RC_STYLE (rc_style)->grip_style;
	terranova_style->trough_visible      = TERRANOVA_RC_STYLE (rc_style)->trough_visible;
	terranova_style->indicator_style     = TERRANOVA_RC_STYLE (rc_style)->indicator_style;
	terranova_style->trough_style        = TERRANOVA_RC_STYLE (rc_style)->trough_style;
	terranova_style->button_effect       = TERRANOVA_RC_STYLE (rc_style)->button_effect;
	terranova_style->button_gradient     = TERRANOVA_RC_STYLE (rc_style)->button_gradient;
	terranova_style->slider_style        = TERRANOVA_RC_STYLE (rc_style)->slider_style;
	terranova_style->grip_pos            = TERRANOVA_RC_STYLE (rc_style)->grip_pos;
	terranova_style->draw_unselected     = TERRANOVA_RC_STYLE (rc_style)->draw_unselected;
	terranova_style->menuitemstyle       = TERRANOVA_RC_STYLE (rc_style)->menuitemstyle;
}

static void
terranova_style_realize (GtkStyle * style)
{
	terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
	double shades[] = {1.15, 0.95, 0.896, 0.82, 0.7, 0.665, 0.475, 0.45, 0.4};
	CairoColor spot_color;
	CairoColor bg_normal;
	double contrast;
	int i;

	terranova_parent_class->realize (style);

	contrast = TERRANOVA_RC_STYLE (style->rc_style)->contrast;

	/* Lighter to darker */
	tn_gdk_color_to_cairo (&style->bg[GTK_STATE_NORMAL], &bg_normal);

	for (i = 0; i < 9; i++)
	{
		tn_shade_color (&bg_normal, (shades[i] < 1.0) ?
		                (shades[i]/contrast) : (shades[i]*contrast),
		                &terranova_style->colors.shade[i]);
	}

	tn_gdk_color_to_cairo (&style->bg[GTK_STATE_SELECTED], &spot_color);

	tn_shade_color (&spot_color, 1.25, &terranova_style->colors.spot[0]);
	tn_shade_color (&spot_color, 1.05, &terranova_style->colors.spot[1]);
	tn_shade_color (&spot_color, 0.65, &terranova_style->colors.spot[2]);

	for (i=0; i<5; i++)
	{
		tn_gdk_color_to_cairo (&style->fg[i], &terranova_style->colors.fg[i]);
		tn_gdk_color_to_cairo (&style->bg[i], &terranova_style->colors.bg[i]);
		tn_gdk_color_to_cairo (&style->base[i], &terranova_style->colors.base[i]);
		tn_gdk_color_to_cairo (&style->text[i], &terranova_style->colors.text[i]);
	}
}

static void
terranova_style_copy (GtkStyle * style, GtkStyle * src)
{
	terranovaStyle * tn_style = TERRANOVA_STYLE (style);
	terranovaStyle * tn_src = TERRANOVA_STYLE (src);

	tn_style->colors              = tn_src->colors;
	tn_style->menubarstyle        = tn_src->menubarstyle;
	tn_style->toolbarstyle        = tn_src->toolbarstyle;
	tn_style->animation           = tn_src->animation;
	tn_style->scrollbarstyle      = tn_src->scrollbarstyle;
	tn_style->progressbarstyle       = tn_src->progressbarstyle;
	tn_style->progressbar_direction     = tn_src->progressbar_direction;
	tn_style->random_effect       = tn_src->random_effect;
	tn_style->radius              = tn_src->radius;
	tn_style->random_effect       = tn_src->random_effect;
	tn_style->random_opacity      = tn_src->random_opacity;
	tn_style->mw_gradient         = tn_src->mw_gradient;
	tn_style->style               = tn_src->style;
	tn_style->scale_trough_size   = tn_src->scale_trough_size;
	tn_style->tooltip_style       = tn_src->tooltip_style;
	tn_style->grip_opacity        = tn_src->grip_opacity;
	tn_style->grip_style          = tn_src->grip_style;
	tn_style->trough_visible      = tn_src->trough_visible;
	tn_style->indicator_style     = tn_src->indicator_style;
	tn_style->trough_style        = tn_src->trough_style;
	tn_style->button_effect       = tn_src->button_effect;
	tn_style->button_gradient     = tn_src->button_gradient;
	tn_style->slider_style        = tn_src->slider_style;
	tn_style->grip_pos            = tn_src->grip_pos;
	tn_style->draw_unselected     = tn_src->draw_unselected;
	tn_style->menuitemstyle       = tn_src->menuitemstyle;

	terranova_parent_class->copy (style, src);
}

static void
terranova_style_unrealize (GtkStyle * style)
{
	terranova_parent_class->unrealize (style);
}

static GdkPixbuf *
set_transparency (const GdkPixbuf *pixbuf, gdouble alpha_percent)
{
	GdkPixbuf *target;
	guchar *data, *current;
	guint x, y, rowstride, height, width;

	g_return_val_if_fail (pixbuf != NULL, NULL);
	g_return_val_if_fail (GDK_IS_PIXBUF (pixbuf), NULL);

	/* Returns a copy of pixbuf with it's non-completely-transparent pixels to
	   have an alpha level "alpha_percent" of their original value. */

	target = gdk_pixbuf_add_alpha (pixbuf, FALSE, 0, 0, 0);

	if (alpha_percent == 1.0)
		return target;
	width = gdk_pixbuf_get_width (target);
	height = gdk_pixbuf_get_height (target);
	rowstride = gdk_pixbuf_get_rowstride (target);
	data = gdk_pixbuf_get_pixels (target);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			/* The "4" is the number of chars per pixel, in this case, RGBA,
			   the 3 means "skip to the alpha" */
			current = data + (y * rowstride) + (x * 4) + 3;
			*(current) = (guchar) (*(current) * alpha_percent);
		}
	}

	return target;
}

static GdkPixbuf*
scale_or_ref (GdkPixbuf *src,
              int width,
              int height)
{
	if (width == gdk_pixbuf_get_width (src) &&
	    height == gdk_pixbuf_get_height (src))
	{
		return g_object_ref (src);
	}
	else
	{
		return gdk_pixbuf_scale_simple (src,
		                                width, height,
		                                GDK_INTERP_BILINEAR);
	}
}

static void
terranova_style_draw_layout (GtkStyle * style,
                              GdkWindow * window,
                              GtkStateType state_type,
                              gboolean use_text,
                              GdkRectangle * area,
                              GtkWidget * widget,
                              const gchar * detail, gint x, gint y, PangoLayout * layout)
{
	GdkGC *gc;

	g_return_if_fail (GTK_IS_STYLE (style));
	g_return_if_fail (window != NULL);

	gc = use_text ? style->text_gc[state_type] : style->fg_gc[state_type];

	if (area)
		gdk_gc_set_clip_rectangle (gc, area);

	if (state_type == GTK_STATE_INSENSITIVE)
	{
		terranovaStyle *terranova_style = TERRANOVA_STYLE (style);
		terranovaColors *colors = &terranova_style->colors;

		WidgetParameters params;
		GdkColor etched;
		CairoColor temp;

		terranova_set_widget_parameters (widget, style, state_type, &params);

		tn_shade_color (&colors->bg[widget->state], 1.2, &temp);

		etched.red = (int) (temp.r * 65535);
		etched.green = (int) (temp.g * 65535);
		etched.blue = (int) (temp.b * 65535);

		gdk_draw_layout_with_colors (window, gc, x + 1, y + 1, layout, &etched, NULL);
		gdk_draw_layout (window, gc, x, y, layout);
	}
	else
		gdk_draw_layout (window, gc, x, y, layout);

	if (area)
		gdk_gc_set_clip_rectangle (gc, NULL);
}

static GdkPixbuf *
terranova_style_draw_render_icon (GtkStyle            *style,
                                   const GtkIconSource *source,
                                   GtkTextDirection     direction,
                                   GtkStateType         state,
                                   GtkIconSize          size,
                                   GtkWidget           *widget,
                                   const char          *detail)
{
	int width = 1;
	int height = 1;
	GdkPixbuf *scaled;
	GdkPixbuf *stated;
	GdkPixbuf *base_pixbuf;
	GdkScreen *screen;
	GtkSettings *settings;

	/* Oddly, style can be NULL in this function, because
	 * GtkIconSet can be used without a style and if so
	 * it uses this function.
	 */

	base_pixbuf = gtk_icon_source_get_pixbuf (source);

	g_return_val_if_fail (base_pixbuf != NULL, NULL);

	if (widget && gtk_widget_has_screen (widget))
	{
		screen = gtk_widget_get_screen (widget);
		settings = gtk_settings_get_for_screen (screen);
	}
	else if (style->colormap)
	{
		screen = gdk_colormap_get_screen (style->colormap);
		settings = gtk_settings_get_for_screen (screen);
	}
	else
	{
		settings = gtk_settings_get_default ();
		GTK_NOTE (MULTIHEAD,
			  g_warning ("Using the default screen for gtk_default_render_icon()"));
	}

	if (size != (GtkIconSize) -1 && !gtk_icon_size_lookup_for_settings (settings, size, &width, &height))
	{
		g_warning (G_STRLOC ": invalid icon size '%d'", size);
		return NULL;
	}

	/* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
	 * leave it alone.
	 */
	if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
		scaled = scale_or_ref (base_pixbuf, width, height);
	else
		scaled = g_object_ref (base_pixbuf);

	/* If the state was wildcarded, then generate a state. */
	if (gtk_icon_source_get_state_wildcarded (source))
	{
		if (state == GTK_STATE_INSENSITIVE)
		{
			stated = set_transparency (scaled, 0.3);
			gdk_pixbuf_saturate_and_pixelate (stated, stated, 0.1, FALSE);

			g_object_unref (scaled);
		}
		else if (state == GTK_STATE_PRELIGHT)
		{
			stated = gdk_pixbuf_copy (scaled);

			gdk_pixbuf_saturate_and_pixelate (scaled, stated, 1.2, FALSE);

			g_object_unref (scaled);
		}
		else
		{
			stated = scaled;
		}
	}
	else
		stated = scaled;

	return stated;
}

static void
terranova_style_init (terranovaStyle * style)
{
}

static void
terranova_style_class_init (terranovaStyleClass * klass)
{
	GtkStyleClass *style_class = GTK_STYLE_CLASS (klass);

	terranova_style_class = TERRANOVA_STYLE_CLASS (klass);
	terranova_parent_class = g_type_class_peek_parent (klass);

	style_class->copy             = terranova_style_copy;
	style_class->realize          = terranova_style_realize;
	style_class->unrealize        = terranova_style_unrealize;
	style_class->init_from_rc     = terranova_style_init_from_rc;
	style_class->draw_handle      = terranova_style_draw_handle;
	style_class->draw_slider      = terranova_style_draw_slider;
	style_class->draw_shadow_gap  = terranova_style_draw_shadow_gap;
	style_class->draw_box         = terranova_style_draw_box;
	style_class->draw_shadow      = terranova_style_draw_shadow;
	style_class->draw_box_gap     = terranova_style_draw_box_gap;
	style_class->draw_extension   = terranova_style_draw_extension;
	style_class->draw_option      = terranova_style_draw_option;
	style_class->draw_check       = terranova_style_draw_check;
	style_class->draw_flat_box    = terranova_style_draw_flat_box;
	style_class->draw_vline       = terranova_style_draw_vline;
	style_class->draw_hline       = terranova_style_draw_hline;
	style_class->draw_resize_grip = terranova_style_draw_resize_grip;
	style_class->draw_tab         = terranova_style_draw_tab;
	style_class->draw_expander    = terranova_style_draw_expander;
	style_class->draw_arrow       = terranova_style_draw_arrow;
	style_class->draw_layout      = terranova_style_draw_layout;
	style_class->render_icon      = terranova_style_draw_render_icon;

	terranova_register_style_clarius (&terranova_style_class->style_functions[TN_STYLE_CLARIUS]);

	klass->style_functions[TN_STYLE_AQUARIUS] = klass->style_functions[TN_STYLE_CLARIUS];
	terranova_register_style_aquarius (&terranova_style_class->style_functions[TN_STYLE_AQUARIUS]);

	klass->style_functions[TN_STYLE_CANDY] = klass->style_functions[TN_STYLE_CLARIUS];
	terranova_register_style_candy (&terranova_style_class->style_functions[TN_STYLE_CANDY]);
}

GType terranova_type_style = 0;

void
terranova_style_register_type (GTypeModule * module)
{
	static const GTypeInfo object_info =
	{
		sizeof (terranovaStyleClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) terranova_style_class_init,
		NULL,         /* class_finalize */
		NULL,         /* class_data */
		sizeof (terranovaStyle),
		0,            /* n_preallocs */
		(GInstanceInitFunc) terranova_style_init,
		NULL
	};

	terranova_type_style = g_type_module_register_type (module,
	                                                     GTK_TYPE_STYLE,
	                                                     "terranovaStyle",
	                                                     &object_info, 0);
}
