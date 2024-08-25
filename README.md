# IES C++ Common Library

- Copyright (C) 2022 ITRI EOSL S-Division
- Common Library for general C++ development facility.
- [IES License](license/IES-LICENSE.txt)

## Use Guide

- Only summarize most useful items, not listing every item in library here.
- Target for light-weighted and not using any `boost`.
- Note on extension:
    - `.hpp`: C++ header with mainly declarations.
    - `.hxx`: C++ template or heavily inlined header.

### SmartEnum
- `#include "ies/Common/SmartEnum.hxx"`.
- Help building abstraction, providing string reflection of enumerator and enabling enum iteration.
- Example:

    ```cpp
    #include "ies/Common/SmartEnum.hxx"

    // ToString() reflects exactly as enumerator's name.
    // Declare enumerator to fit purpose that requires ToString().
    // For example, if log requires lower-cased build type names.
    IES_SMART_ENUM(BuildType, release, debug);
    // No longer can have typo. Prefer auto completed code.
    // Print "Log - BuildType[debug]".
    fmt::println(
        "Log - {}[{}]",
        BuildTypeSmartEnum::GetName(),
        ToString(BuildType::debug));

    // Declare enum class MyEnv and auxiliary class MyEnvSmartEnum.
    // Declare in namespace this macro is used.
    IES_SMART_ENUM(MyEnv,
        ENV_A,
        ENV_B,
        ENV_C
    );

    // MyEnvSmartEnum::Size() is constexpr and can use for size of std::array.
    // Model enumerator value 1-1 mapping to array index.
    std::array<std::optional<std::string>, MyEnvSmartEnum::Size()> EnvValues;

    void InitEnv()
    {
        // Iterating enum easily using SmartEnum::ToRange().
        for (auto env : MyEnvSmartEnum::ToRange())
        {
            // ToString() returns const std::string&.
            // No longer can have typo when using getenv().
            if (auto* value = std::getenv(ToString(env).c_str()))
            {
                // Model enumerator value 1-1 mapping to array index.
                // ToIndex() returns std::size_t.
                EnvValues[ToIndex(env)] = value;
            }
        }
    }
    ```

### Find
- `#include "ies/StdUtil/Find.hxx"`.
- Usage: `auto resultOption = Find(container, element);`.
    - Returns `std::optional` of original `.find()` or `std::find()` return.
- Reduce boilerplate code comparing `find()` to `end()` or `std::string::npos`.
    - It's also non-intuitive because double negation `!=end()`.
- Same interface for all STL container, `std::string` and nlohmann json.
    - However, beware that for non-associative container it uses `O(n)` `std::find()`.
- Example:

    ```cpp
    #include "ies/StdUtil/Find.hxx"

    std::set<int> set{1, 2};
    std::map<int, std::string> map{{1, "a"}, {2, "b"}};
    std::vector<int> vector{1, 2};
    std::string str{"hello world!"};
    nlohmann::json json{{"one", 1}, {"two", 2}};
    // Without ies::Find
    if (auto it = set.find(1); it!=set.end())
    {
        fmt::println("has key {}.", *it);
    }
    if (auto it = map.find(1); it!=map.end())
    {
        fmt::println("has key {} and value is {}.", it->first, it->second);
    }
    if (auto it = std::find(vector.begin(), vector.end(), 1); it!=vector.end())
    {
        fmt::println("has element {}.", *it);
    }
    if (auto pos = str.find("world"); pos!=std::string::npos)
    {
        fmt::println("has substring 'world' at pos {}", pos);
    }
    if (auto it = json.find("one"); it!=json.end())
    {
        fmt::println("has key {} and value is {}.", it.key(), it.value());
    }
    // Using ies::Find
    // Not repeating container name and no longer has double negation.
    if (auto found = ies::Find(set, 1))
    {
        fmt::println("has key {}.", *found.value());
    }
    if (auto found = ies::Find(map, 1))
    {
        fmt::println("has key {} and value is {}.", found.value()->first, found.value()->second);
    }
    if (auto found = ies::Find(vector, 1))
    {
        fmt::println("has element {}.", *found.value());
    }
    if (auto found = ies::Find(str, "world"))
    {
        fmt::println("has substring 'world' at pos {}", found.value());
    }
    if (auto found = ies::Find(json, "one"))
    {
        fmt::println("has key {} and value is {}.", found.value().key(), found.value().value());
    }
    ```

