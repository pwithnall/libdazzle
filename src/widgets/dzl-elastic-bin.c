/* dzl-elastic-bin.c
 *
 * Copyright (C) 2017 Christian Hergert <chergert@redhat.com>
 *
 * This file is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define G_LOG_DOMAIN "dzl-elastic-bin"

#include "animation/dzl-animation.h"
#include "widgets/dzl-elastic-bin.h"

#if 0
# define _TRACE_LEVEL (1<<G_LOG_LEVEL_USER_SHIFT)
# define _TRACE(...) do { g_log(G_LOG_DOMAIN, _TRACE_LEVEL, __VA_ARGS__); } while (0)
# define TRACE_MSG(m,...) _TRACE("   MSG: %s():%u: "m, G_STRFUNC, __LINE__, __VA_ARGS__)
# define ENTRY _TRACE(" ENTRY: %s(): %u", G_STRFUNC, __LINE__)
# define EXIT do { _TRACE("  EXIT: %s(): %u", G_STRFUNC, __LINE__); return; } while (0)
# define RETURN(r) do { _TRACE("  EXIT: %s(): %u", G_STRFUNC, __LINE__); return (r); } while (0)
#else
# define TRACE_MSG(m,...) do { } while (0)
# define ENTRY            do { } while (0)
# define EXIT             return
# define RETURN(r)        return (r)
#endif

typedef struct
{
  GtkAdjustment *hadj;
  DzlAnimation  *hanim;
  gint           cached_min_height;
  gint           cached_nat_height;
} DzlElasticBinPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (DzlElasticBin, dzl_elastic_bin, GTK_TYPE_BIN)

static void
dzl_elastic_bin_cancel_animation (DzlElasticBin *self)
{
  DzlElasticBinPrivate *priv = dzl_elastic_bin_get_instance_private (self);

  ENTRY;

  g_assert (DZL_IS_ELASTIC_BIN (self));

  if (priv->hanim != NULL)
    {
      DzlAnimation *anim = priv->hanim;

      g_object_remove_weak_pointer (G_OBJECT (priv->hanim), (gpointer *)&priv->hanim);
      priv->hanim = NULL;

      dzl_animation_stop (anim);
    }

  EXIT;
}

static guint
dzl_elastic_bin_calculate_duration (DzlElasticBin *self,
                                    gdouble        from_value,
                                    gdouble        to_value)
{
  GdkDisplay *display;
  GdkMonitor *monitor;
  GdkWindow *window;

  g_assert (DZL_IS_ELASTIC_BIN (self));
  g_assert (from_value >= 0.0);
  g_assert (to_value >= 0.0);

  if (NULL != (display = gtk_widget_get_display (GTK_WIDGET (self))) &&
      NULL != (window = gtk_widget_get_window (GTK_WIDGET (self))) &&
      NULL != (monitor = gdk_display_get_monitor_at_window (display, window)))
    return dzl_animation_calculate_duration (monitor, from_value, to_value);

  return 0;
}

static void
dzl_elastic_bin_animate_to (DzlElasticBin *self,
                            gdouble        value)
{
  DzlElasticBinPrivate *priv = dzl_elastic_bin_get_instance_private (self);
  guint duration;

  ENTRY;

  g_assert (DZL_IS_ELASTIC_BIN (self));

  dzl_elastic_bin_cancel_animation (self);

  duration = dzl_elastic_bin_calculate_duration (self,
                                                 gtk_adjustment_get_value (priv->hadj),
                                                 value);

  TRACE_MSG ("Duration is %u milliseconds", duration);

  priv->hanim = dzl_object_animate (priv->hadj,
                                    DZL_ANIMATION_EASE_OUT_CUBIC,
                                    duration,
                                    gtk_widget_get_frame_clock (GTK_WIDGET (self)),
                                    "value", value,
                                    NULL);
  g_object_add_weak_pointer (G_OBJECT (priv->hanim), (gpointer *)&priv->hanim);

  EXIT;
}

static void
dzl_elastic_bin_get_preferred_height_for_width (GtkWidget *widget,
                                                gint       width,
                                                gint      *min_height,
                                                gint      *nat_height)
{
  DzlElasticBin *self = (DzlElasticBin *)widget;
  DzlElasticBinPrivate *priv = dzl_elastic_bin_get_instance_private (self);

  ENTRY;

  TRACE_MSG ("width=%d", width);

  g_assert (DZL_IS_ELASTIC_BIN (self));

  /*
   * We must always chain up to the parent get_preferred_height_for_width()
   * so that we can detect changes while we are animating.
   */

  GTK_WIDGET_CLASS (dzl_elastic_bin_parent_class)->get_preferred_height_for_width (widget, width, min_height, nat_height);

  /*
   * If we are animating the widget, and the size request hasn't changed since
   * our last animation frame, go ahead and process that now.
   */

  if (*min_height == priv->cached_min_height &&
      *nat_height == priv->cached_nat_height &&
      priv->hanim != NULL)
    {
      *min_height = priv->cached_min_height;
      *nat_height = (gint)gtk_adjustment_get_value (priv->hadj);

      TRACE_MSG ("Fast path min=%d nat=%d", *min_height, *nat_height);

      if (*nat_height == priv->cached_nat_height)
        dzl_elastic_bin_cancel_animation (self);

      EXIT;
    }

  if (*min_height != priv->cached_min_height || *nat_height != priv->cached_nat_height)
    {
      priv->cached_min_height = *min_height;
      priv->cached_nat_height = *nat_height;

      TRACE_MSG ("New requested height is min=%d nat=%d",
                 *min_height, *nat_height);

      if (*min_height > (gint)gtk_adjustment_get_value (priv->hadj))
        gtk_adjustment_set_value (priv->hadj, *min_height);

      *min_height = priv->cached_min_height;
      *nat_height = (gint)gtk_adjustment_get_value (priv->hadj);

      dzl_elastic_bin_animate_to (self, priv->cached_nat_height);

      TRACE_MSG ("!!! min=%d nat=%d", *min_height, *nat_height);

      EXIT;
    }

  TRACE_MSG ("*** min=%d nat=%d", *min_height, *nat_height);

  EXIT;
}

