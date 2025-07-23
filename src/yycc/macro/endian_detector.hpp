#pragma once

// Check endian
#if (defined(YYCC_ENDIAN_LITTLE) + defined(YYCC_ENDIAN_BIG)) != 1
#error "Current system endian (byte order) is not supported!"
#endif