### SplitString[View]
- `#include "ies/String/SplitString[View].hpp"`.
- Usage: `auto tokens = ies::SplitString(separator, input)`.
- SplitString: split `std::string` into `std::vector<std::string>`.
- SplitStringView: split `std::string_view` into `std::vector<std::string_view>`.
- Separator: split to any one of characters in `separator` string.
- Example:

    ```cpp
    #include "ies/String/SplitString[View].hpp"

    std::string input{"(x,y), ,(a b)"};
    std::string separator{"() ,"};

    // tokens = {"x", "y", "a", "b"}
    auto tokens = ies::SplitString(separator, input);
    // equivalent boost
    #include <boost/algorithm/string.hpp>
    std::vector<std::string> tokens;
    auto trimmedInput = input;
    boost::trim_if(trimmedInput, boost::is_any_of(separator));
    boost::split(tokens, trimmedInput, boost::is_any_of(separator), boost::token_compress_on);

    // Preserving empty values from multiple adjacent separators.
    // Useful for CSV format, for example.
    std::string csvRow{",,b,,,e,,"};
    constexpr std::size_t CsvColumnCount = 8;
    // tokens = {"", "", "b", "", "", "e", "", ""}
    auto tokens = ies::SplitStringPreserve(",", csvRow, CsvColumnCount);
    // equivalent boost
    #include <boost/algorithm/string.hpp>
    std::vector<std::string> tokens;
    boost::split(tokens, csvRow, boost::is_any_of(","));
    ```

### IntegralRange (IntRange, IndexRange)
- `#include "ies/Common/IntegralRange.hxx"` for arbitrary integral type `ies::IntegralRange<T>`.
- `#include "ies/Common/IntegralRangeUsing.hpp"` for common `IntRange`, `IndexRange` in global namespace and ease of using.
    - `IntRange`: alias for `IntegralRange<int>`.
    - `IndexRange`: alias for `IntegralRange<std::size_t>`.
- IntegralRange models non-empty range `[begin, end)`.
    - Can opt to be empty by specifying `ies::EmptyPolicy::Allow`.
- Python-like range usage to iterate integral value in range-based for loop.
    - Cannot specify `step`, `step` is always 1.
- Runtime constructed range, do not use in performance critical loop.
- IntegralRange also provides utilities to compare with value point or overlap with other range.
    - It was designed to represent 2D/3D rectilinear shape.
- Example:

    ```cpp
    #include "ies/Common/IntegralRangeUsing.hpp"

    std::array<int, 8> values;
    // Do not use IntegralRange if loop count is constant value!
    // Compiler can do vectorization, just use raw for loop.
    for (std::size_t i = 0; i<8; ++i)
    {
        DoSomething(value[i]);
    }

    std::vector<int> v1;
    std::vector<int> v2; // assume same size as v2
    v1.resize(100);
    v2.resize(100);
    // Iterating v1 and v2 at same time using index.
    for (std::size_t i = 0; i<v1.size(); ++i)
    {
        DoSomething(v1[i]-v2[v1.size()-i]);
    }

    // If raw loop count is dynamic, performance is close besides the constructing cost.
    for (auto i : IndexRange{0, v1.size()})
    {
        DoSomething(v1[i]-v2[v1.size()-i]);
    }
    // Default assume non-empty range and throws if empty.
    // Use ies::EmptyPolicy::Allow if range is possibly empty.
    for (auto i : IndexRange{0, v1.size(), ies::EmptyPolicy::Allow})
    {
        DoSomething(v1[i]-v2[v1.size()-i]);
    }

    // Alternative: boost::irange.
    // However, compilation cost including irange is higher.
    #include <boost/range/irange.hpp>
    for (auto i : boost::irange<std::size_t>(0, v1.size.size()))
    {
        DoSomething(v1[i]-v2[v1.size()-i]);
    }

    // Alternative: C++20, IntegralRange was invented before C++17.
    #include <ranges>
    for (auto i : std::ranges::iota_view{0u, v1.size.size()})
    {
        DoSomething(v1[i]-v2[v1.size()-i]);
    }

    // Example of provided utilities:
    struct Rect
    {
        IntRange XRange;
        IntRange YRange;
    };

    Rect r1{{0, 3}, {0, 3}};
    Rect r2{{1, 4}, {1, 4}};
    if (r1.XRange.Overlaps(r2.XRange)&&r1.YRange.Overlaps(r2.YRange))
    {
        fmt::println("r1 and r2 overlaps.");
    }
    ```

## Dependency

