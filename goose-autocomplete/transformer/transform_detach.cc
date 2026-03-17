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


#include <goose/parser/statement/detach_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformDetachStatement(PEGTransformer &transformer,
                                                                             optional_ptr<ParseResult> parse_result) {
        auto result = make_uniq<DetachStatement>();
        auto info = make_uniq<DetachInfo>();

        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto &if_exists = list_pr.Child<OptionalParseResult>(2);
        info->if_not_found = if_exists.optional_result
                                 ? OnEntryNotFound::RETURN_NULL
                                 : OnEntryNotFound::THROW_EXCEPTION;
        info->name = list_pr.Child<IdentifierParseResult>(3).identifier;
        result->info = std::move(info);
        return result;
    }
} // namespace goose
