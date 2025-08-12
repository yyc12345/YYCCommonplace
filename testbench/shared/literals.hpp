#pragma once
#include <yycc.hpp>
#include <yycc/macro/class_copy_move.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace yyccshared::literals {

    class UtfLiterals {
    public:
        UtfLiterals();
        ~UtfLiterals();
        YYCC_DELETE_COPY_MOVE(UtfLiterals)

        size_t get_size() const;
        const std::vector<std::u8string>& get_u8str_vec() const;
        const std::vector<std::u16string>& get_u16str_vec() const;
        const std::vector<std::u32string>& get_u32str_vec() const;
        const std::vector<std::wstring>& get_wstr_vec() const;

    private:
        const std::vector<std::u8string>& u8str_vec;
        const std::vector<std::u16string>& u16str_vec;
        const std::vector<std::u32string>& u32str_vec;
        const std::vector<std::wstring>& wstr_vec;
    };

    class OtherLiteral {
    public:
        OtherLiteral(std::string&& other_str, uint32_t windows_ident, std::string&& iconv_ident, std::u8string&& pycodec_ident);
        ~OtherLiteral();
        YYCC_DEFAULT_COPY_MOVE(OtherLiteral)

        const std::string& get_other_str() const;
        uint32_t get_windows_ident() const;
        const std::string& get_iconv_ident() const;
        const std::u8string& get_pycodec_ident() const;

    private:
        std::string other_str;
        uint32_t windows_ident;
        std::string iconv_ident;
        std::u8string pycodec_ident;
    };

    class OtherLiterals {
    public:
        OtherLiterals();
        ~OtherLiterals();
        YYCC_DELETE_COPY_MOVE(OtherLiterals)

        size_t get_size() const;
        const std::vector<OtherLiteral>& get_other_str_vec() const;
        const std::vector<std::u8string>& get_u8str_vec() const;
        const std::vector<std::wstring>& get_wstr_vec() const;

    private:
        const std::vector<OtherLiteral>& other_str_vec;
        const std::vector<std::u8string>& u8str_vec;
        const std::vector<std::wstring>& wstr_vec;
    };

} // namespace yyccshared::literals
