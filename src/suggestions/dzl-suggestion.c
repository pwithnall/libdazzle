/* dzl-suggestion.c
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

#define G_LOG_DOMAIN "dzl-suggestion"

#include "dzl-suggestion.h"

typedef struct
{
  gchar *title;
  gchar *subtitle;
  gchar *icon_name;
  gchar *id;
} DzlSuggestionPrivate;

enum {
  PROP_0,
  PROP_ICON_NAME,
  PROP_ID,
  PROP_SUBTITLE,
  PROP_TITLE,
  N_PROPS
};

enum {
  REPLACE_TYPED_TEXT,
  SUGGEST_SUFFIX,
  N_SIGNALS
};

G_DEFINE_TYPE_WITH_PRIVATE (DzlSuggestion, dzl_suggestion, G_TYPE_OBJECT)

static GParamSpec *properties [N_PROPS];
static guint signals [N_SIGNALS];

static void
dzl_suggestion_finalize (GObject *object)
{
  DzlSuggestion *self = (DzlSuggestion *)object;
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_clear_pointer (&priv->title, g_free);
  g_clear_pointer (&priv->subtitle, g_free);
  g_clear_pointer (&priv->icon_name, g_free);
  g_clear_pointer (&priv->id, g_free);

  G_OBJECT_CLASS (dzl_suggestion_parent_class)->finalize (object);
}

static void
dzl_suggestion_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  DzlSuggestion *self = DZL_SUGGESTION (object);

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_string (value, dzl_suggestion_get_id (self));
      break;

    case PROP_ICON_NAME:
      g_value_set_string (value, dzl_suggestion_get_icon_name (self));
      break;

    case PROP_TITLE:
      g_value_set_string (value, dzl_suggestion_get_title (self));
      break;

    case PROP_SUBTITLE:
      g_value_set_string (value, dzl_suggestion_get_subtitle (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
dzl_suggestion_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  DzlSuggestion *self = DZL_SUGGESTION (object);

  switch (prop_id)
    {
    case PROP_ICON_NAME:
      dzl_suggestion_set_icon_name (self, g_value_get_string (value));
      break;

    case PROP_ID:
      dzl_suggestion_set_id (self, g_value_get_string (value));
      break;

    case PROP_TITLE:
      dzl_suggestion_set_title (self, g_value_get_string (value));
      break;

    case PROP_SUBTITLE:
      dzl_suggestion_set_subtitle (self, g_value_get_string (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
dzl_suggestion_class_init (DzlSuggestionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = dzl_suggestion_finalize;
  object_class->get_property = dzl_suggestion_get_property;
  object_class->set_property = dzl_suggestion_set_property;

  properties [PROP_ID] =
    g_param_spec_string ("id",
                         "Id",
                         "The suggestion identifier",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS));

  properties [PROP_ICON_NAME] =
    g_param_spec_string ("icon-name",
                         "Icon Name",
                         "The name of the icon to display",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS));

  properties [PROP_TITLE] =
    g_param_spec_string ("title",
                         "Title",
                         "The title of the suggestion",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS));

  properties [PROP_SUBTITLE] =
    g_param_spec_string ("subtitle",
                         "Subtitle",
                         "The subtitle of the suggestion",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [REPLACE_TYPED_TEXT] =
    g_signal_new ("replace-typed-text",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (DzlSuggestionClass, replace_typed_text),
                  g_signal_accumulator_first_wins, NULL, NULL,
                  G_TYPE_STRING, 1, G_TYPE_STRING | G_SIGNAL_TYPE_STATIC_SCOPE);

  signals [SUGGEST_SUFFIX] =
    g_signal_new ("suggest-suffix",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (DzlSuggestionClass, suggest_suffix),
                  g_signal_accumulator_first_wins, NULL, NULL,
                  G_TYPE_STRING, 1, G_TYPE_STRING | G_SIGNAL_TYPE_STATIC_SCOPE);
}

static void
dzl_suggestion_init (DzlSuggestion *self)
{
}

const gchar *
dzl_suggestion_get_id (DzlSuggestion *self)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);

  return priv->id;
}

const gchar *
dzl_suggestion_get_icon_name (DzlSuggestion *self)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);

  return priv->icon_name;
}

const gchar *
dzl_suggestion_get_title (DzlSuggestion *self)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);

  return priv->title;
}

const gchar *
dzl_suggestion_get_subtitle (DzlSuggestion *self)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);

  return priv->subtitle;
}

void
dzl_suggestion_set_icon_name (DzlSuggestion *self,
                              const gchar   *icon_name)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_if_fail (DZL_IS_SUGGESTION (self));

  if (g_strcmp0 (priv->icon_name, icon_name) != 0)
    {
      g_free (priv->icon_name);
      priv->icon_name = g_strdup (icon_name);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_ICON_NAME]);
    }
}

void
dzl_suggestion_set_id (DzlSuggestion *self,
                       const gchar   *id)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_if_fail (DZL_IS_SUGGESTION (self));

  if (g_strcmp0 (priv->id, id) != 0)
    {
      g_free (priv->id);
      priv->id = g_strdup (id);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_ID]);
    }
}

void
dzl_suggestion_set_title (DzlSuggestion *self,
                          const gchar   *title)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_if_fail (DZL_IS_SUGGESTION (self));

  if (g_strcmp0 (priv->title, title) != 0)
    {
      g_free (priv->title);
      priv->title = g_strdup (title);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_TITLE]);
    }
}

void
dzl_suggestion_set_subtitle (DzlSuggestion *self,
                             const gchar   *subtitle)
{
  DzlSuggestionPrivate *priv = dzl_suggestion_get_instance_private (self);

  g_return_if_fail (DZL_IS_SUGGESTION (self));

  if (g_strcmp0 (priv->subtitle, subtitle) != 0)
    {
      g_free (priv->subtitle);
      priv->subtitle = g_strdup (subtitle);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_SUBTITLE]);
    }
}

/**
 * dzl_suggestion_suggest_suffix:
 * @self: a #DzlSuggestion
 * @typed_text: The user entered text
 *
 * This function requests potential text to append to @typed_text to make it
 * more clear to the user what they will be activating by selecting this
 * suggestion. For example, if they start typing "gno", a potential suggested
 * suffix might be "me.org" to create "gnome.org".
 *
 * Returns: (transfer full) (nullable): Suffix to append to @typed_text
 *   or %NULL to leave it unchanged.
 */
