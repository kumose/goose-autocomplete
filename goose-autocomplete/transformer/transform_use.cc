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

#include <goose-autocomplete/transformer/peg_transformer.h>
#include <goose/parser/sql_statement.h>

namespace goose {
    // UseStatement <- 'USE' UseTarget
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformUseStatement(PEGTransformer &transformer,
                                                                          optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto qn = transformer.Transform<QualifiedName>(list_pr, 1);

        string value_str;
        if (IsInvalidSchema(qn.schema)) {
            value_str = KeywordHelper::WriteOptionallyQuoted(qn.name);
        } else {
            value_str =
                    KeywordHelper::WriteOptionallyQuoted(qn.schema) + "." + KeywordHelper::WriteOptionallyQuoted(
                        qn.name);
        }

        auto value_expr = make_uniq<ConstantExpression>(Value(value_str));
        return make_uniq<SetVariableStatement>("schema", std::move(value_expr), SetScope::AUTOMATIC);
    }

    // UseTarget <- (CatalogName '.' ReservedSchemaName) / SchemaName / CatalogName
    QualifiedName PEGTransformerFactory::TransformUseTarget(PEGTransformer &transformer,
                                                            optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto &choice_pr = list_pr.Child<ChoiceParseResult>(0);
        QualifiedName result;
        if (choice_pr.result->type == ParseResultType::LIST) {
            vector<string> entries;
            auto use_target_children = choice_pr.result->Cast<ListParseResult>();
            for (auto &child: use_target_children.GetChildren()) {
                if (child->type == ParseResultType::IDENTIFIER) {
                    entries.push_back(child->Cast<IdentifierParseResult>().identifier);
                }
            }
            if (entries.size() == 2) {
                result.catalog = INVALID_CATALOG;
                result.schema = entries[0];
                result.name = entries[1];
            } else {
                throw InternalException("Invalid amount of entries for use statement");
            }
        } else if (choice_pr.result->type == ParseResultType::IDENTIFIER) {
            result.name = choice_pr.result->Cast<IdentifierParseResult>().identifier;
        } else {
            throw InternalException("Unexpected parse result type encountered in UseTarget");
        }
        return result;
    }
} // namespace goose
