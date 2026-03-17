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
#include <goose/parser/qualified_name.h>
#include <goose/parser/parsed_data/create_sequence_info.h>

namespace goose {
    class SequenceOption {
    public:
        SequenceOption(SequenceInfo type_p) : type(type_p) {
        }

    public:
        SequenceInfo type;
    };

    class ValueSequenceOption : public SequenceOption {
    public:
        ValueSequenceOption(SequenceInfo type, Value value_p) : SequenceOption(type), value(value_p) {
        }

    public:
        Value value;
    };

    class QualifiedSequenceOption : public SequenceOption {
    public:
        QualifiedSequenceOption(SequenceInfo type, QualifiedName qualified_name_p)
            : SequenceOption(type), qualified_name(qualified_name_p) {
        }

    public:
        QualifiedName qualified_name;
    };
} // namespace goose
