# Changelog of IES

- v5.1.1 [2024-07-12]:
    - Fix `IntegerPow` large N when N>2^32 issue.
    - Fix `IntegerPow` 10^19 issue.

- v5.1.0 [2024-07-12]:
    - Rename `FastIntegerPow*` filename to `IntegerPow*`.
    - Rename function `FastIntegerPow()` to `LookupIntegerPow()`.
    - Fix `IntegerPow` implementation and add more tests.

- v5.0.0 [2024-07-11]:
    - Add `FastIntegerPow` and start using C++20.

- v4.4.0 [2024-07-03]:
    - Add `[[nodiscard]]` to functions declared in `SmartEnum` in `Common`.
    - Add `[[nodiscard]]` to `Find` in `StdUtil`.

- v4.3.0 [2024-06-10]:
    - Add `RequireCpp[20|23].hpp` in `StdUtil`.

- v4.2.1 [2024-01-23]:
    - Fix `SmartEnum::Has` bug.

- v4.2.0 [2023-12-28]:
    - Upgrade `SmartEnum` to use C++17 and improve writing and document.
    - As a result, `test-cpp11only` no longer works.
    - Previous implementation is copied to `SmartEnumCpp11` as reference.
    - Add BuildTime source to estimate header include time impact.
        - On my PC, single thread, gcc(mingw) `-ftime-report` (no clang environment).
        - IntregralRange: 0.46s
        - SmartEnum: 0.27s
        - Find: 0.47s

- v4.1.0 [2023-03-29]:
    - Fix IWYU and clang tidy warning in headers.
    
- v4.0.0 [2022-11-13]:
    - Update license to MIT-0 license.

- v3.3.0 [2022-09-28]:
    - Find() now support std::string_view.

- v3.2.2 [2022-09-06]:
    - Update CMake GTest usage.

- v3.2.1 [2022-09-05]:
    - Fix IsDuration headers not add to public headers.
    - Remove -DFMT_HEADER_ONLY from flags since fmt now use config mode fmt::fmt-header-only.

- v3.2.0 [2022-09-02]:
    - Update CMake, now all dependency use installed config mode.
        - List fmt and nlohmann_json requirement explicitly.
    - Update README for build instructions.
    
- v3.1.1 [2022-09-01]:
    - Fix CMake test-all and MinGW incorrect command after previous reorganization.

- v3.1.0 [2022-08-30]:
    - Add ToIndex for SmartEnum.
    - Add ScopeTimePrinter.
    - Update CMake using gtest and benchmark to optional to ease git clone and simply build flow.
    - Fix MSVC flow.

- v3.0.0 [2022-02-18]:
    - Rebrand `icl_s2` to `ies`.

- v2.2.0 [2021-12-03]:
    - Update Cmake for project-group v2 flow.

- v2.1.1 [2021-07-30]:
    - Fix Find() incorrect behavior if container is nlohmann::json when json is array type.

- v2.1.0 [2021-05-25]:
    - Follow CMake export package guide, remove unnecessary ICL_S2_INCLUDE_DIRS for client project.
    - Add set, list, forward_list, deque for FormatString.
    - Add google benchmark and benchmark.cpp, which implement all performance.cpp in google benchmark.

- v2.0.0 [2021-04-09]:
    - Rebrand `itricommon` to `icl_s2`.
    - Drastically refactored project structure and interface.
    - Project structure:
        - Change project name and namespace to `icl_s2`.
        - Top level cmake is now under `icl_s2/`, instead of `icl_s2/src/`.
        - Add namespace target `icl_s2::` in cmake.
        - Rename `version-history.txt` to `changelog.md`.
        - Add support for Windows MINGW and MSVC.
        - Update README.md.

    - See `doc/icl_s2_migration.md` for detail changes and migration notes.

- v1.5.0 [2020-10-30]:
    - Fix StringTree interface GetChildNodes, GetParentNode, GetNodesByLevel return type should be a reference to TreeNode member.
        - Update StringTree implementation using StdUtil::Find (requires C++17) and change policy below.
    - Implementation policy renewed:
        - For HeaderOnly (.hpp for definition, .hxx for template/inline implementation):
            Header can choose to support C++11 or C++17.
        - For Implementation (.cpp)
            Source code will assume build with C++17, does not modify to support C++11 only.
        - i.e. To build itricommon, C++17 is required.
            - When use pre-built library (include+link) or header only (include), user can use C++11-only part and build user application with C++11 only.
        - Now use test-cpp11only to ensure some headers are properly written for C++11 user.
            - test-cpp11only include following tests that are assumed using C++11 compatible headers:
                ```
                IntegralRangeTest.cpp
                SmartEnumTest.cpp
                StdUtilityTest.cpp
                ```
    - Move format/print related functions in TimeUtil to TimeUtilFormat, to let user can choose include TimeUtil when user do not use fmt.
    - Add Extremum to represent Min/Max concept, and add IntegralRange::get(Extremum) interface.

- v1.4.0 [2020-09-17]:
    - SmartEnum::Size type change to std::size_t (was int).
    - Add nlohmann::json support for StdUtil::Find.

- v1.3.0 [2020-08-03]:
    - Change target_compile_options to private so that library does not propagate
      compile options when client use find_package.

- v1.2.2 [2020-07-21]:
    - StdUtil add MapApplyWithInit and convenient MapAddCount functions.
    - TimeUtil address windows clock problem, add clock alias.

- v1.2.1 [2020-06-18]:
    - Undo SplitString signature to avoid ABI break.
    - SplitStringPreserve is now a separate function.

- v1.2.0 [2020-06-18]:
    - Add Windows porting.
    - Find can search null character correctly now.
    - Remove LUT and Property directory.

- v1.1.3 [2020-05-29]:
    - SplitString can optionally preserve empty token.

- v1.1.2 [2020-04-16]:
    - Improve SmartEnum::ToRange.
    - Add TimeUtil.

- v1.1.1 [2020-03-18]:
    - Fix Cmake config typo.

- v1.1.0 [2020-03-16]:
    - Add Cmake config mode for find_package.

- v1.0.0 [2020-03-09]:
    - Switch to use git.
