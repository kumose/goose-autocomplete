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

#include <goose/parser/statement/drop_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformDeallocateStatement(PEGTransformer &transformer,
        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto result = make_uniq<DropStatement>();
        result->info->type = CatalogType::PREPARED_STATEMENT;
        result->info->name = list_pr.Child<IdentifierParseResult>(2).identifier;
        return result;
    }
} // namespace goose
