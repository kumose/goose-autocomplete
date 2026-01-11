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
