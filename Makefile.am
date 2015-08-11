ACLOCAL_AMFLAGS = -I m4
INCLUDES = $(GTK_CFLAGS) $(PARANOIA_CFLAGS) $(DEPRECATE_CFLAGS)

enginedir = $(libdir)/gtk-2.0/$(GTK_VERSION)/engines
engine_LTLIBRARIES = libterranova.la

themedir = $(datadir)/themes/TerraNova-Clarius/gtk-2.0
theme_DATA = themes/TerraNova-Clarius/gtk-2.0/gtkrc

themenautidir = $(datadir)/themes/TerraNova-Clarius/gtk-2.0/nautilus
themenauti_DATA = themes/TerraNova-Clarius/gtk-2.0/nautilus/nautilus.rc \
	     themes/TerraNova-Clarius/gtk-2.0/nautilus/breadcrumb_active.png \
	     themes/TerraNova-Clarius/gtk-2.0/nautilus/breadcrumb_normal.png \
	     themes/TerraNova-Clarius/gtk-2.0/nautilus/breadcrumb_prelight.png \
	     themes/TerraNova-Clarius/gtk-2.0/nautilus/left_slider_normal.png \
	     themes/TerraNova-Clarius/gtk-2.0/nautilus/right_slider_normal.png

darkthemedir = $(datadir)/themes/TerraNova-Aquarius/gtk-2.0
darktheme_DATA = themes/TerraNova-Aquarius/gtk-2.0/gtkrc

darkthemenautidir = $(datadir)/themes/TerraNova-Aquarius/gtk-2.0/nautilus
darkthemenauti_DATA = themes/TerraNova-Aquarius/gtk-2.0/nautilus/nautilus.rc \
		 themes/TerraNova-Aquarius/gtk-2.0/nautilus/breadcrumb_active.png \
		 themes/TerraNova-Aquarius/gtk-2.0/nautilus/breadcrumb_normal.png \
		 themes/TerraNova-Aquarius/gtk-2.0/nautilus/breadcrumb_prelight.png \
		 themes/TerraNova-Aquarius/gtk-2.0/nautilus/left_slider_normal.png \
		 themes/TerraNova-Aquarius/gtk-2.0/nautilus/right_slider_normal.png

candythemedir = $(datadir)/themes/TerraNova-Candy/gtk-2.0
candytheme_DATA = themes/TerraNova-Candy/gtk-2.0/gtkrc

candythemenautidir = $(datadir)/themes/TerraNova-Candy/gtk-2.0/nautilus
candythemenauti_DATA = themes/TerraNova-Candy/gtk-2.0/nautilus/nautilus.rc \
		 themes/TerraNova-Candy/gtk-2.0/nautilus/breadcrumb_active.png \
		 themes/TerraNova-Candy/gtk-2.0/nautilus/breadcrumb_normal.png \
		 themes/TerraNova-Candy/gtk-2.0/nautilus/breadcrumb_prelight.png \
		 themes/TerraNova-Candy/gtk-2.0/nautilus/left_slider_normal.png \
		 themes/TerraNova-Candy/gtk-2.0/nautilus/right_slider_normal.png

pcdir = $(libdir)/pkgconfig/
pc_DATA = terranova.pc

xmldir = $(datadir)/gtk-engines/
xml_DATA = schema/terranova.xml

EXTRA_DIST = pc_DATA \
	     xml_DATA

libterranova_la_SOURCES =		\
	./config.h			\
	./src/animation.c 		\
	./src/animation.h		\
	./src/cairo-support.c	\
	./src/cairo-support.h	\
	./src/terranova_functions.c	\
	./src/terranova_functions.h	\
	./src/terranova_draw.c	\
	./src/terranova_draw_aquarius.c \
	./src/terranova_draw_candy.c \
	./src/terranova_draw.h	\
	./src/terranova_rc_style.c	\
	./src/terranova_rc_style.h	\
	./src/terranova_style.c	\
	./src/terranova_style.h	\
	./src/terranova_theme_main.c	\
	./src/terranova_types.h	\
	./src/general-support.h	\
	./src/ge-support.h		\
	./src/support.c		\
	./src/support.h		\
	./src/widget-information.c	\
	./src/widget-information.h

libterranova_la_LDFLAGS = -module -avoid-version -no-undefined
libterranova_la_LIBADD =  $(GTK_LIBS)
