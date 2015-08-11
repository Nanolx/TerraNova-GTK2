
#ifndef WIDGET_INFORMATION_H
#define WIDGET_INFORMATION_H

#include "general-support.h"
#include <glib.h>
#include <gtk/gtk.h>

typedef enum {
	GE_HINT_TREEVIEW,
	GE_HINT_TREEVIEW_HEADER,
	GE_HINT_STATUSBAR,
	GE_HINT_COMBOBOX_ENTRY,
	GE_HINT_SPINBUTTON,
	GE_HINT_SCALE,
	GE_HINT_VSCALE,
	GE_HINT_HSCALE,
	GE_HINT_SCROLLBAR,
	GE_HINT_VSCROLLBAR,
	GE_HINT_HSCROLLBAR,
	GE_HINT_PROGRESSBAR,
	GE_HINT_MENUBAR,
	GE_HINT_COUNT
} GEHint;

#define TN_IS_CALENDAR(object) ((object) && tn_object_is_a (( GObject*)(object), "GtkCalendar"))

#define TN_IS_EXPANDER(object) ((object) && tn_object_is_a (( GObject*)(object), "GtkExpander"))

#define TN_IS_WINDOW(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkWindow"))

#define TN_IS_WIDGET(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkWidget"))
#define TN_IS_CONTAINER(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkContainer"))
#define TN_IS_BIN(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkBin"))

#define TN_IS_ARROW(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkArrow"))

#define TN_IS_SEPARATOR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkSeparator"))
#define TN_IS_VSEPARATOR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkVSeparator"))
#define TN_IS_HSEPARATOR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHSeparator"))

#define TN_IS_HANDLE_BOX(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHandleBox"))
#define TN_IS_HANDLE_BOX_ITEM(object) ((object) && TN_IS_HANDLE_BOX(object->parent))
#define TN_IS_BONOBO_DOCK_ITEM(object) ((object) && tn_object_is_a ((GObject*)(object), "BonoboDockItem"))
#define TN_IS_BONOBO_DOCK_ITEM_GRIP(object) ((object) && tn_object_is_a ((GObject*)(object), "BonoboDockItemGrip"))
#define TN_IS_BONOBO_TOOLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "BonoboUIToolbar"))
#define TN_IS_EGG_TOOLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "Toolbar"))
#define TN_IS_TOOLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkToolbar"))
#define TN_IS_PANEL_WIDGET(object) ((object) && (tn_object_is_a ((GObject*)(object), "PanelWidget") || tn_object_is_a ((GObject*)(object), "PanelApplet")))

#define TN_IS_COMBO_BOX_ENTRY(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkComboBoxEntry"))
#define TN_IS_COMBO_BOX(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkComboBox"))
#define TN_IS_COMBO(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkCombo"))
#define TN_IS_OPTION_MENU(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkOptionMenu"))

#define TN_IS_TOGGLE_BUTTON(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkToggleButton"))
#define TN_IS_CHECK_BUTTON(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkCheckButton"))
#define TN_IS_SPIN_BUTTON(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkSpinButton"))

#define TN_IS_STATUSBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkStatusbar"))
#define TN_IS_PROGRESS_BAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkProgressBar"))

#define TN_IS_MENU_SHELL(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkMenuShell"))
#define TN_IS_MENU(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkMenu"))
#define TN_IS_MENU_BAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkMenuBar"))
#define TN_IS_MENU_ITEM(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkMenuItem"))

#define TN_IS_CHECK_MENU_ITEM(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkCheckMenuItem"))

#define TN_IS_RANGE(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkRange"))

#define TN_IS_SCROLLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkScrollbar"))
#define TN_IS_VSCROLLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkVScrollbar"))
#define TN_IS_HSCROLLBAR(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHScrollbar"))

#define TN_IS_SCALE(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkScale"))
#define TN_IS_VSCALE(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkVScale"))
#define TN_IS_HSCALE(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHScale"))

#define TN_IS_PANED(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkPaned"))
#define TN_IS_VPANED(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkVPaned"))
#define TN_IS_HPANED(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHPaned"))

#define TN_IS_BOX(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkBox"))
#define TN_IS_VBOX(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkVBox"))
#define TN_IS_HBOX(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkHBox"))

#define TN_IS_CLIST(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkCList"))
#define TN_IS_TREE_VIEW(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkTreeView"))
#define TN_IS_ENTRY(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkEntry"))
#define TN_IS_BUTTON(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkButton"))
#define TN_IS_FIXED(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkFixed"))

#define TOGGLE_BUTTON(object) (TN_IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)

#define TN_IS_NOTEBOOK(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkNotebook"))
#define TN_IS_CELL_RENDERER_TOGGLE(object) ((object) && tn_object_is_a ((GObject*)(object), "GtkCellRendererToggle"))

#define GE_WIDGET_HAS_DEFAULT(object) ((object) && TN_IS_WIDGET(object) && gtk_widget_has_default((GtkWidget*)(object)))

TN_INTERNAL gboolean tn_check_hint (GEHint hint, GQuark style_hint, GtkWidget *widget);

TN_INTERNAL gboolean tn_object_is_a (const GObject * object, const gchar * type_name);

TN_INTERNAL gboolean tn_is_combo_box_entry (GtkWidget * widget);
TN_INTERNAL gboolean tn_is_combo_box (GtkWidget * widget, gboolean as_list);
TN_INTERNAL gboolean tn_is_combo (GtkWidget * widget);
TN_INTERNAL gboolean tn_is_in_combo_box (GtkWidget * widget);

TN_INTERNAL gboolean tn_is_toolbar_item (GtkWidget * widget);

TN_INTERNAL gboolean tn_is_panel_widget_item (GtkWidget * widget);

TN_INTERNAL gboolean tn_is_bonobo_dock_item (GtkWidget * widget);

TN_INTERNAL GtkWidget *tn_find_combo_box_widget_parent (GtkWidget * widget);

TN_INTERNAL void tn_option_menu_get_props (GtkWidget * widget,
                            GtkRequisition * indicator_size,
                            GtkBorder * indicator_spacing);

TN_INTERNAL void tn_button_get_default_border (GtkWidget *widget,
                                               GtkBorder *border);

TN_INTERNAL gboolean tn_widget_is_ltr (GtkWidget *widget);

TN_INTERNAL guint tn_rc_parse_hint (GScanner *scanner, GQuark *quark);

#endif /* WIDGET_INFORMATION_H */
