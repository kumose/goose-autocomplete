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
#include <goose/common/string.h>

namespace goose {
    enum class PEGKeywordCategory : uint8_t {
        KEYWORD_NONE,
        KEYWORD_UNRESERVED,
        KEYWORD_RESERVED,
        KEYWORD_TYPE_FUNC,
        KEYWORD_COL_NAME
    };

    class PEGKeywordHelper {
    public:
        static PEGKeywordHelper &Instance();

        bool KeywordCategoryType(const string &text, PEGKeywordCategory type) const;

        void InitializeKeywordMaps();

    private:
        PEGKeywordHelper();

        bool initialized;
        case_insensitive_set_t reserved_keyword_map;
        case_insensitive_set_t unreserved_keyword_map;
        case_insensitive_set_t colname_keyword_map;
        case_insensitive_set_t typefunc_keyword_map;
    };
} // namespace goose
