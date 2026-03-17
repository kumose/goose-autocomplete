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
