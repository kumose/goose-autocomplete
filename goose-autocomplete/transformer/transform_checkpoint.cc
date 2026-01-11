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

#include <goose/parser/expression/function_expression.h>
#include <goose/parser/statement/call_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformCheckpointStatement(PEGTransformer &transformer,
        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();

        auto &force_pr = list_pr.Child<OptionalParseResult>(0);
        auto checkpoint_name = force_pr.HasResult() ? "force_checkpoint" : "checkpoint";
        auto result = make_uniq<CallStatement>();
        vector<unique_ptr<ParsedExpression> > children;
        auto function = make_uniq<FunctionExpression>(checkpoint_name, std::move(children));
        function->catalog = SYSTEM_CATALOG;
        function->schema = DEFAULT_SCHEMA;

        auto catalog_name_pr = list_pr.Child<OptionalParseResult>(2);
        if (catalog_name_pr.HasResult()) {
            function->children.push_back(
                make_uniq<ConstantExpression>(
                    catalog_name_pr.optional_result->Cast<IdentifierParseResult>().identifier));
        }
        result->function = std::move(function);
        return result;
    }
} // namespace goose
