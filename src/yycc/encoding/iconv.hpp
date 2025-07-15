#pragma once
#include "../macro/os_detector.hpp"

#if YYCC_FEAT_ICONV || (YYCC_OS != YYCC_OS_WINDOWS)

#include "../macro/class_copy_move.hpp"
#include "../patch/expected.hpp"
#include "../string.hpp"
#include <memory>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::iconv {

    // YYC MARK:
    // I don't want to include "iconv.h" in there.
    // One of reasons is that I want to hide all implementation of Iconv.
    // Another reason is that "iconv.h" defines some annoying macros which intervene some names in this files.
    // So I introduce PIMPL design mode. Use a pointer to hide all details in class PrivToken.

    using CodeName = NS_YYCC_STRING::u8string_view;

    /// @private
    class PrivToken;

    class Token {
    public:
        Token(const CodeName& from_code, const CodeName& to_code);
        ~Token();
        YYCC_DELETE_COPY(Token)
        YYCC_DEFAULT_MOVE(Token)

        bool is_valid() const;
        PrivToken* get_inner() const;

    private:
        std::unique_ptr<PrivToken> inner;
    };

    /// @private
    enum class ConvError {
        InvalidCd, ///< Given token is invalid.
        NullPointer, ///< Some of essential pointer in argument is nullptr.
        InvalidMbSeq, ///< An invalid multibyte sequence has been encountered in the input.
        IncompleteMbSeq, ///< An incomplete multibyte sequence has been encountered in the input.
    };

    /// @private
    template<typename T>
    using ConvResult = NS_YYCC_PATCH_EXPECTED::Expected<T, ConvError>;

} // namespace yycc::encoding::iconv

#undef NS_YYCC_PATCH_EXPECTED
#undef NS_YYCC_STRING

#endif
