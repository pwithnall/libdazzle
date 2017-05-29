/* dzl-settings-sandwich.h
 *
 * Copyright (C) 2015 Christian Hergert <christian@hergert.me>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DZL_SETTINGS_SANDWICH_H
#define DZL_SETTINGS_SANDWICH_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define DZL_TYPE_SETTINGS_SANDWICH (dzl_settings_sandwich_get_type())

G_DECLARE_FINAL_TYPE (DzlSettingsSandwich, dzl_settings_sandwich, DZL, SETTINGS_SANDWICH, GObject)

DzlSettingsSandwich *dzl_settings_sandwich_new               (const gchar             *schema_id,
                                                              const gchar             *path);
GVariant            *dzl_settings_sandwich_get_default_value (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
GVariant            *dzl_settings_sandwich_get_user_value    (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
GVariant            *dzl_settings_sandwich_get_value         (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
void                 dzl_settings_sandwich_set_value         (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              GVariant                *value);
gboolean             dzl_settings_sandwich_get_boolean       (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
gdouble              dzl_settings_sandwich_get_double        (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
gint                 dzl_settings_sandwich_get_int           (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
gchar               *dzl_settings_sandwich_get_string        (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
guint                dzl_settings_sandwich_get_uint          (DzlSettingsSandwich     *self,
                                                              const gchar             *key);
void                 dzl_settings_sandwich_set_boolean       (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              gboolean                 val);
void                 dzl_settings_sandwich_set_double        (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              gdouble                  val);
void                 dzl_settings_sandwich_set_int           (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              gint                     val);
void                 dzl_settings_sandwich_set_string        (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              const gchar             *val);
void                 dzl_settings_sandwich_set_uint          (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              guint                    val);
void                 dzl_settings_sandwich_append            (DzlSettingsSandwich     *self,
                                                              GSettings               *settings);
void                 dzl_settings_sandwich_bind              (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              gpointer                 object,
                                                              const gchar             *property,
                                                              GSettingsBindFlags       flags);
void                 dzl_settings_sandwich_bind_with_mapping (DzlSettingsSandwich     *self,
                                                              const gchar             *key,
                                                              gpointer                 object,
                                                              const gchar             *property,
                                                              GSettingsBindFlags       flags,
                                                              GSettingsBindGetMapping  get_mapping,
                                                              GSettingsBindSetMapping  set_mapping,
                                                              gpointer                 user_data,
                                                              GDestroyNotify           destroy);
void                 dzl_settings_sandwich_unbind            (DzlSettingsSandwich     *self,
                                                              const gchar             *property);

G_END_DECLS

#endif /* DZL_SETTINGS_SANDWICH_H */
