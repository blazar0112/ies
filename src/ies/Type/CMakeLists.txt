get_property(HEADERS GLOBAL PROPERTY PROP_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_HEADERS
    ${HEADERS}
)

get_property(SOURCES GLOBAL PROPERTY PROP_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_SOURCES
    ${SOURCES}
)

get_property(PUBLIC_HEADERS GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_PUBLIC_HEADERS
    ${PUBLIC_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Cpp11VoidT.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/HasStdToString.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsAssociativeContainer.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsAssociativeContainerV.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsDuration.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsDurationV.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsJson.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsJsonV.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IsMap.hxx
)

get_property(TEST_SOURCES GLOBAL PROPERTY PROP_TEST_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/IsMapTest.cpp
)
