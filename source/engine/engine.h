#pragma once

#include <functional>
#include <unordered_map>
#include <stdexcept>

#include "../wrapper/wrapper.h"

class Engine {
    std::unordered_map<std::string, std::function<std::any(ArgsDict)>> commands;
public:
    template<typename ClassT, typename ReturnT, typename... ArgsT>
    void register_command(Wrapper<ClassT, ReturnT, ArgsT...>* wrapper, const std::string &command) {
        if (commands.contains(command)) {
            throw std::runtime_error("Command " + command + "already exists");
        }

        commands.emplace(command, [wrapper](std::unordered_map<std::string, std::any> const &args) {
            if constexpr (std::is_void_v<ReturnT>) {
                (*wrapper)(args);
                return std::any(nullptr);
            } else {
                return std::any((*wrapper)(args));
            }
        });
    }

    template<typename ReturnT = void>
    ReturnT execute(std::string const& command, ArgsDict args) {
        if (!commands.contains(command)) {
            throw std::runtime_error("Miss command " + command);
        }

        if constexpr (std::is_void_v<ReturnT>) {
            commands[command](args);
        } else {
            return std::any_cast<ReturnT>(commands[command](args));
        }
    }
};
