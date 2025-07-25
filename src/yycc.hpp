#pragma once

/**
 * @file
 * When you use YYCCommonplace, please make sure that you include this header first,
 * before including any other headers of YYCC.
 * This header contain essential check macros and version infos.
 * They are crucial before using YYCC.
 */

// Library Version and Comparison Macros
#include "yycc/version.hpp"
#include "yycc/macro/version_cmp.hpp"

// Detect essential macros
// Operating System macros
#include "yycc/macro/os_detector.hpp"
// Compiler macros
#include "yycc/macro/compiler_detector.hpp"
// Endian macros
#include "yycc/macro/endian_detector.hpp"

// Batch Class Move / Copy Function Macros
#include "yycc/macro/class_copy_move.hpp"

namespace yycc {}