gchar *
dzl_suggestion_suggest_suffix (DzlSuggestion *self,
                               const gchar   *typed_text)
{
  gchar *ret = NULL;

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);
  g_return_val_if_fail (typed_text != NULL, NULL);

  g_signal_emit (self, signals [SUGGEST_SUFFIX], 0, typed_text, &ret);

  return ret;
}

DzlSuggestion *
dzl_suggestion_new (void)
{
  return g_object_new (DZL_TYPE_SUGGESTION, NULL);
}

/**
 * dzl_suggestion_replace_typed_text:
 * @self: An #DzlSuggestion
 * @typed_text: the text that was typed into the entry
 *
 * This function is meant to be used to replace the text in the entry with text
 * that represents the suggestion most accurately. This happens when the user
 * presses tab while typing a suggestion. For example, if typing "gno" in the
 * entry, you might have a suggest_suffix of "me.org" so that the user sees
 * "gnome.org". But the replace_typed_text might include more data such as
 * "https://gnome.org" as it more closely represents the suggestion.
 *
 * Returns: (transfer full) (nullable): The replacement text to insert into
 *   the entry when "tab" is pressed to complete the insertion.
 */
gchar *
dzl_suggestion_replace_typed_text (DzlSuggestion *self,
                                   const gchar   *typed_text)
{
  gchar *ret = NULL;

  g_return_val_if_fail (DZL_IS_SUGGESTION (self), NULL);

  g_signal_emit (self, signals [REPLACE_TYPED_TEXT], 0, typed_text, &ret);

  return ret;
}
