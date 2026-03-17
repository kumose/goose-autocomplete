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

#pragma once

#include <goose/parser/expression/constant_expression.h>

#include <goose/common/string.h>
#include <goose/common/types/value.h>

namespace goose {
    struct GenericCopyOption {
        string name;
        vector<Value> children; // Default value
        unique_ptr<ParsedExpression> expression = nullptr;

        GenericCopyOption() {
        };

        GenericCopyOption(const string &name_p, const Value &value) : name(name_p) {
            children.push_back(value);
        }

        GenericCopyOption(const GenericCopyOption &other)
            : name(other.name), children(other.children),
              expression(other.expression ? other.expression->Copy() : nullptr) {
        }

        GenericCopyOption &operator=(const GenericCopyOption &other) {
            if (this == &other) {
                return *this;
            }

            name = other.name;
            children = other.children;
            expression = other.expression ? other.expression->Copy() : nullptr;

            return *this;
        }

        GenericCopyOption(GenericCopyOption &&other) noexcept = default;

        GenericCopyOption &operator=(GenericCopyOption &&other) noexcept = default;

        unique_ptr<ParsedExpression> GetFirstChildOrExpression() {
            if (!children.empty()) {
                return make_uniq<ConstantExpression>(children[0]);
            }
            if (!expression) {
                throw InvalidInputException("No expression in GenericCopyOption");
            }
            return expression->Copy();
        }
    };
} // namespace goose