- Linux GCC/MINGW: set install path to environment variable (EnvVar) listed.
- MSVC: use vcpkg to manage library.

    | Library | Requirement | Install path | CMake Usage |
    | - | - | - | - |
    | [fmt](https://github.com/fmtlib/fmt) | Required | `$ENV{fmt_DIR}` | `find_package(fmt CONFIG REQUIRED)` |
    | [nlohmann_json](https://github.com/nlohmann/json) | Required | `$ENV{nlohmann_json_DIR}` | `find_package(nlohmann_json CONFIG REQUIRED)` |
    | [Google Test](https://github.com/google/googletest) | Optional | `$ENV{GTest_DIR}` | `find_package(GTest CONFIG)` |
    | [Google Benchmark](https://github.com/google/benchmark) | Optional | `$ENV{benchmark_DIR}` | `find_package(benchmark CONFIG)` |

### Install Dependency

- All dependencies are cmake project, usually just install like any cmake project.
    - Google Benchmark: set `option(BENCHMARK_ENABLE_GTEST_TESTS OFF)`

- Linux GCC/Clang:

    ```sh
    cd <source_home>
    git clone <project git link>
    cd <build_home>
    mkdir -p <project>/build-<toolchain>
    cd build-<toolchain>
    cmake <source_home>/<project> -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PATH=<install_home>/<project>/<version>/<toolchain>
    make -j8
    make install
    ```

- Windows QtCreator+MINGW:
    - In QtCreator `File` -> `Open File or Project...` -> select project's CMakeLists.txt. -> `Configure Project`.
    - `Projects` (mode, Ctrl+5):
        - `Build & Run` -> `<kit> Build` -> `Build Settings` -> `CMake` -> edit configure CMAKE_INSTALL_PATH to desired install path -> `Run CMake`.
        - `Build Steps` -> `Details` -> `Tool arguments` -> add `-j8`.
        - `Build Steps` -> `Details` -> `Targets` -> check `install`.
    - Click `Build` (Ctrl+B).

- Windows Vcpkg:
    - [Vcpkg](https://github.com/microsoft/vcpkg)
    - Follow instruction of vcpkg to configure Visual Studio can link to vcpkg.
    - Install by open PowerShell in vcpkg directory and run command:

        ```powershell
        # set EnvVar to let vcpkg default triplet use x64 instead of x86
        setx VCPKG_DEFAULT_TRIPLET x64-windows
        cd vcpkg
        ./vcpkg install fmt
        # nlohmann-json, not nlohmann_json
        ./vcpkg install nlohmann-json
        ./vcpkg install gtest
        ./vcpkg install benchmark
        ```

## MINGW

- Can build using Qt bundled MINGW or MSYS2 MINGW.
- Auxiliary EnvVar `$ENV{MINGW64_ROOT}` help copy running executable required .dll to build directory.
- Set to path of the MINGW used.

## Build

- Install [Dependency](#Dependency). Linux/MINGW flow need set EnvVar.
    - Vcpkg flow does not need set EnvVar.
- Linux GCC/Clang:

    ```sh
    git clone https://github.com/blazar0112/ies.git
    mkdir build
    cd build
    cmake ..
    make -j8
    # run example main that profile time of functions.
    ./src/performance
    # if build with Google Test
    ./src/test-all
    # if build with Google Benchmark
    ./src/benchmark
    ```

- Windows QtCreator+MINGW:
    - In QtCreator `File` -> `Open File or Project...` -> select project's CMakeLists.txt. -> `Configure Project`.
    - `Projects` (mode, Ctrl+5):
        - `Build & Run` -> `<kit> Build` -> `Build Settings` -> `CMake` -> edit configure CMAKE_INSTALL_PATH to desired install path -> `Run CMake`.
        - `Build Steps` -> `Details` -> `Tool arguments` -> add `-j8`.
        - `Build Steps` -> `Details` -> `Targets` -> check `install`.
    - Click `Build` (Ctrl+B).

- Windows VisualStudio+MSVC+Vcpkg:
    - Add `CMakeSettings.json` to project root (this is ignored in git as it's personal setting) to configure cmake setting for visual studio.
    - Example content:

        ```json
        {
            "configurations": [
                {
                    "name": "x64-Debug",
                    "generator": "Ninja",
                    "configurationType": "Debug",
                    "inheritEnvironments": [
                        "msvc_x64_x64"
                    ],
                    "buildRoot": "D:\\build\\ies\\msvc-${name}",
                    "installRoot": "D:\\install\\ies\\latest\\msvc-vs2019",
                    "cmakeCommandArgs": "",
                    "buildCommandArgs": "",
                    "ctestCommandArgs": "",
                    "variables": [
                        {
                            "name": "CMAKE_TOOLCHAIN_FILE",
                            "value": "F:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
                        }
                    ]
                },
                {
                    "name": "x64-Release",
                    "generator": "Ninja",
                    "configurationType": "RelWithDebInfo",
                    "buildRoot": "D:\\build\\ies\\msvc-${name}",
                    "installRoot": "D:\\install\\ies\\latest\\msvc-vs2019",
                    "cmakeCommandArgs": "",
                    "buildCommandArgs": "",
                    "ctestCommandArgs": "",
                    "inheritEnvironments": [
                        "msvc_x64_x64"
                    ],
                    "variables": [
                        {
                            "name": "CMAKE_TOOLCHAIN_FILE",
                            "value": "F:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake"
                        }
                    ]
                }
            ]
        }
        ```
    - `File` -> `Open` -> `Folder...` at project root, will recognize as a cmake project.
    - `Build` -> `Install IES`.
