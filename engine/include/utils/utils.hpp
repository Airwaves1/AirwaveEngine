#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#if defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#endif

namespace Airwave
{
inline std::string demangle(const char *name)
{
#if defined(__GNUC__) || defined(__GNUG__)
    int status;
    char *realname = abi::__cxa_demangle(name, 0, 0, &status);
    std::string result(realname);
    free(realname);
    return result;
#else
    return name;

#endif
}

} // namespace Airwave
