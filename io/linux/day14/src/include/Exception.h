#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

enum ExceptionType
{
    INVALID = 0,
    INVALID_SOCKET = 1,
};

class Exception : public std::runtime_error
{
public:
    explicit Exception( std::string const & msg )
        : std::runtime_erro( msg ), type_ { ExceptionType::INVALID }
    {
        std::string except_msg = "Message ::" + msg + "\n";
        std::cerr << except_msg;
    }

    Exception( ExceptionType type, std::string const & msg )
        : std::runtime_error( msg ), type_ { type }
    {
        std::string except_msg = "Exception Type ::" + ExceptionType2Str( type_ ) + "\nMessage ::"
            + msg + "\n";
        std::cerr << except_msg;
    }

private:
    static std::string ExceptionType2Str( ExceptionType type )
    {
        switch ( type ) {
            case ExceptionType::INVALID:
                return "Invalid";
            case ExceptionType::INVALID_SOCKET:
                return "Invalid Socket";
            default:
                return "Unkown";
        }
    }

private:
    ExceptionType type_;
};
