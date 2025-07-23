#pragma once

#if (defined(YYCC_CC_MSVC) + defined(YYCC_CC_GCC) + defined(YYCC_CC_CLANG)) != 1
#error "Current compiler is not supported!"
#endif
