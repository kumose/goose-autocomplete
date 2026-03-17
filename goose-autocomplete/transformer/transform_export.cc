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


#include <goose/parser/parsed_data/copy_info.h>
#include <goose/parser/statement/export_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformExportStatement(PEGTransformer &transformer,
                                                                             optional_ptr<ParseResult> parse_result) {
        auto info = make_uniq<CopyInfo>();
        auto &list_pr = parse_result->Cast<ListParseResult>();
        info->file_path = list_pr.Child<StringLiteralParseResult>(3).result;
        info->format = "csv";
        info->is_from = false;

        auto &parens = list_pr.Child<OptionalParseResult>(4);
        if (parens.HasResult()) {
            auto &generic_copy_option_list = parens.optional_result->Cast<ListParseResult>().Child<ListParseResult>(1);
            auto option_list = transformer.Transform<unordered_map<string, vector<Value> > >(generic_copy_option_list);
            case_insensitive_map_t<vector<Value> > option_result;
            for (auto &option: option_list) {
                option_result[option.first] = option.second;
            }
            info->options = option_result;
        }

        auto result = make_uniq<ExportStatement>(std::move(info));
        auto database_result = list_pr.Child<OptionalParseResult>(2);
        if (database_result.HasResult()) {
            result->database = transformer.Transform<string>(database_result.optional_result);
        }
        return result;
    }

    string PEGTransformerFactory::TransformExportSource(PEGTransformer &transformer,
                                                        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        return list_pr.Child<IdentifierParseResult>(0).identifier;
    }
} // namespace goose
