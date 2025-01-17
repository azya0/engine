#pragma once

#include <any>
#include <stdexcept>

#include "utils.h"

template<typename ClassT, typename ReturnT, typename... ArgsT>
class Wrapper {
private:
    ClassT* instance;

    ReturnT (ClassT::*function)(ArgsT...);

    std::vector<std::string> const arguments;

    std::vector<std::any> validateArgs(ArgsDict args) const;

    template<std::size_t... Indices>
    ReturnT call(IndexSequence<Indices...>, std::vector<std::any> const &argsValues) {
        return (instance->*function)(std::any_cast<ArgsT>(argsValues[Indices])...);
    }
public:
    using FunctionT = ReturnT (ClassT::*)(ArgsT...);

    Wrapper(ClassT *inst, const FunctionT func, std::vector<std::string> args)
        : instance(inst), function(func), arguments(std::move(args)) {
    }

    ReturnT operator()(ArgsDict args);
};

template<typename ClassT, typename ReturnT, typename... ArgsT>
std::vector<std::any> Wrapper<ClassT, ReturnT, ArgsT...>::validateArgs(ArgsDict args) const {
    std::vector<std::any> argsValues;

    if (args.size() != arguments.size()) {
        throw std::runtime_error("Wrong number of arguments");
    }

    for (std::string const &arg: arguments) {
        if (!args.contains(arg)) {
            throw std::runtime_error("Argument not found");
        }
        argsValues.emplace_back(args.at(arg));
    }

    return argsValues;
}

template<typename ClassT, typename ReturnT, typename... ArgsT>
ReturnT Wrapper<ClassT, ReturnT, ArgsT...>::operator()(ArgsDict args) {
    auto argsValues = validateArgs(args);
    try {
        if constexpr (std::is_void_v<ReturnT>) {
            call(MakeIndexSequence<sizeof...(ArgsT)>{}, argsValues);
        } else {
            return call(MakeIndexSequence<sizeof...(ArgsT)>{}, argsValues);
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("Wrong type of arguments");
    }
}
