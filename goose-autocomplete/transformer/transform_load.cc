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


#include <goose/parser/statement/load_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>
#include <goose-autocomplete/ast/extension_repository_info.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformLoadStatement(PEGTransformer &transformer,
                                                                           optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto result = make_uniq<LoadStatement>();
        auto info = make_uniq<LoadInfo>();
        info->load_type = LoadType::LOAD;
        info->filename = transformer.Transform<string>(list_pr.Child<ListParseResult>(1));
        result->info = std::move(info);
        return result;
    }

    unique_ptr<SQLStatement> PEGTransformerFactory::TransformInstallStatement(PEGTransformer &transformer,
                                                                              optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto result = make_uniq<LoadStatement>();
        auto info = make_uniq<LoadInfo>();
        auto opt_force = list_pr.Child<OptionalParseResult>(0).HasResult();
        info->load_type = opt_force ? LoadType::FORCE_INSTALL : LoadType::INSTALL;
        info->filename = list_pr.Child<IdentifierParseResult>(2).identifier;
        info->repo_is_alias = false;
        auto &from_source_opt = list_pr.Child<OptionalParseResult>(3);
        if (from_source_opt.HasResult()) {
            auto repository_info = transformer.Transform<ExtensionRepositoryInfo>(from_source_opt.optional_result);
            info->repository = repository_info.name;
            info->repo_is_alias = repository_info.repository_is_alias;
        }
        transformer.TransformOptional<string>(list_pr, 4, info->version);
        result->info = std::move(info);
        return result;
    }

    ExtensionRepositoryInfo PEGTransformerFactory::TransformFromSource(PEGTransformer &transformer,
                                                                       optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto &sub_list_pr = list_pr.Child<ListParseResult>(1);
        auto &string_or_identifier = sub_list_pr.Child<ChoiceParseResult>(0);
        ExtensionRepositoryInfo result;
        if (string_or_identifier.result->type == ParseResultType::STRING) {
            result.name = transformer.Transform<string>(string_or_identifier.result);
            result.repository_is_alias = false;
        } else {
            result.name = string_or_identifier.result->Cast<IdentifierParseResult>().identifier;
            result.repository_is_alias = true;
        }
        return result;
    }

    string PEGTransformerFactory::TransformVersionNumber(PEGTransformer &transformer,
                                                         optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        return list_pr.Child<IdentifierParseResult>(1).identifier;
    }
} // namespace goose
