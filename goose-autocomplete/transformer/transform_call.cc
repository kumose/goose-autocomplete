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
