#pragma once

#include <unordered_map>
#include <string>
#include <any>

template<std::size_t... Indices>
struct IndexSequence {
};

template<std::size_t N, std::size_t... Indices>
struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Indices...> {
};

template<std::size_t... Indices>
struct MakeIndexSequence<0, Indices...> : IndexSequence<Indices...> {
};

using ArgsDict = std::unordered_map<std::string, std::any> const &;