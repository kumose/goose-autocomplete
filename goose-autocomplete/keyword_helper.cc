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

#include <goose-autocomplete/keyword_helper.h>

namespace goose {
    PEGKeywordHelper &PEGKeywordHelper::Instance() {
        static PEGKeywordHelper instance;
        return instance;
    }

    PEGKeywordHelper::PEGKeywordHelper() {
        InitializeKeywordMaps();
    }

    bool PEGKeywordHelper::KeywordCategoryType(const std::string &text, const PEGKeywordCategory type) const {
        switch (type) {
            case PEGKeywordCategory::KEYWORD_RESERVED: {
                auto it = reserved_keyword_map.find(text);
                return it != reserved_keyword_map.end();
            }
            case PEGKeywordCategory::KEYWORD_UNRESERVED: {
                auto it = unreserved_keyword_map.find(text);
                return it != unreserved_keyword_map.end();
            }
            case PEGKeywordCategory::KEYWORD_TYPE_FUNC: {
                auto it = typefunc_keyword_map.find(text);
                return it != typefunc_keyword_map.end();
            }
            case PEGKeywordCategory::KEYWORD_COL_NAME: {
                auto it = colname_keyword_map.find(text);
                return it != colname_keyword_map.end();
            }
            default:
                return false;
        }
    }
} // namespace goose
