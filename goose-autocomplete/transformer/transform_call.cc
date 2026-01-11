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

#include <goose/function/function.h>
#include <goose/parser/expression/function_expression.h>
#include <goose/parser/statement/call_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformCallStatement(PEGTransformer &transformer,
                                                                           optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto table_function_name = list_pr.Child<IdentifierParseResult>(1).identifier;
        auto function_children =
                transformer.Transform<vector<unique_ptr<ParsedExpression> > >(list_pr.Child<ListParseResult>(2));
        auto result = make_uniq<CallStatement>();
        auto function_expression = make_uniq<FunctionExpression>(table_function_name, std::move(function_children));
        result->function = std::move(function_expression);
        return result;
    }
} // namespace goose
