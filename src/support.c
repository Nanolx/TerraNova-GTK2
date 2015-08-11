/* Terranova theme engine
 * Copyright (C) 2005 Richard Stellingwerff.
 * Copyright (C) 2007 Benjamin Berg <benjamin@sipsolutions.net>.
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

#include "support.h"

void terranova_treeview_get_header_index (GtkTreeView *tv, GtkWidget *header,
                                    gint *column_index, gint *columns,
                                    gboolean *resizable)
{
	GList *list, *list_start;
	*column_index = *columns = 0;
	list_start = list = gtk_tree_view_get_columns (tv);

	do
	{
		GtkTreeViewColumn *column = GTK_TREE_VIEW_COLUMN(list->data);
		if ( column->button == header )
		{
			*column_index = *columns;
			*resizable = column->resizable;
		}
		if ( column->visible )
			(*columns)++;
	} while ((list = g_list_next(list)));

	g_list_free (list_start);
}

void
terranova_get_parent_bg (const GtkWidget *widget, CairoColor *color)
{
	GtkStateType state_type;
	const GtkWidget *parent;
	GdkColor *gcolor;

	if (widget == NULL)
		return;

	parent = widget->parent;

	while (parent && gtk_widget_get_has_window (parent) && !((GTK_IS_NOTEBOOK (parent)) || (GTK_IS_TOOLBAR (parent))))
		parent = parent->parent;

	if (parent == NULL)
		return;

	state_type = gtk_widget_get_state(parent);

	gcolor = &parent->style->bg[state_type];

	tn_gdk_color_to_cairo (gcolor, color);
}

terranovaStepper
terranova_scrollbar_get_stepper (GtkWidget    *widget,
                       GdkRectangle *stepper)
{
	terranovaStepper value = TN_STEPPER_UNKNOWN;
	GdkRectangle tmp;
	GdkRectangle check_rectangle;
	GtkOrientation orientation;

	if (!TN_IS_RANGE (widget))
		return TN_STEPPER_UNKNOWN;

	check_rectangle.x      = widget->allocation.x;
	check_rectangle.y      = widget->allocation.y;
	check_rectangle.width  = stepper->width;
	check_rectangle.height = stepper->height;

	orientation = GTK_RANGE (widget)->orientation;

	if (widget->allocation.x == -1 && widget->allocation.y == -1)
		return TN_STEPPER_UNKNOWN;

	if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
		value = TN_STEPPER_A;

	if (value == TN_STEPPER_UNKNOWN) /* Haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + stepper->width;
		else
			check_rectangle.y = widget->allocation.y + stepper->height;

		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = TN_STEPPER_B;
	}

	if (value == TN_STEPPER_UNKNOWN) /* Still haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + widget->allocation.width - (stepper->width * 2);
		else
			check_rectangle.y = widget->allocation.y + widget->allocation.height - (stepper->height * 2);

		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = TN_STEPPER_C;
	}

	if (value == TN_STEPPER_UNKNOWN) /* STILL haven't found a match */
	{
		if (orientation == GTK_ORIENTATION_HORIZONTAL)
			check_rectangle.x = widget->allocation.x + widget->allocation.width - stepper->width;
		else
			check_rectangle.y = widget->allocation.y + widget->allocation.height - stepper->height;

		if (gdk_rectangle_intersect (stepper, &check_rectangle, &tmp))
			value = TN_STEPPER_D;
	}

	return value;
}

terranovaStepper
terranova_scrollbar_visible_steppers (GtkWidget *widget)
{
	terranovaStepper steppers = 0;

	if (!TN_IS_RANGE (widget))
		return 0;

	if (GTK_RANGE (widget)->has_stepper_a)
		steppers |= TN_STEPPER_A;

	if (GTK_RANGE (widget)->has_stepper_b)
		steppers |= TN_STEPPER_B;

	if (GTK_RANGE (widget)->has_stepper_c)
		steppers |= TN_STEPPER_C;

	if (GTK_RANGE (widget)->has_stepper_d)
		steppers |= TN_STEPPER_D;

	return steppers;
}

terranovaJunction
terranova_scrollbar_get_junction (GtkWidget    *widget)
{
	GtkAdjustment *adj;
	terranovaJunction junction = TN_JUNCTION_NONE;

	if (!TN_IS_RANGE (widget))
		return TN_JUNCTION_NONE;

	adj = GTK_RANGE (widget)->adjustment;

	if (adj->value <= adj->lower &&
		(GTK_RANGE (widget)->has_stepper_a || GTK_RANGE (widget)->has_stepper_b))
	{
		junction |= TN_JUNCTION_BEGIN;
	}

	if (adj->value >= adj->upper - adj->page_size &&
		(GTK_RANGE (widget)->has_stepper_c || GTK_RANGE (widget)->has_stepper_d))
	{
		junction |= TN_JUNCTION_END;
	}

	return junction;
}

void
terranova_set_toolbar_parameters (ToolbarParameters *toolbar,
                                   GtkWidget *widget,
                                   GdkWindow *window,
                                   gint x, gint y)
{
	toolbar->topmost = FALSE;

	if (x == 0 && y == 0) {
		if (widget && widget->allocation.x == 0 && widget->allocation.y == 0)
		{
			if (widget->window == window && TN_IS_TOOLBAR (widget))
			{
				toolbar->topmost = TRUE;
			}
		}
	}
}

void
terranova_get_notebook_tab_position (GtkWidget *widget,
                                      gboolean  *start,
                                      gboolean  *end)
{
	/* default value */
	*start = TRUE;
	*end = FALSE;

	if (TN_IS_NOTEBOOK (widget)) {
		gboolean found_tabs = FALSE;
		gint i, n_pages;
		GtkNotebook *notebook = GTK_NOTEBOOK (widget);

		/* got a notebook, so walk over all the tabs and decide based
		 * on that ...
		 * It works like this:
		 *   - If there is any visible tab that is expanded, set both.
		 *   - Set start/end if there is any visible tab that is at
		 *     the start/end.
		 *   - If one has the child_visibility set to false, arrows
		 *     are present; so none
		 * The heuristic falls over if there is a notebook that just
		 * happens to fill up all the available space. ie. All tabs
		 * are left aligned, but it does not require scrolling.
		 * (a more complex heuristic could calculate the tabs width
		 * and add them all up) */

		n_pages = gtk_notebook_get_n_pages (notebook);
		for (i = 0; i < n_pages; i++) {
			GtkWidget *tab_child;
			GtkWidget *tab_label;
			gboolean expand;
			GtkPackType pack_type;

			tab_child = gtk_notebook_get_nth_page (notebook, i);

			/* Skip invisible tabs */
			tab_label = gtk_notebook_get_tab_label (notebook, tab_child);
			if (!tab_label || !gtk_widget_get_visible (tab_label))
				continue;
			/* This is the same what the notebook does internally. */
			if (tab_label && !gtk_widget_get_child_visible (tab_label)) {
				/* One child is hidden because scroll arrows are present.
				 * So both corners are rounded. */
				*start = FALSE;
				*end = FALSE;
				return;
			}

			gtk_notebook_query_tab_label_packing (notebook, tab_child,
			                                      &expand,
			                                      NULL, /* don't need fill */
			                                      &pack_type);

			if (!found_tabs) {
				found_tabs = TRUE;
				*start = FALSE;
				*end = FALSE;
			}

			if (expand) {
				*start = TRUE;
				*end = TRUE;
			} else if (pack_type == GTK_PACK_START) {
				*start = TRUE;
			} else {
				*end = TRUE;
			}
		}
	}
}


