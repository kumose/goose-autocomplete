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
