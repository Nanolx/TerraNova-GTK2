#ifndef TERRANOVA_TYPES_H
#define TERRANOVA_TYPES_H

#include "ge-support.h"

typedef unsigned char boolean;
typedef unsigned char uint8;
typedef struct _terranovaStyleFunctions terranovaStyleFunctions;

typedef enum
{
	TN_STYLE_CLARIUS = 0,
	TN_STYLE_AQUARIUS = 1,
	TN_STYLE_CANDY = 2,
	TN_NUM_STYLES = 3
} terranovaStyles;

typedef enum
{
	TN_STATE_NORMAL,
	TN_STATE_ACTIVE,
	TN_STATE_SELECTED,
	TN_STATE_INSENSITIVE,
	TN_STATE_PRELIGHT
} terranovaStateType;

typedef enum
{
	TN_JUNCTION_NONE      = 0,
	TN_JUNCTION_BEGIN     = 1,
	TN_JUNCTION_END       = 2
} terranovaJunction;

typedef enum
{
	TN_STEPPER_UNKNOWN    = 0,
	TN_STEPPER_A          = 1,
	TN_STEPPER_B          = 2,
	TN_STEPPER_C          = 4,
	TN_STEPPER_D          = 8
} terranovaStepper;

typedef enum
{
	TN_ORDER_FIRST,
	TN_ORDER_MIDDLE,
	TN_ORDER_LAST
} terranovaOrder;

typedef enum
{
	TN_CONT_NONE       = 0,
	TN_CONT_LEFT       = 1 << 0,
	TN_CONT_RIGHT      = 1 << 1
} terranovaContinue;

typedef enum
{
	TN_ORIENTATION_LEFT_TO_RIGHT,
	TN_ORIENTATION_RIGHT_TO_LEFT,
	TN_ORIENTATION_BOTTOM_TO_TOP,
	TN_ORIENTATION_TOP_TO_BOTTOM
} terranovaOrientation;

typedef enum
{
	TN_GAP_LEFT,
	TN_GAP_RIGHT,
	TN_GAP_TOP,
	TN_GAP_BOTTOM
} terranovaGapSide;

typedef enum
{
	TN_SHADOW_NONE,
	TN_SHADOW_IN,
	TN_SHADOW_OUT,
	TN_SHADOW_ETCHED_IN,
	TN_SHADOW_ETCHED_OUT
} terranovaShadowType;

typedef enum
{
	TN_HANDLE_TOOLBAR,
	TN_HANDLE_SPLITTER
} terranovaHandleType;

typedef enum
{
	TN_ARROW_NORMAL,
	TN_ARROW_COMBO
} terranovaArrowType;

typedef enum
{
	TN_FOCUS_BUTTON,
	TN_FOCUS_BUTTON_FLAT,
	TN_FOCUS_LABEL,
	TN_FOCUS_TREEVIEW,
	TN_FOCUS_TREEVIEW_HEADER,
	TN_FOCUS_TREEVIEW_ROW,
	TN_FOCUS_TREEVIEW_DND,
	TN_FOCUS_SCALE,
	TN_FOCUS_TAB,
	TN_FOCUS_COLOR_WHEEL_DARK,
	TN_FOCUS_COLOR_WHEEL_LIGHT,
	TN_FOCUS_UNKNOWN
} terranovaFocusType;


typedef enum
{
	TN_DIRECTION_UP,
	TN_DIRECTION_DOWN,
	TN_DIRECTION_LEFT,
	TN_DIRECTION_RIGHT
} terranovaDirection;

typedef enum
{
	TN_PROGRESSBAR_CONTINUOUS,
	TN_PROGRESSBAR_DISCRETE
} terranovaProgressBarStyle;

