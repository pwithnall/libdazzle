/* dzl-fuzzy-index-match.h
 *
 * Copyright (C) 2016 Christian Hergert <christian@hergert.me>
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

#ifndef DZL_FUZZY_INDEX_MATCH_H
#define DZL_FUZZY_INDEX_MATCH_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define DZL_TYPE_FUZZY_INDEX_MATCH (dzl_fuzzy_index_match_get_type())

G_DECLARE_FINAL_TYPE (DzlFuzzyIndexMatch, dzl_fuzzy_index_match, DZL, FUZZY_INDEX_MATCH, GObject)

const gchar *dzl_fuzzy_index_match_get_key      (DzlFuzzyIndexMatch *self);
GVariant    *dzl_fuzzy_index_match_get_document (DzlFuzzyIndexMatch *self);
gfloat       dzl_fuzzy_index_match_get_score    (DzlFuzzyIndexMatch *self);

G_END_DECLS

#endif /* DZL_FUZZY_INDEX_MATCH_H */