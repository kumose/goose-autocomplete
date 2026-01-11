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
