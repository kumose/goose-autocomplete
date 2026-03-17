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
