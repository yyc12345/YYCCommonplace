#pragma once
#include "../macro/ptr_size_detector.hpp"
#include <cstdint>

/**
 * @def PRIXPTR_LPAD
 * @brief The left-padding zero format string of HEX-printed pointer type.
 * @details
 * When printing a pointer with HEX style, we usually hope it can be left-padded with some zero for easy reading.
 * In different architecture, the size of this padding is differnet too so we create this macro.
 *
 * In 32-bit environment, it will be "08" meaning left pad zero until 8 number position.
 * In 64-bit environment, it will be "016" meaning left pad zero until 16 number position.
*/

#if defined(YYCC_PTRSIZE_32)
#define PRIXPTR_LPAD "08"
#else
#define PRIXPTR_LPAD "016"
#endif
