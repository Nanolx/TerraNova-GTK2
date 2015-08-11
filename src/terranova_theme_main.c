#include <glib.h>
#include <gtk/gtk.h>

#include "terranova_style.h"
#include "terranova_rc_style.h"

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  terranova_rc_style_register_type (module);
  terranova_style_register_type (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkRcStyle *
theme_create_rc_style (void)
{
  return GTK_RC_STYLE (g_object_new (TERRANOVA_TYPE_RC_STYLE, NULL));
}
