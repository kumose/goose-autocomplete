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

#include <goose/parser/statement/transaction_statement.h>
#include <goose-autocomplete/transformer/peg_transformer.h>

namespace goose {
    unique_ptr<SQLStatement> PEGTransformerFactory::TransformTransactionStatement(PEGTransformer &transformer,
        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto &choice_pr = list_pr.Child<ChoiceParseResult>(0);
        return transformer.Transform<unique_ptr<TransactionStatement> >(choice_pr.result);
    }

    unique_ptr<TransactionStatement>
    PEGTransformerFactory::TransformBeginTransaction(PEGTransformer &transformer,
                                                     optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto info = make_uniq<TransactionInfo>(TransactionType::BEGIN_TRANSACTION);
        auto &read_or_write = list_pr.Child<OptionalParseResult>(2);
        if (read_or_write.HasResult()) {
            info->modifier = transformer.Transform<TransactionModifierType>(read_or_write.optional_result);
        } else {
            info->modifier = TransactionModifierType::TRANSACTION_DEFAULT_MODIFIER;
        }
        return make_uniq<TransactionStatement>(std::move(info));
    }

    TransactionModifierType PEGTransformerFactory::TransformReadOrWrite(PEGTransformer &transformer,
                                                                        optional_ptr<ParseResult> parse_result) {
        auto &list_pr = parse_result->Cast<ListParseResult>();
        auto &only_or_write = list_pr.Child<ListParseResult>(1);
        auto &only_or_write_choice = only_or_write.Child<ChoiceParseResult>(0);
        return transformer.TransformEnum<TransactionModifierType>(only_or_write_choice.result);
    }

    unique_ptr<TransactionStatement> PEGTransformerFactory::TransformCommitTransaction(PEGTransformer &,
        optional_ptr<ParseResult>) {
        return make_uniq<TransactionStatement>(make_uniq<TransactionInfo>(TransactionType::COMMIT));
    }

    unique_ptr<TransactionStatement> PEGTransformerFactory::TransformRollbackTransaction(PEGTransformer &,
        optional_ptr<ParseResult>) {
        return make_uniq<TransactionStatement>(make_uniq<TransactionInfo>(TransactionType::ROLLBACK));
    }
} // namespace goose
