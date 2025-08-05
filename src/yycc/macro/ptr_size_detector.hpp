#pragma once

// Check pointer size macro
#if (defined(YYCC_PTRSIZE_32) + defined(YYCC_PTRSIZE_64)) != 1
#error "Current environment used pointer size is not supported!"
#endif
