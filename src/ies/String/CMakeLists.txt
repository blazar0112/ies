get_property(HEADERS GLOBAL PROPERTY PROP_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_HEADERS
    ${HEADERS}
)

get_property(SOURCES GLOBAL PROPERTY PROP_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/Levenshtein.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RecursiveReplace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitStringView.cpp
)

get_property(PUBLIC_HEADERS GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_PUBLIC_HEADERS
    ${PUBLIC_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Levenshtein.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RecursiveReplace.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitString.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitStringView.hpp
)

get_property(TEST_SOURCES GLOBAL PROPERTY PROP_TEST_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/LevenshteinTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RecursiveReplaceTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitStringTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SplitStringViewTest.cpp
)
