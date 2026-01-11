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

#include <goose/parser/parsed_data/create_schema_info.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<CreateStatement> PEGTransformerFactory::TransformCreateSchemaStmt(PEGTransformer &transformer,
        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto if_not_exists = list_pr.Child<OptionalParseResult>(1).HasResult();
        auto qualified_name = transformer.Transform<QualifiedName>(list_pr.Child<ListParseResult>(2));
        if (qualified_name.catalog != INVALID_CATALOG) {
            throw ParserException("CREATE SCHEMA too many dots: expected \"catalog.schema\" or \"schema\"");
        }
        auto result = make_uniq<CreateStatement>();
        auto info = make_uniq<CreateSchemaInfo>();
        info->on_conflict = if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
        info->catalog = qualified_name.schema;
        info->schema = qualified_name.name;

        result->info = std::move(info);
        return result;
    }
} // namespace goose
