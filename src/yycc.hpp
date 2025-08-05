#pragma once

/**
 * @file
 * When you use YYCCommonplace, please make sure that you include this header first,
 * before including any other headers of YYCC.
 * This header contain essential check macros and version infos.
 * They are crucial before using YYCC.
 */

// Library version
#include "yycc/version.hpp"

// Detect essential macros
// Operating System macros
#include "yycc/macro/os_detector.hpp"
// Compiler macros
#include "yycc/macro/compiler_detector.hpp"
// Endian macros
#include "yycc/macro/endian_detector.hpp"
// Pointer size macros
#include "yycc/macro/ptr_size_detector.hpp"
// STL macros
#include "yycc/macro/stl_detector.hpp"

namespace yycc {}
