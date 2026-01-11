// Copyright (C) Kumo inc. and its affiliates.
// Author: Jeff.li lijippy@163.com
// All rights reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <goose-autocomplete/transformer/peg_transformer.h>
#include <goose/parser/parsed_data/create_view_info.h>

namespace goose {
    unique_ptr<CreateStatement> PEGTransformerFactory::TransformCreateViewStmt(PEGTransformer &transformer,
                                                                               optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        throw NotImplementedException("TransformCreateViewStmt");
        // TODO(Dtenwolde) handle recursive views
        auto if_not_exists = list_pr.Child<OptionalParseResult>(2).HasResult();
        auto qualified_name = transformer.Transform<QualifiedName>(list_pr.Child<ListParseResult>(3));
        auto insert_column_list_pr = list_pr.Child<OptionalParseResult>(4);
        vector<string> column_list;
        if (insert_column_list_pr.HasResult()) {
            column_list = transformer.Transform<vector<string> >(insert_column_list_pr.optional_result);
        }
        auto result = make_uniq<CreateStatement>();
        auto info = make_uniq<CreateViewInfo>();
        info->on_conflict = if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
        info->catalog = qualified_name.catalog;
        info->schema = qualified_name.schema;
        info->view_name = qualified_name.name;
        info->aliases = column_list;
        info->query = transformer.Transform<unique_ptr<SelectStatement> >(list_pr.Child<ListParseResult>(6));
        result->info = std::move(info);
        return result;
    }
} // namespace goose
