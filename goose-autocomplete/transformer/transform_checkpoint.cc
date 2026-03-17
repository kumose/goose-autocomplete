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
