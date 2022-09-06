# IES C++ Common Library

- Copyright (C) 2022 ITRI EOSL S-Division
- Common Library for general C++ development facility.
- [IES License](license/IES-LICENSE.txt)

# Table of Contents

- [IES C++ Common Library](#ies-c-common-library)
- [Table of Contents](#table-of-contents)
    - [Dependency](#dependency)
        - [Install Dependency](#install-dependency)
    - [MINGW](#mingw)
    - [Build](#build)

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