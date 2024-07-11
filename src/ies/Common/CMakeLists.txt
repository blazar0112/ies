get_property(HEADERS GLOBAL PROPERTY PROP_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_HEADERS
    ${HEADERS}
)

get_property(SOURCES GLOBAL PROPERTY PROP_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/Byte.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRangeBuildTime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntRangeUtil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NamedObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SmartEnumBuildTime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringTree.cpp
)

get_property(PUBLIC_HEADERS GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
set_property(GLOBAL PROPERTY
    PROP_PUBLIC_HEADERS
    ${PUBLIC_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/AdjacentArrayRange.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/AdjacentVectorRange.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/Byte.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Endian.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/Extremum.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerPow.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRange.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRangeList.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRangeUsing.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntRangeUtil.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NamedObject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pimpl.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/RangeSide.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SmartEnum.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/StringTree.hpp
)

get_property(TEST_SOURCES GLOBAL PROPERTY PROP_TEST_SOURCES)
set_property(GLOBAL PROPERTY
    PROP_TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/AdjacentArrayRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AdjacentVectorRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ByteTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EndianTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegerPowTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntegralRangeListTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntRangeUtilTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PimplTestClass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PimplTestClassImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PimplTestClassTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SmartEnumTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SmartEnumCpp11Test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringTreeTest.cpp
)
