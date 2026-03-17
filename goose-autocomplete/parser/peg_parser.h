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
