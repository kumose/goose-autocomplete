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

#include <goose-autocomplete/transformer/peg_transformer.h>
#include <goose/parser/parsed_data/create_type_info.h>

namespace goose {
    unique_ptr<CreateStatement> PEGTransformerFactory::TransformCreateTypeStmt(PEGTransformer &transformer,
                                                                               optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto result = make_uniq<CreateStatement>();
        auto if_not_exists = list_pr.Child<OptionalParseResult>(1).HasResult();
        auto qualified_name = transformer.Transform<QualifiedName>(list_pr.Child<ListParseResult>(2));
        auto create_type_info = transformer.Transform<unique_ptr<CreateTypeInfo> >(list_pr.Child<ListParseResult>(4));
        create_type_info->catalog = qualified_name.catalog;
        create_type_info->schema = qualified_name.schema;
        create_type_info->name = qualified_name.name;
        create_type_info->on_conflict =
                if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
        result->info = std::move(create_type_info);
        return result;
    }

    unique_ptr<CreateTypeInfo> PEGTransformerFactory::TransformCreateType(PEGTransformer &transformer,
                                                                          optional_ptr<ParseResult> parse_result) {
        auto result = make_uniq<CreateTypeInfo>();
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto choice_pr = list_pr.Child<ChoiceParseResult>(0);
        if (choice_pr.result->name == "EnumSelectType") {
            result->query = transformer.Transform<unique_ptr<SelectStatement> >(choice_pr.result);
            result->type = LogicalType::INVALID;
        } else {
            result->type = transformer.Transform<LogicalType>(choice_pr.result);
        }
        return result;
    }

    unique_ptr<SelectStatement> PEGTransformerFactory::TransformEnumSelectType(PEGTransformer &transformer,
                                                                               optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto extract_parens = ExtractResultFromParens(list_pr.Child<ListParseResult>(1));
        return transformer.Transform<unique_ptr<SelectStatement> >(extract_parens);
    }

    LogicalType PEGTransformerFactory::TransformEnumStringLiteralList(PEGTransformer &transformer,
                                                                      optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto extract_parens = ExtractResultFromParens(list_pr.Child<ListParseResult>(1));
        auto string_literal_list = ExtractParseResultsFromList(extract_parens);

        Vector enum_vector(LogicalType::VARCHAR, string_literal_list.size());
        auto string_data = FlatVector::GetData<string_t>(enum_vector);
        idx_t pos = 0;
        for (auto string_literal: string_literal_list) {
            string_data[pos++] =
                    StringVector::AddString(enum_vector, string_literal->Cast<StringLiteralParseResult>().result);
        }
        return LogicalType::ENUM(enum_vector, string_literal_list.size());
    }
} // namespace goose
