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
