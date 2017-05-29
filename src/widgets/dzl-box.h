/* dzl-box.h
 *
 * Copyright (C) 2015 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DZL_BOX_H
#define DZL_BOX_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DZL_TYPE_BOX (dzl_box_get_type())

G_DECLARE_DERIVABLE_TYPE (DzlBox, dzl_box, DZL, BOX, GtkBox)

struct _DzlBoxClass
{
  GtkBoxClass parent_class;
};

GtkWidget *dzl_box_new                   (void);
gint       dzl_box_get_max_width_request (DzlBox *self);
void       dzl_box_set_max_width_request (DzlBox *self,
                                          gint    max_width_request);

G_END_DECLS

#endif /* DZL_BOX_H */
