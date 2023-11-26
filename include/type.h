#pragma once

#include <string>

template <typename DataType>
std::string getType() {
    if constexpr (std::is_same_v<DataType, float>) {
       return "F32";
    } else if constexpr (std::is_same_v<DataType, double>) {
        return "F64";
    } else if constexpr (std::is_same_v<DataType, int32_t>) {
        return "I32";
    } else if constexpr (std::is_same_v<DataType, uint32_t>) {
        return "UI32";
    } else if constexpr (std::is_same_v<DataType, uint64_t>) {
        return "UI64";
    }
    return "void";
}
