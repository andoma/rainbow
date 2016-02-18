// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#include "DotNet/CoreCLR.h"

#include "Common/Logging.h"
#include "Common/String.h"
#include "DotNet/Error.h"
#include "FileSystem/FileSystem.h"
#include "Platform/Macros.h"

#if !USE_STD_FILESYSTEM
#   define BOOST_FILESYSTEM_NO_DEPRECATED
#   ifdef __clang__
#       pragma clang diagnostic push
#       pragma clang diagnostic ignored "-Wold-style-cast"
#   endif
#   include <boost/filesystem.hpp>
#   ifdef __clang__
#       pragma clang diagnostic pop
#   endif
namespace std
{
    namespace filesystem = boost::filesystem;
}
#else
#   include <filesystem>

namespace std
{
    namespace filesystem = std::experimental::filesystem;
}
#endif

#ifndef NETCORE_VERSION
#   define NETCORE_VERSION "1.1.1"
#endif

#if defined(RAINBOW_OS_UNIX)
#   define ASSEMBLY_SEPARATOR ':'
#   define CORECLR_PATH "/usr/local/share/dotnet/shared/Microsoft.NETCore.App/"
#   define LIBRARY_PREFIX "/lib"
#   if defined(RAINBOW_OS_MACOS)
#       define LIBRARY_SUFFIX ".dylib"
#   else
#       define LIBRARY_SUFFIX ".so"
#   endif  // RAINBOW_OS_MACOS
#elif defined(RAINBOW_OS_WINDOWS)
#   define ASSEMBLY_SEPARATOR ';'
#   define CORECLR_PATH R"(C:\Program Files\dotnet\shared\Microsoft.NETCore.App\)"
#   define LIBRARY_PREFIX "\\"
#   define LIBRARY_SUFFIX ".dll"
#else
#   error "Unsupported platform"
#endif

namespace
{
    constexpr char kAssemblySeparator = ASSEMBLY_SEPARATOR;
    constexpr char kCoreCLRLibrary[] =
        CORECLR_PATH NETCORE_VERSION LIBRARY_PREFIX "coreclr" LIBRARY_SUFFIX;
    constexpr char kCoreCLRPath[] = CORECLR_PATH NETCORE_VERSION;
}

#undef ASSEMBLY_SEPARATOR
#undef CORECLR_PATH
#undef LIBRARY_PREFIX
#undef LIBRARY_SUFFIX

using rainbow::czstring;

auto coreclr::core_library_path() -> std::string
{
    return kCoreCLRLibrary;
}

auto coreclr::native_dll_search_directories() -> std::string
{
    return rainbow::filesystem::assets_path();
}

auto coreclr::trusted_platform_assemblies(const std::string& search_path)
    -> std::string
{
    std::string trusted_assemblies;
    std::string il_assemblies;
    auto find = [&trusted_assemblies, &il_assemblies](const std::string& path) {
        for (auto&& entry : std::filesystem::directory_iterator{path})
        {
            if (!std::filesystem::is_regular_file(entry.status()))
                continue;

            const auto& p = entry.path();
            if (p.extension() == ".dll")
            {
                if (rainbow::ends_with(p.stem().string(), ".ni"))
                    trusted_assemblies += p.string() + kAssemblySeparator;
                else
                    il_assemblies += p.string() + kAssemblySeparator;
            }
        }
    };

    find(search_path);
    find(kCoreCLRPath);

    trusted_assemblies += il_assemblies;

    if (!trusted_assemblies.empty())
        trusted_assemblies.pop_back();

    return trusted_assemblies;
}

#define kFailedCreateDelegate                                                  \
    "CoreCLR: Failed to load '%s.%s' from assembly '%s'"

#define LOG_ERROR_WITH(logx)                                                   \
    do                                                                         \
    {                                                                          \
        if (error != nullptr)                                                  \
        {                                                                      \
            logx(kFailedCreateDelegate ": %s",                                 \
                 type,                                                         \
                 method_name,                                                  \
                 assembly,                                                     \
                 error);                                                       \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            logx(kFailedCreateDelegate " (HRESULT: 0x%x)",                     \
                 type,                                                         \
                 method_name,                                                  \
                 assembly,                                                     \
                 result);                                                      \
        }                                                                      \
    } while (false)

bool coreclr::CreateDelegateHelper::operator()(czstring assembly,
                                               czstring type,
                                               czstring method_name,
                                               void** delegate,
                                               Requirement requirement) const
{
    const int result = create_delegate_(  //
        host_handle_,
        domain_id_,
        assembly,
        type,
        method_name,
        delegate);
    const bool succeeded = !failed(result);
    if (!succeeded)
    {
        auto error = rainbow::error::format_message(result);
        if (requirement == Requirement::Optional)
            LOG_ERROR_WITH(LOGW);
        else
            LOG_ERROR_WITH(LOGF);
    }
    return succeeded;
}
