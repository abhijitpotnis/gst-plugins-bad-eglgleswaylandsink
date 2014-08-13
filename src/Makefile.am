modlibexecdir=$(GST_MODDIR)

modlibexec_LTLIBRARIES = libgsteglgleswaylandsink.la

libgsteglgleswaylandsink_la_SOURCES =  gsteglgleswaylandsink.c
libgsteglgleswaylandsink_la_CFLAGS = $(GST_CFLAGS) \
                                $(WAYLAND_CFLAGS)
libgsteglgleswaylandsink_la_LIBADD = $(GST_LIBS) \
                                -lgstvideo-0.10 \
                                -lgstinterfaces-0.10 \
                                -lwayland-cursor \
                                $(WAYLAND_LIBS) \
                                $(GL_LIBS)
libgsteglgleswaylandsink_la_LDFLAGS = -avoid-version -module -export-dynamic
libgsteglgleswaylandsink_la_LIBTOOLFLAGS = --tag=disable-static

noinst_HEADERS = gsteglgleswaylandsink.h
