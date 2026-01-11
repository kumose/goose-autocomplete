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
