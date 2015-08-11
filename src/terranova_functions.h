
/*
 * TN_INTERNALs for random-effect functions
 */

#include "general-support.h"
#include "support.h"
#include "ge-support.h"

TN_INTERNAL void terranova_draw_claws(cairo_t *cr, int x1, int y1, int x2, int y2);
TN_INTERNAL void terranova_draw_random_claws(cairo_t *cr,
			const terranovaColors *colors,
			const WidgetParameters *params,
			int xoffset, int yoffset,
			int x, int y, int width, int height, int count);

TN_INTERNAL void terranova_draw_bubbles(cairo_t *cr, int x1, int y1, int x2, int y2,
						int circle_spacing, int circle_radius);
TN_INTERNAL void
terranova_draw_random_bubbles(cairo_t *cr,
				const terranovaColors *colors,
				const WidgetParameters *params, int xoffset, int yoffset, int x,
				int y, int width, int height, int count,
				int circle_spacing, int circle_radius);

/*
 * TN_INTERNAL for mirror rotating
 */

TN_INTERNAL void
terranova_mirror_rotate (cairo_t *cr, double radius, double x, double y,
				boolean mirror_horizontally, boolean mirror_vertically);

/*
 * TN_INTERNAL for drawing border gradient
 */

TN_INTERNAL void
terranova_draw_border_gradient (cairo_t *cr, const CairoColor *color, double hilight,
					int width, int height);

/*
 * TN_INTERNAL for drawing highlight and shade
 */

TN_INTERNAL void
terranova_draw_highlight_and_shade (cairo_t *cr, const terranovaColors *colors,
                                     const ShadowParameters *params,
                                     int width, int height, gdouble radius);

/*
 * GradientPositions and TN_INTERNAL for changing gradient directon
 */

typedef struct {

	int width;
	int height;

} GradientPositions;


TN_INTERNAL GradientPositions
terranova_get_gradient_position(const WidgetParameters *params,
					int width, int height);

/*
 * TN_INTERNAL for drawing button-effect
 */

TN_INTERNAL void
terranova_draw_button_effect(cairo_t *cr, const WidgetParameters *params,
				const terranovaColors *colors, int width, int height);

/*
 * TN_INTERNAL for drawing button-grip
 */

TN_INTERNAL void
terranova_draw_button_grip(cairo_t *cr, const WidgetParameters *params,
				const terranovaColors *colors, int width, int height,
				int xoffset, int yoffset, double radius);


/*
 * TN_INTERNAL for drawing fill-stripes
 */

TN_INTERNAL void
terranova_draw_fill_diagonal_stripes(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset);

/*
 * TN_INTERNAL for drawing fill-stripes
 */

TN_INTERNAL void
terranova_draw_fill_vertical_stripes(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset);

/*
 * TN_INTERNAL for drawing fill-arrows
 */

TN_INTERNAL void
terranova_draw_fill_arrows(cairo_t *cr,
                                  const terranovaColors *colors, const WidgetParameters *params,
                                  int width, int height, gint offset);
