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

namespace goose {
    struct TransformEnumValue {
        virtual ~TransformEnumValue() = default;
    };

    template<class T>
    struct TypedTransformEnumResult : public TransformEnumValue {
        explicit TypedTransformEnumResult(T value_p) : value(std::move(value_p)) {
        }

        T value;
    };
} // namespace goose
