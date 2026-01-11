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
#include <goose/common/case_insensitive_map.h>
#include <goose/common/string_map_set.h>

namespace goose {
    enum class PEGRuleType {
        LITERAL, // literal rule ('Keyword'i)
        REFERENCE, // reference to another rule (Rule)
        OPTIONAL, // optional rule (Rule?)
        OR, // or rule (Rule1 / Rule2)
        REPEAT // repeat rule (Rule1*
    };

    enum class PEGTokenType {
        LITERAL, // literal token ('Keyword'i)
        REFERENCE, // reference token (Rule)
        OPERATOR, // operator token (/ or )
        FUNCTION_CALL, // start of function call (i.e. Function(...))
        REGEX // regular expression ([ \t\n\r] or <[a-z_]i[a-z0-9_]i>)
    };

    struct PEGToken {
        PEGTokenType type;
        string_t text;
    };

    struct PEGRule {
        string_map_t<idx_t> parameters;
        vector<PEGToken> tokens;

        void Clear() {
            parameters.clear();
            tokens.clear();
        }
    };

    struct PEGParser {
    public:
        void ParseRules(const char *grammar);

        void AddRule(string_t rule_name, PEGRule rule);

        case_insensitive_map_t<PEGRule> rules;
    };

    enum class PEGParseState {
        RULE_NAME, // Rule name
        RULE_SEPARATOR, // look for <-
        RULE_DEFINITION // part of rule definition
    };

    inline bool IsPEGOperator(char c) {
        switch (c) {
            case '/':
            case '?':
            case '(':
            case ')':
            case '*':
            case '+':
            case '!':
                return true;
            default:
                return false;
        }
    }
} // namespace goose
