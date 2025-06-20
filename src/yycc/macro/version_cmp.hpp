#pragma once

/// @brief Return true if left version number is equal to right version number, otherwise false.
#define YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3) ((av1) == (bv1) && (av2) == (bv2) && (av3) == (bv3))
/// @brief Return true if left version number is not equal to right version number, otherwise false.
#define YYCC_VERCMP_NE(av1, av2, av3, bv1, bv2, bv3) (!YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is greater than right version number, otherwise false.
#define YYCC_VERCMP_G(av1, av2, av3, bv1, bv2, bv3) ( \
    ((av1) > (bv1)) || \
    ((av1) == (bv1) && (av2) > (bv2)) || \
    ((av1) == (bv1) && (av2) == (bv2) && (av3) > (bv3)) \
)
/// @brief Return true if left version number is greater than or equal to right version number, otherwise false.
#define YYCC_VERCMP_GE(av1, av2, av3, bv1, bv2, bv3) (YYCC_VERCMP_G(av1, av2, av3, bv1, bv2, bv3) || YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is not lower than right version number, otherwise false.
#define YYCC_VERCMP_NL(av1, av2, av3, bv1, bv2, bv3) YYCC_VERCMP_GE(av1, av2, av3, bv1, bv2, bv3)
/// @brief Return true if left version number is lower than right version number, otherwise false.
#define YYCC_VERCMP_L(av1, av2, av3, bv1, bv2, bv3) ( \
    ((av1) < (bv1)) || \
    ((av1) == (bv1) && (av2) < (bv2)) || \
    ((av1) == (bv1) && (av2) == (bv2) && (av3) < (bv3)) \
)
/// @brief Return true if left version number is lower than or equal to right version number, otherwise false.
#define YYCC_VERCMP_LE(av1, av2, av3, bv1, bv2, bv3) (YYCC_VERCMP_L(av1, av2, av3, bv1, bv2, bv3) || YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is not greater than right version number, otherwise false.
#define YYCC_VERCMP_NG(av1, av2, av3, bv1, bv2, bv3) YYCC_VERCMP_LE(av1, av2, av3, bv1, bv2, bv3)