typedef enum
{
	TN_WINDOW_EDGE_NORTH_WEST,
	TN_WINDOW_EDGE_NORTH,
	TN_WINDOW_EDGE_NORTH_EAST,
	TN_WINDOW_EDGE_WEST,
	TN_WINDOW_EDGE_EAST,
	TN_WINDOW_EDGE_SOUTH_WEST,
	TN_WINDOW_EDGE_SOUTH,
	TN_WINDOW_EDGE_SOUTH_EAST
} terranovaWindowEdge;

typedef struct
{
	double x;
	double y;
	double width;
	double height;
} terranovaRectangle;

typedef struct
{
	CairoColor fg[5];
	CairoColor bg[5];
	CairoColor base[5];
	CairoColor text[5];

	CairoColor shade[9];
	CairoColor spot[3];
} terranovaColors;

typedef struct
{
	boolean active;
	boolean prelight;
	boolean disabled;
	boolean focus;
	boolean is_default;
	boolean ltr;
	boolean enable_shadow;
	guint8  random_effect;
	guint8  random_style;
	gfloat  random_opacity;
	guint8  mw_gradient;
	guint8  button_effect;
	guint8  old_scales;
	guint8  scale_trough_size;
	guint8  tooltip_style;
	gfloat  grip_opacity;
	guint8  grip_style;
	guint8  trough_visible;
	guint8  indicator_style;
	guint8  trough_style;
	guint8  button_gradient;
	guint8  slider_style;
	guint8  grip_pos;
	guint8  draw_unselected;
	guint8  menuitemstyle;

	gfloat  radius;

	terranovaStateType state_type;
	terranovaStateType prev_state_type;

	uint8 corners;
	uint8 xthickness;
	uint8 ythickness;

	CairoColor parentbg;

	terranovaStyleFunctions *style_functions;
} WidgetParameters;

typedef struct
{
	terranovaFocusType type;
	terranovaContinue  continue_side;
	CairoColor          color;
	guint8              focus_line_width;
	gint                padding;
	guint8*             dash_list;
	boolean             interior;
} FocusParameters;

typedef struct
{
	boolean lower;
	boolean horizontal;
	boolean fill_level;
} SliderParameters;

typedef struct
{
	terranovaOrientation orientation;
	boolean pulsing;
	float value;
	guint8 progressbarstyle;
	guint8 progressbar_direction;
} ProgressBarParameters;

typedef struct
{
	int linepos;
} OptionMenuParameters;

typedef struct
{
	terranovaShadowType shadow;
	terranovaGapSide gap_side;
	int gap_x;
	int gap_width;
	const CairoColor *border; /* maybe changes this to some other hint ... */
} FrameParameters;

typedef struct
{
	terranovaGapSide gap_side;
	FocusParameters   focus;
} TabParameters;

typedef struct
{
	CairoCorners    corners;
	terranovaShadowType shadow;
} ShadowParameters;

typedef struct
{
	boolean horizontal;
} SeparatorParameters;

typedef struct
{
	terranovaOrder order; /* XXX: rename to position */
	boolean         resizable;
} ListViewHeaderParameters;

typedef struct
{
	terranovaJunction junction; /* On which sides the slider junctions */
	boolean            horizontal;
	int                style;
} ScrollBarParameters;

typedef struct
{
	terranovaHandleType type;
	boolean              horizontal;
} HandleParameters;

typedef struct
{
	terranovaStepper stepper;  /* Which stepper to draw */
} ScrollBarStepperParameters;

typedef struct
{
	terranovaWindowEdge edge;
} ResizeGripParameters;

typedef struct
{
	int style;
} MenuBarParameters;

typedef struct
{
	terranovaShadowType shadow_type;
	boolean              in_cell;
	boolean              in_menu;
} CheckboxParameters;

typedef struct
{
	terranovaArrowType type;
	terranovaDirection direction;
} ArrowParameters;

typedef struct
{
	int      style;
	boolean  topmost;
} ToolbarParameters;

struct _terranovaStyleFunctions
{

