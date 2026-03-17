// Copyright (C) DuckDB inc. and its affiliates.
// Copyright (C) Kumo inc. and its affiliates.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
