/*
 * GStreamer EGL/GLES Wayland Sink
 * Copyright (C) 2012 Collabora Ltd.
 *   @author: Reynaldo H. Verdejo Pinochet <reynaldo@collabora.com>
 *   @author: Sebastian Dröge <sebastian.droege@collabora.co.uk>
 * Copyright (C) 2014 Mentor Graphics Ltd.
 *   @author: Abhijit Potnis <abhijit_potnis@mentor.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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
 */

#ifndef __GST_EGLGLESSINK_H__
#define __GST_EGLGLESSINK_H__

#include <gst/gst.h>
#include <gst/video/gstvideosink.h>
#include <gst/base/gstdataqueue.h>

/* GLES - EGL Includes */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/* Wayland Includes */
#include <wayland-cursor.h>
#include <wayland-egl.h>
#include <wayland-client.h>

/* Wayland - EGL Defines */
#ifndef EGL_EXT_swap_buffers_with_damage
#define EGL_EXT_swap_buffers_with_damage 1
typedef EGLBoolean (EGLAPIENTRYP PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
#endif


#define DEFAULT_SURFACE 10
#define DEFAULT_OPACITY 1.0
#define DEFAULT_ORIENTATION ILM_ZERO
#define DEFAULT_DST_SIZE 0
#define DEFAULT_DST_POS 0

G_BEGIN_DECLS
#define GST_TYPE_EGLGLESSINK \
  (gst_eglglessink_get_type())
#define GST_EGLGLESSINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_EGLGLESSINK,GstEglGlesSink))
#define GST_EGLGLESSINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_EGLGLESSINK,GstEglGlesSinkClass))
#define GST_IS_EGLGLESSINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_EGLGLESSINK))
#define GST_IS_EGLGLESSINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_EGLGLESSINK))
#define GST_EGLGLESSINK_IMAGE_NOFMT 0
#define GST_EGLGLESSINK_IMAGE_RGB888 1
#define GST_EGLGLESSINK_IMAGE_RGB565 2
#define GST_EGLGLESSINK_IMAGE_RGBA8888 3
#define GST_EGLGLESSINK_EGL_MIN_VERSION 1
typedef struct _GstEglGlesSink GstEglGlesSink;
typedef struct _GstEglGlesSinkClass GstEglGlesSinkClass;
typedef struct _GstEglGlesRenderContext GstEglGlesRenderContext;

typedef struct _GstEglGlesImageFmt GstEglGlesImageFmt;

typedef struct _coord5
{
  float x;
  float y;
  float z;
  float a;                      /* texpos x */
  float b;                      /* texpos y */
} coord5;

struct window;

struct display {
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_shell *shell;
	struct wl_seat *seat;
	struct wl_pointer *pointer;
	struct wl_touch *touch;
	struct wl_keyboard *keyboard;
	struct wl_shm *shm;
	struct wl_cursor_theme *cursor_theme;
	struct wl_cursor *default_cursor;
	struct wl_surface *cursor_surface;
	struct window *window;

	PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC swap_buffers_with_damage;
};

struct geometry {
	int width, height;
};


struct window {
	struct display *display;
	struct geometry geometry, window_size;
	struct {
		GLuint rotation_uniform;
		GLuint pos;
		GLuint col;
	} gl;

	uint32_t benchmark_time, frames;
	struct wl_egl_window *native;
	struct wl_surface *surface;
	struct wl_shell_surface *shell_surface;
	EGLSurface egl_surface;
	struct wl_callback *callback;
	int fullscreen, configured, opaque, buffer_size, frame_sync;
};

/*
 * GstEglGlesRenderContext:
 * @config: Current EGL config
 * @eglcontext: Current EGL context
 * @display: Current EGL display connection
 * @window: Current EGL window asociated with the display connection
 * @used_window: Last seen EGL window asociated with the display connection
 * @surface: EGL surface the sink is rendering into
 * @fragshader: Fragment shader
 * @vertshader: Vertex shader
 * @glslprogram: Compiled and linked GLSL program in use for rendering
 * @texture Texture units in use
 * @surface_width: Pixel width of the surface the sink is rendering into
 * @surface_height: Pixel height of the surface the sink is rendering into
 * @pixel_aspect_ratio: EGL display aspect ratio
 * @egl_minor: EGL version (minor)
 * @egl_major: EGL version (major)
 * @n_textures: Texture units count
 * @position_loc: Index of the position vertex attribute array
 * @texpos_loc: Index of the textpos vertex attribute array
 * @position_array: VBO position array
 * @texpos_array: VBO texpos array
 * @index_array: VBO index array
 * @position_buffer: Position buffer object name
 * @texpos_buffer: Texpos buffer object name
 * @index_buffer: Index buffer object name
 *
 * This struct holds the sink's EGL/GLES rendering context.
 */
struct _GstEglGlesRenderContext
{
  EGLConfig config;
  EGLContext eglcontext;
  EGLDisplay egl_display;
  EGLNativeWindowType native_window, used_window;
  EGLSurface surface;
  gboolean buffer_preserved;
  GLuint fragshader[2], vertshader[2], glslprogram[2];
  GLuint texture[3];
  EGLint surface_width;
  EGLint surface_height;
  EGLint pixel_aspect_ratio;
  EGLint egl_minor, egl_major;
  gint n_textures;

  /* shader vars */
  GLuint position_loc[2], texpos_loc;
  GLuint tex_loc[3];
  coord5 position_array[12];    /* 3 x Frame, 3 x Border1, 3 x Border2 */
  unsigned short index_array[4];
  unsigned int position_buffer, index_buffer;
};

/*
 * GstEglGlesImageFmt:
 * @fmt: Internal identifier for the EGL attribs / GST caps pairing
 * @attribs: Pointer to the set of EGL attributes asociated with this format
 * @caps: Pointer to the GST caps asociated with this format
 *
 * This struct holds a pairing between GST caps and the matching EGL attributes
 * associated with a given pixel format
 */
struct _GstEglGlesImageFmt
{
  gint fmt;                     /* Private identifier */
  const EGLint *attribs;        /* EGL Attributes */
  GstCaps *caps;                /* Matching caps for the attribs */
};

/*
 * GstEglGlesSink:
 * @par_n: Incoming frame's aspect ratio numerator
 * @par_d: Incoming frame's aspect ratio denominator
 * @format: Caps' video format field
 * @display_region: Surface region to use as rendering canvas
 * @sinkcaps: Full set of suported caps
 * @current_caps: Current caps
 * @selected_fmt: Pointer to the GST caps/EGL attribs pairing in use
 * @rendering_path: Rendering path (Slow/Fast)
 * @eglglesctx: Pointer to the associated EGL/GLESv2 rendering context
 * @flow_lock: Simple concurrent access ward to the sink's runtime state
 * @supported_fmts: Pointer to the runtime supported format list
 * @have_window: Set if the sink has access to a window to hold it's canvas
 * @using_own_window: Set if the sink created its own window
 * @have_surface: Set if the EGL surface setup has been performed
 * @have_vbo: Set if the GLES VBO setup has been performed
 * @have_texture: Set if the GLES texture setup has been performed
 * @egl_started: Set if the whole EGL setup has been performed
 * @create_window: Property value holder to allow/forbid internal window creation
 * @force_rendering_slow: Property value holder to force slow rendering path
 * @force_aspect_ratio: Property value holder to consider PAR/DAR when scaling
 *
 * The #GstEglGlesSink data structure.
 */
struct _GstEglGlesSink
{
  GstVideoSink videosink;       /* Element hook */
  int par_n, par_d;             /* Aspect ratio from caps */

  GstVideoFormat format;
  GstVideoRectangle display_region;
  GstCaps *sinkcaps;
  GstCaps *current_caps, *configured_caps;

  GstEglGlesImageFmt *selected_fmt;
  GstEglGlesRenderContext eglglesctx;
  struct display display;
  struct window  window;

  GList *supported_fmts;

  /* Runtime flags */
  gboolean have_window;
  gboolean using_own_window;
  gboolean have_surface;;
  gboolean have_vbo;
  gboolean have_texture;
  gboolean egl_started;

  GThread *thread;
  gboolean thread_running;
  GstDataQueue *queue;
  GCond *render_cond;
  GMutex *render_lock;
  GstFlowReturn last_flow;

  /* Properties */
  gboolean create_window;
  gboolean force_aspect_ratio;

};

struct _GstEglGlesSinkClass
{
  GstVideoSinkClass parent_class;
};

GType gst_eglglessink_get_type (void);

G_END_DECLS
#endif /* __GST_EGLGLESSINK_H__ */
