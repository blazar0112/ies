#pragma once

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define IES_VARIADIC_SIZE_LIMIT 256

//! @brief Get size of of __VA_ARGS__ in macro, up to IES_VARIADIC_SIZE_LIMIT.
//! @note These macros are referred from <boost/preprocessor/variadic/size.hpp>
//! Don't want to depend on boost in this library, to reduce overhead for user.
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define IES_VARIADIC_SIZE(...) IES_VARIADIC_SIZE_I( \
    __VA_ARGS__, IES_VARIADIC_SIZE_LIMIT, \
                        255, 254, 253, 252, 251, 250, \
    249, 248, 247, 246, 245, 244, 243, 242, 241, 240, \
    239, 238, 237, 236, 235, 234, 233, 232, 231, 230, \
    229, 228, 227, 226, 225, 224, 223, 222, 221, 220, \
    219, 218, 217, 216, 215, 214, 213, 212, 211, 210, \
    209, 208, 207, 206, 205, 204, 203, 202, 201, 200, \
    \
    199, 198, 197, 196, 195, 194, 193, 192, 191, 190, \
    189, 188, 187, 186, 185, 184, 183, 182, 181, 180, \
    179, 178, 177, 176, 175, 174, 173, 172, 171, 170, \
    169, 168, 167, 166, 165, 164, 163, 162, 161, 160, \
    159, 158, 157, 156, 155, 154, 153, 152, 151, 150, \
    149, 148, 147, 146, 145, 144, 143, 142, 141, 140, \
    139, 138, 137, 136, 135, 134, 133, 132, 131, 130, \
    129, 128, 127, 126, 125, 124, 123, 122, 121, 120, \
    119, 118, 117, 116, 115, 114, 113, 112, 111, 110, \
    109, 108, 107, 106, 105, 104, 103, 102, 101, 100, \
    \
    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
    89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
    69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9, 8, 7, 6, 5, 4, 3, 2, 1,)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define IES_VARIADIC_SIZE_I( \
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, \
    e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, \
    e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, \
    e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, \
    e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, \
    e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, \
    e60, e61, e62, e63, e64, e65, e66, e67, e68, e69, \
    e70, e71, e72, e73, e74, e75, e76, e77, e78, e79, \
    e80, e81, e82, e83, e84, e85, e86, e87, e88, e89, \
    e90, e91, e92, e93, e94, e95, e96, e97, e98, e99, \
    \
    e100, e101, e102, e103, e104, e105, e106, e107, e108, e109, \
    e110, e111, e112, e113, e114, e115, e116, e117, e118, e119, \
    e120, e121, e122, e123, e124, e125, e126, e127, e128, e129, \
    e130, e131, e132, e133, e134, e135, e136, e137, e138, e139, \
    e140, e141, e142, e143, e144, e145, e146, e147, e148, e149, \
    e150, e151, e152, e153, e154, e155, e156, e157, e158, e159, \
    e160, e161, e162, e163, e164, e165, e166, e167, e168, e169, \
    e170, e171, e172, e173, e174, e175, e176, e177, e178, e179, \
    e180, e181, e182, e183, e184, e185, e186, e187, e188, e189, \
    e190, e191, e192, e193, e194, e195, e196, e197, e198, e199, \
    \
    e200, e201, e202, e203, e204, e205, e206, e207, e208, e209, \
    e210, e211, e212, e213, e214, e215, e216, e217, e218, e219, \
    e220, e221, e222, e223, e224, e225, e226, e227, e228, e229, \
    e230, e231, e232, e233, e234, e235, e236, e237, e238, e239, \
    e240, e241, e242, e243, e244, e245, e246, e247, e248, e249, \
    e250, e251, e252, e253, e254, e255, \
    size, ...) size
