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


#include <goose-autocomplete/transformer/peg_transformer.h>

#include <goose/parser/statement/set_statement.h>
#include <goose/common/string_util.h>

namespace goose {
    void PEGTransformer::ParamTypeCheck(PreparedParamType last_type, PreparedParamType new_type) {
        // Mixing positional/auto-increment and named parameters is not supported
        if (last_type == PreparedParamType::INVALID) {
            return;
        }
        if (last_type == PreparedParamType::NAMED) {
            if (new_type != PreparedParamType::NAMED) {
                throw NotImplementedException("Mixing named and positional parameters is not supported yet");
            }
        }
        if (last_type != PreparedParamType::NAMED) {
            if (new_type == PreparedParamType::NAMED) {
                throw NotImplementedException("Mixing named and positional parameters is not supported yet");
            }
        }
    }

    bool PEGTransformer::GetParam(const string &identifier, idx_t &index, PreparedParamType type) {
        ParamTypeCheck(last_param_type, type);
        auto entry = named_parameter_map.find(identifier);
        if (entry == named_parameter_map.end()) {
            return false;
        }
        index = entry->second;
        return true;
    }

    void PEGTransformer::SetParam(const string &identifier, idx_t index, PreparedParamType type) {
        ParamTypeCheck(last_param_type, type);
        last_param_type = type;
        D_ASSERT(!named_parameter_map.count(identifier));
        named_parameter_map[identifier] = index;
    }

    void PEGTransformer::ClearParameters() {
        prepared_statement_parameter_index = 0;
        named_parameter_map.clear();
    }
} // namespace goose