static void
dzl_elastic_bin_hadj_value_changed (DzlElasticBin *self,
                                    GtkAdjustment *adj)
{
  ENTRY;

  g_assert (DZL_IS_ELASTIC_BIN (self));
  g_assert (GTK_IS_ADJUSTMENT (adj));

  gtk_widget_queue_resize (GTK_WIDGET (self));

  EXIT;
}

static void
dzl_elastic_bin_size_allocate (GtkWidget     *widget,
                               GtkAllocation *allocation)
{
  g_assert (GTK_IS_WIDGET (widget));
  g_assert (allocation != NULL);

  TRACE_MSG ("Allocating %d,%d %dx%d",
             allocation->x, allocation->y,
             allocation->width, allocation->height);

  GTK_WIDGET_CLASS (dzl_elastic_bin_parent_class)->size_allocate (widget, allocation);
}

static GtkSizeRequestMode
dzl_elastic_bin_get_request_mode (GtkWidget *widget)
{
  return GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH;
}

static void
dzl_elastic_bin_destroy (GtkWidget *widget)
{
  DzlElasticBin *self = (DzlElasticBin *)widget;

  g_assert (DZL_IS_ELASTIC_BIN (self));

  dzl_elastic_bin_cancel_animation (self);

  GTK_WIDGET_CLASS (dzl_elastic_bin_parent_class)->destroy (widget);
}

static void
dzl_elastic_bin_finalize (GObject *object)
{
  DzlElasticBin *self = (DzlElasticBin *)object;
  DzlElasticBinPrivate *priv = dzl_elastic_bin_get_instance_private (self);

  g_clear_object (&priv->hadj);

  G_OBJECT_CLASS (dzl_elastic_bin_parent_class)->finalize (object);
}

static void
dzl_elastic_bin_class_init (DzlElasticBinClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = dzl_elastic_bin_finalize;

  widget_class->destroy = dzl_elastic_bin_destroy;
  widget_class->get_preferred_height_for_width = dzl_elastic_bin_get_preferred_height_for_width;
  widget_class->size_allocate = dzl_elastic_bin_size_allocate;
  widget_class->get_request_mode = dzl_elastic_bin_get_request_mode;

  gtk_widget_class_set_css_name (widget_class, "elastic");
}

static void
dzl_elastic_bin_init (DzlElasticBin *self)
{
  DzlElasticBinPrivate *priv = dzl_elastic_bin_get_instance_private (self);

  priv->hadj = gtk_adjustment_new (0, 0, G_MAXINT, 1, 1, 1);

  g_signal_connect_object (priv->hadj,
                           "value-changed",
                           G_CALLBACK (dzl_elastic_bin_hadj_value_changed),
                           self,
                           G_CONNECT_SWAPPED);
}
