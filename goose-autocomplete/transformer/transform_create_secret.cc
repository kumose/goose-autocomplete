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

#include <goose/parser/parsed_data/create_secret_info.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<CreateStatement> PEGTransformerFactory::TransformCreateSecretStmt(PEGTransformer &transformer,
        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto result = make_uniq<CreateStatement>();
        auto if_not_exists = list_pr.Child<OptionalParseResult>(1).HasResult();
        auto on_conflict = if_not_exists ? OnCreateConflict::IGNORE_ON_CONFLICT : OnCreateConflict::ERROR_ON_CONFLICT;
        auto info = make_uniq<CreateSecretInfo>(on_conflict, SecretPersistType::DEFAULT);
        auto secret_name_pr = list_pr.Child<OptionalParseResult>(2);
        if (secret_name_pr.HasResult()) {
            info->name = transformer.Transform<string>(secret_name_pr.optional_result);
        }
        auto secret_storage_specifier_pr = list_pr.Child<OptionalParseResult>(3);
        if (secret_storage_specifier_pr.HasResult()) {
            info->storage_type = transformer.Transform<string>(secret_storage_specifier_pr.optional_result);
        }
        auto options_pr = list_pr.Child<ListParseResult>(4);
        auto generic_options_list = ExtractResultFromParens(options_pr);
        auto option_list = transformer.Transform<vector<GenericCopyOption> >(generic_options_list);
        for (auto option: option_list) {
            auto lower_name = StringUtil::Lower(option.name);
            if (lower_name == "scope") {
                info->scope = option.GetFirstChildOrExpression();
            } else if (lower_name == "type") {
                info->type = option.GetFirstChildOrExpression();
            } else if (lower_name == "provider") {
                info->provider = option.GetFirstChildOrExpression();
            } else {
                info->options.insert({lower_name, option.GetFirstChildOrExpression()});
            }
        }
        result->info = std::move(info);
        return result;
    }

    string PEGTransformerFactory::TransformSecretName(PEGTransformer &transformer,
                                                      optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        return transformer.Transform<string>(list_pr.Child<ListParseResult>(0));
    }

    string PEGTransformerFactory::TransformSecretStorageSpecifier(PEGTransformer &transformer,
                                                                  optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        return list_pr.Child<IdentifierParseResult>(1).identifier;
    }
} // namespace goose
