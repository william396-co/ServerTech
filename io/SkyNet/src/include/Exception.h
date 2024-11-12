#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

enum class ExceptionType {
    INVALID = 0,
    INVALID_SOCKET = 1,
};

class Exception : public std::runtime_error {
   public:
    explicit Exception(std::string const& message) : std::runtime_error(message), type_{ExceptionType::INVALID} {
        std::string exception_msg = "Message :: " + message + "\n";
        std::cerr << exception_msg;
    }

    Exception(ExceptionType type, std::string const& message) : std::runtime_error(message), type_{type} {
        std::string exception_msg = "Exception Type :: " + ExceptionType2Str(type_) + "\nMessage :: " + message + "\n";
        std::cerr << exception_msg;
    }

   private:
    static std::string ExceptionType2Str(ExceptionType type) {
        switch (type) {
            case ExceptionType::INVALID:
                return "Invalid";
            case ExceptionType::INVALID_SOCKET:
                return "Invalid Socket";
            default:
                return "Unknown";
        }
    }

   private:
    ExceptionType type_{};
};
