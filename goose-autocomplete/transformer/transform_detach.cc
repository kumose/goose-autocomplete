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
