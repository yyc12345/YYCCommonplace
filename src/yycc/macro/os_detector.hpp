#pragma once

// Check OS macro
#if (defined(YYCC_OS_WINDOWS) + defined(YYCC_OS_LINUX)) != 1
#error "Current operating system is not supported!"
#endif