	void (*draw_button)           (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_scale_trough)     (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const SliderParameters *slider,
	                               int x, int y, int width, int height);

	void (*draw_progressbar_trough) (cairo_t 	*cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_progressbar_fill) (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ProgressBarParameters *progressbar,
	                               int x, int y, int width, int height, gint offset);

	void (*draw_slider_button)    (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const SliderParameters *slider,
	                               int x, int y, int width, int height);

	void (*draw_entry)            (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_mw_gradient)      (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_spinbutton)       (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_spinbutton_down)  (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_optionmenu)       (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const OptionMenuParameters *optionmenu,
	                               int x, int y, int width, int height);

	void (*draw_inset)            (cairo_t *cr,
	                                const CairoColor *bg_color,
	                                double x, double y, double w, double h,
	                                double radius, uint8 corners);

	void (*draw_menubar)          (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const MenuBarParameters *menubar,
	                               int x, int y, int width, int height);

	void (*draw_tab)              (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const TabParameters *tab,
	                               int x, int y, int width, int height);

	void (*draw_frame)            (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const FrameParameters *frame,
	                               int x, int y, int width, int height);

	void (*draw_separator)        (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const SeparatorParameters *separator,
	                               int x, int y, int width, int height);

	void (*draw_menu_item_separator) (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const SeparatorParameters *separator,
	                               int x, int y, int width, int height);

	void (*draw_list_view_header) (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ListViewHeaderParameters *header,
	                               int x, int y, int width, int height);

	void (*draw_toolbar)          (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ToolbarParameters *toolbar,
	                               int x, int y, int width, int height);

	void (*draw_menuitem)         (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_menubaritem)      (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_selected_cell)    (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_unselected_cell)  (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_scrollbar_stepper) (cairo_t *cr,
	                                const terranovaColors	*colors,
	                                const WidgetParameters *params,
	                                const ScrollBarParameters *scrollbar,
	                                const ScrollBarStepperParameters *stepper,
	                                int x, int y, int width, int height);

	void (*draw_scrollbar_slider) (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ScrollBarParameters	*scrollbar,
	                               int x, int y, int width, int height);

	void (*draw_scrollbar_trough) (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ScrollBarParameters	*scrollbar,
	                               int x, int y, int width, int height);

	void (*draw_statusbar)        (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_calendar)         (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_menu_frame)       (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_tooltip)          (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_handle)           (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const HandleParameters *handle,
	                               int x, int y, int width, int height);

	void (*draw_resize_grip)      (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ResizeGripParameters	*grip,
	                               int x, int y, int width, int height);

	void (*draw_arrow)            (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const ArrowParameters *arrow,
	                               int x, int y, int width, int height);

	void (*draw_focus)            (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const FocusParameters *focus,
	                               int x, int y, int width, int height);

	void (*draw_checkbox)         (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const CheckboxParameters	*checkbox,
	                               int x, int y, int width, int height);

	void (*draw_radiobutton)      (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               const CheckboxParameters	*checkbox,
	                               int x, int y, int width, int height);

	void (*draw_icon_view_item)   (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	/* Style internal functions */
	/* XXX: Only used by slider_button, inline it? */
	void (*draw_shadow)           (cairo_t *cr,
	                               const terranovaColors *colors,
	                               gfloat radius, int width, int height);

	void (*draw_slider)           (cairo_t *cr,
	                               const terranovaColors *colors,
	                               const WidgetParameters *params,
	                               int x, int y, int width, int height);

	void (*draw_gripdots)         (cairo_t *cr,
	                               const terranovaColors *colors, int x, int y,
	                               int width, int height, int xr, int yr,
	                               float contrast);

};


#define TERRANOVA_RECTANGLE_SET(rect, _x, _y, _w, _h) rect.x      = _x; \
                                                       rect.y      = _y; \
                                                       rect.width  = _w; \
                                                       rect.height = _h;

#endif /* TERRANOVA_TYPES_H */
