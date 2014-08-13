# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# This file is part of gst-eglgleswaylandsink.
#
# gst-pulse is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# gst-pulse is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with gst-pulse; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

AC_PREREQ(2.57)
AC_INIT([gst-lmwayland],[0.1],[Abhijit_Potnis (at) mentor (dot) com])
AC_CONFIG_SRCDIR([src/gsteglgleswaylandsink.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign 1.9 -Wall])

AC_SUBST(PACKAGE_URL, [http://mentor.com/])

if type -p stow > /dev/null && test -d /usr/local/stow ; then
   AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
   ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

# Checks for programs.
AC_PROG_CC
AC_PROG_LN_S
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_CHECK_FUNCS([gethostname]) 
AC_CHECK_HEADERS([limits.h])
AC_C_CONST
AC_FUNC_MALLOC
AC_TYPE_SIZE_T

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(WAYLAND, [wayland-server >= 1.2 wayland-client >= 1.2],
   [AC_DEFINE(HAVE_WAYLAND_1_2_PLUS, [1], [New Wayland interface])],
   [PKG_CHECK_MODULES(WAYLAND, [wayland-server >= 0.95 wayland-client >= 0.95])
])
AC_CHECK_HEADERS([wayland-client.h wayland-client-protocol.h wayland-egl.h])
AC_CHECK_LIB([wayland-client], [wl_display_bind])
AC_CHECK_LIB([wayland-egl], [wl_egl_window_create])
WAYLAND_LIBS="-lwayland-client -lwayland-egl"
AC_SUBST(WAYLAND_LIBS)

AC_CHECK_HEADERS([ilm/ilm_client.h])
AC_CHECK_HEADERS([ilm/ilm_control.h])
AC_CHECK_HEADERS([layermanager/IpcModuleLoader.h])

AC_CHECK_LIB([ilmClient], [ilm_surfaceCreate])
AC_CHECK_LIB([ilmControl], [lm_surfaceInitialize])

AC_ARG_WITH([layermanager-include-path],
            [AS_HELP_STRING([--with-layermanager-include-path],
                [LayerManager headers, defaults to ${prefix}/include/layermanager])],
            [LM_CFLAGS="-I$withval"],
            [LM_CFLAGS='-I${includedir}/layermanager'])
AC_SUBST([LM_CFLAGS])

AC_ARG_WITH([ilm-include-path],
            [AS_HELP_STRING([--with-ilm-include-path],
                [LayerManager clients headers, defaults to ${prefix}/include/ilm])],
            [ILM_CFLAGS="-I$withval"],
            [ILM_CFLAGS='-I${includedir}/ilm'])
AC_SUBST([ILM_CFLAGS])

AC_ARG_WITH([layermanager-lib-path],
            [AS_HELP_STRING([--with-layermanager-lib-path], [LayerManager libraries])],
            [LM_LIBS="-L$withval -lilmClient -lilmCommon -lilmControl"],
            [LM_LIBS='-lilmClient -lilmCommon -lilmControl'])
AC_SUBST([LM_LIBS])

AC_CHECK_LIB([EGL], [eglCreateContext])
AC_CHECK_LIB([GLESv2], [glEGLImageTargetTexture2DOES])
GL_LIBS="-lEGL -lGLESv2"
AC_SUBST(GL_LIBS)

PKG_CHECK_MODULES(GST, [ gstreamer-0.10 >= 0.10 gstreamer-plugins-base-0.10 >= 0.10 ])

GST_MODDIR=`$PKG_CONFIG --variable=pluginsdir gstreamer-0.10`

AC_SUBST(GST_LIBS)
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_MODDIR)

# If using GCC specifiy some additional parameters
if test "x$GCC" = "xyes" ; then
   CFLAGS="$CFLAGS -pipe -Wall -W -Wno-unused-parameter"
fi

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
