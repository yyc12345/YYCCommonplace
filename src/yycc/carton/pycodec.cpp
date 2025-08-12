#include "pycodec.hpp"

#include "../string/op.hpp"
#include <map>

using namespace std::literals::string_view_literals;
namespace op = ::yycc::string::op;

namespace yycc::carton::pycodec {

    /// @brief Error occurs when fetching token from encoding name.
    enum class FetchError {
        NoSuchName, ///< Given name can not be resolved.
    };

    /// @brief The Result type used when fetching token from encoding name.
    template<typename T>
    using FetchResult = std::expected<T, FetchError>;

#pragma region Encoding Name

    static const std::map<std::u8string_view, std::u8string_view> ALIAS_MAP{
        {u8"646"sv, u8"ascii"sv},
        {u8"us-ascii"sv, u8"ascii"sv},
        {u8"big5-tw"sv, u8"big5"sv},
        {u8"csbig5"sv, u8"big5"sv},
        {u8"big5-hkscs"sv, u8"big5hkscs"sv},
        {u8"hkscs"sv, u8"big5hkscs"sv},
        {u8"ibm037"sv, u8"cp037"sv},
        {u8"ibm039"sv, u8"cp037"sv},
        {u8"273"sv, u8"cp273"sv},
        {u8"ibm273"sv, u8"cp273"sv},
        {u8"csibm273"sv, u8"cp273"sv},
        {u8"ebcdic-cp-he"sv, u8"cp424"sv},
        {u8"ibm424"sv, u8"cp424"sv},
        {u8"437"sv, u8"cp437"sv},
        {u8"ibm437"sv, u8"cp437"sv},
        {u8"ebcdic-cp-be"sv, u8"cp500"sv},
        {u8"ebcdic-cp-ch"sv, u8"cp500"sv},
        {u8"ibm500"sv, u8"cp500"sv},
        {u8"ibm775"sv, u8"cp775"sv},
        {u8"850"sv, u8"cp850"sv},
        {u8"ibm850"sv, u8"cp850"sv},
        {u8"852"sv, u8"cp852"sv},
        {u8"ibm852"sv, u8"cp852"sv},
        {u8"855"sv, u8"cp855"sv},
        {u8"ibm855"sv, u8"cp855"sv},
        {u8"857"sv, u8"cp857"sv},
        {u8"ibm857"sv, u8"cp857"sv},
        {u8"858"sv, u8"cp858"sv},
        {u8"ibm858"sv, u8"cp858"sv},
        {u8"860"sv, u8"cp860"sv},
        {u8"ibm860"sv, u8"cp860"sv},
        {u8"861"sv, u8"cp861"sv},
        {u8"cp-is"sv, u8"cp861"sv},
        {u8"ibm861"sv, u8"cp861"sv},
        {u8"862"sv, u8"cp862"sv},
        {u8"ibm862"sv, u8"cp862"sv},
        {u8"863"sv, u8"cp863"sv},
        {u8"ibm863"sv, u8"cp863"sv},
        {u8"ibm864"sv, u8"cp864"sv},
        {u8"865"sv, u8"cp865"sv},
        {u8"ibm865"sv, u8"cp865"sv},
        {u8"866"sv, u8"cp866"sv},
        {u8"ibm866"sv, u8"cp866"sv},
        {u8"869"sv, u8"cp869"sv},
        {u8"cp-gr"sv, u8"cp869"sv},
        {u8"ibm869"sv, u8"cp869"sv},
        {u8"932"sv, u8"cp932"sv},
        {u8"ms932"sv, u8"cp932"sv},
        {u8"mskanji"sv, u8"cp932"sv},
        {u8"ms-kanji"sv, u8"cp932"sv},
        {u8"windows-31j"sv, u8"cp932"sv},
        {u8"949"sv, u8"cp949"sv},
        {u8"ms949"sv, u8"cp949"sv},
        {u8"uhc"sv, u8"cp949"sv},
        {u8"950"sv, u8"cp950"sv},
        {u8"ms950"sv, u8"cp950"sv},
        {u8"ibm1026"sv, u8"cp1026"sv},
        {u8"1125"sv, u8"cp1125"sv},
        {u8"ibm1125"sv, u8"cp1125"sv},
        {u8"cp866u"sv, u8"cp1125"sv},
        {u8"ruscii"sv, u8"cp1125"sv},
        {u8"ibm1140"sv, u8"cp1140"sv},
        {u8"windows-1250"sv, u8"cp1250"sv},
        {u8"windows-1251"sv, u8"cp1251"sv},
        {u8"windows-1252"sv, u8"cp1252"sv},
        {u8"windows-1253"sv, u8"cp1253"sv},
        {u8"windows-1254"sv, u8"cp1254"sv},
        {u8"windows-1255"sv, u8"cp1255"sv},
        {u8"windows-1256"sv, u8"cp1256"sv},
        {u8"windows-1257"sv, u8"cp1257"sv},
        {u8"windows-1258"sv, u8"cp1258"sv},
        {u8"eucjp"sv, u8"euc_jp"sv},
        {u8"ujis"sv, u8"euc_jp"sv},
        {u8"u-jis"sv, u8"euc_jp"sv},
        {u8"jisx0213"sv, u8"euc_jis_2004"sv},
        {u8"eucjis2004"sv, u8"euc_jis_2004"sv},
        {u8"eucjisx0213"sv, u8"euc_jisx0213"sv},
        {u8"euckr"sv, u8"euc_kr"sv},
        {u8"korean"sv, u8"euc_kr"sv},
        {u8"ksc5601"sv, u8"euc_kr"sv},
        {u8"ks_c-5601"sv, u8"euc_kr"sv},
        {u8"ks_c-5601-1987"sv, u8"euc_kr"sv},
        {u8"ksx1001"sv, u8"euc_kr"sv},
        {u8"ks_x-1001"sv, u8"euc_kr"sv},
        {u8"chinese"sv, u8"gb2312"sv},
        {u8"csiso58gb231280"sv, u8"gb2312"sv},
        {u8"euc-cn"sv, u8"gb2312"sv},
        {u8"euccn"sv, u8"gb2312"sv},
        {u8"eucgb2312-cn"sv, u8"gb2312"sv},
        {u8"gb2312-1980"sv, u8"gb2312"sv},
        {u8"gb2312-80"sv, u8"gb2312"sv},
        {u8"iso-ir-58"sv, u8"gb2312"sv},
        {u8"936"sv, u8"gbk"sv},
        {u8"cp936"sv, u8"gbk"sv},
        {u8"ms936"sv, u8"gbk"sv},
        {u8"gb18030-2000"sv, u8"gb18030"sv},
        {u8"hzgb"sv, u8"hz"sv},
        {u8"hz-gb"sv, u8"hz"sv},
        {u8"hz-gb-2312"sv, u8"hz"sv},
        {u8"csiso2022jp"sv, u8"iso2022_jp"sv},
        {u8"iso2022jp"sv, u8"iso2022_jp"sv},
        {u8"iso-2022-jp"sv, u8"iso2022_jp"sv},
        {u8"iso2022jp-1"sv, u8"iso2022_jp_1"sv},
        {u8"iso-2022-jp-1"sv, u8"iso2022_jp_1"sv},
        {u8"iso2022jp-2"sv, u8"iso2022_jp_2"sv},
        {u8"iso-2022-jp-2"sv, u8"iso2022_jp_2"sv},
        {u8"iso2022jp-2004"sv, u8"iso2022_jp_2004"sv},
        {u8"iso-2022-jp-2004"sv, u8"iso2022_jp_2004"sv},
        {u8"iso2022jp-3"sv, u8"iso2022_jp_3"sv},
        {u8"iso-2022-jp-3"sv, u8"iso2022_jp_3"sv},
        {u8"iso2022jp-ext"sv, u8"iso2022_jp_ext"sv},
        {u8"iso-2022-jp-ext"sv, u8"iso2022_jp_ext"sv},
        {u8"csiso2022kr"sv, u8"iso2022_kr"sv},
        {u8"iso2022kr"sv, u8"iso2022_kr"sv},
        {u8"iso-2022-kr"sv, u8"iso2022_kr"sv},
        {u8"iso-8859-1"sv, u8"latin_1"sv},
        {u8"iso8859-1"sv, u8"latin_1"sv},
        {u8"8859"sv, u8"latin_1"sv},
        {u8"cp819"sv, u8"latin_1"sv},
        {u8"latin"sv, u8"latin_1"sv},
        {u8"latin1"sv, u8"latin_1"sv},
        {u8"l1"sv, u8"latin_1"sv},
        {u8"iso-8859-2"sv, u8"iso8859_2"sv},
        {u8"latin2"sv, u8"iso8859_2"sv},
        {u8"l2"sv, u8"iso8859_2"sv},
        {u8"iso-8859-3"sv, u8"iso8859_3"sv},
        {u8"latin3"sv, u8"iso8859_3"sv},
        {u8"l3"sv, u8"iso8859_3"sv},
        {u8"iso-8859-4"sv, u8"iso8859_4"sv},
        {u8"latin4"sv, u8"iso8859_4"sv},
        {u8"l4"sv, u8"iso8859_4"sv},
        {u8"iso-8859-5"sv, u8"iso8859_5"sv},
        {u8"cyrillic"sv, u8"iso8859_5"sv},
        {u8"iso-8859-6"sv, u8"iso8859_6"sv},
        {u8"arabic"sv, u8"iso8859_6"sv},
        {u8"iso-8859-7"sv, u8"iso8859_7"sv},
        {u8"greek"sv, u8"iso8859_7"sv},
        {u8"greek8"sv, u8"iso8859_7"sv},
        {u8"iso-8859-8"sv, u8"iso8859_8"sv},
        {u8"hebrew"sv, u8"iso8859_8"sv},
        {u8"iso-8859-9"sv, u8"iso8859_9"sv},
        {u8"latin5"sv, u8"iso8859_9"sv},
        {u8"l5"sv, u8"iso8859_9"sv},
        {u8"iso-8859-10"sv, u8"iso8859_10"sv},
        {u8"latin6"sv, u8"iso8859_10"sv},
        {u8"l6"sv, u8"iso8859_10"sv},
        {u8"iso-8859-11"sv, u8"iso8859_11"sv},
        {u8"thai"sv, u8"iso8859_11"sv},
        {u8"iso-8859-13"sv, u8"iso8859_13"sv},
        {u8"latin7"sv, u8"iso8859_13"sv},
        {u8"l7"sv, u8"iso8859_13"sv},
        {u8"iso-8859-14"sv, u8"iso8859_14"sv},
        {u8"latin8"sv, u8"iso8859_14"sv},
        {u8"l8"sv, u8"iso8859_14"sv},
        {u8"iso-8859-15"sv, u8"iso8859_15"sv},
        {u8"latin9"sv, u8"iso8859_15"sv},
        {u8"l9"sv, u8"iso8859_15"sv},
        {u8"iso-8859-16"sv, u8"iso8859_16"sv},
        {u8"latin10"sv, u8"iso8859_16"sv},
        {u8"l10"sv, u8"iso8859_16"sv},
        {u8"cp1361"sv, u8"johab"sv},
        {u8"ms1361"sv, u8"johab"sv},
        {u8"kz_1048"sv, u8"kz1048"sv},
        {u8"strk1048_2002"sv, u8"kz1048"sv},
        {u8"rk1048"sv, u8"kz1048"sv},
        {u8"maccyrillic"sv, u8"mac_cyrillic"sv},
        {u8"macgreek"sv, u8"mac_greek"sv},
        {u8"maciceland"sv, u8"mac_iceland"sv},
        {u8"maclatin2"sv, u8"mac_latin2"sv},
        {u8"maccentraleurope"sv, u8"mac_latin2"sv},
        {u8"mac_centeuro"sv, u8"mac_latin2"sv},
        {u8"macroman"sv, u8"mac_roman"sv},
        {u8"macintosh"sv, u8"mac_roman"sv},
        {u8"macturkish"sv, u8"mac_turkish"sv},
        {u8"csptcp154"sv, u8"ptcp154"sv},
        {u8"pt154"sv, u8"ptcp154"sv},
        {u8"cp154"sv, u8"ptcp154"sv},
        {u8"cyrillic-asian"sv, u8"ptcp154"sv},
        {u8"csshiftjis"sv, u8"shift_jis"sv},
        {u8"shiftjis"sv, u8"shift_jis"sv},
        {u8"sjis"sv, u8"shift_jis"sv},
        {u8"s_jis"sv, u8"shift_jis"sv},
        {u8"shiftjis2004"sv, u8"shift_jis_2004"sv},
        {u8"sjis_2004"sv, u8"shift_jis_2004"sv},
        {u8"sjis2004"sv, u8"shift_jis_2004"sv},
        {u8"shiftjisx0213"sv, u8"shift_jisx0213"sv},
        {u8"sjisx0213"sv, u8"shift_jisx0213"sv},
        {u8"s_jisx0213"sv, u8"shift_jisx0213"sv},
        {u8"u32"sv, u8"utf_32"sv},
        {u8"utf32"sv, u8"utf_32"sv},
        {u8"utf-32be"sv, u8"utf_32_be"sv},
        {u8"utf-32le"sv, u8"utf_32_le"sv},
        {u8"u16"sv, u8"utf_16"sv},
        {u8"utf16"sv, u8"utf_16"sv},
        {u8"utf-16be"sv, u8"utf_16_be"sv},
        {u8"utf-16le"sv, u8"utf_16_le"sv},
        {u8"u7"sv, u8"utf_7"sv},
        {u8"unicode-1-1-utf-7"sv, u8"utf_7"sv},
        {u8"u8"sv, u8"utf_8"sv},
        {u8"utf"sv, u8"utf_8"sv},
        {u8"utf8"sv, u8"utf_8"sv},
        {u8"utf-8"sv, u8"utf_8"sv},
        {u8"cp65001"sv, u8"utf_8"sv},
    };

    /**
     * @brief Resolve encoding name alias and fetch real encoding name.
     * @param[in] lang The encoding name for finding.
     * @return
     * The given encoding name if given name not present in alias map.
     * Otherwise the found encoding name by given name.
    */
    static std::u8string resolve_encoding_alias(const std::u8string_view& enc_name) {
        auto name = op::to_lower(enc_name);
        auto finder = ALIAS_MAP.find(name);
        if (finder == ALIAS_MAP.end()) {
            // not found, use original encoding name.
            return std::u8string(enc_name);
        } else {
            // found, use found encoding name.
            return std::u8string(finder->second);
        }
    }

#if defined(YYCC_PYCODEC_WIN32_BACKEND)

    using CodePage = YYCC_PYCODEC_BACKEND_NS::CodePage;

    static const std::map<std::u8string_view, CodePage> WINCP_MAP{
        {u8"ascii"sv, static_cast<CodePage>(437u)},         {u8"big5"sv, static_cast<CodePage>(950u)},
        {u8"cp037"sv, static_cast<CodePage>(037u)},         {u8"cp437"sv, static_cast<CodePage>(437u)},
        {u8"cp500"sv, static_cast<CodePage>(500u)},         {u8"cp720"sv, static_cast<CodePage>(720u)},
        {u8"cp737"sv, static_cast<CodePage>(737u)},         {u8"cp775"sv, static_cast<CodePage>(775u)},
        {u8"cp850"sv, static_cast<CodePage>(850u)},         {u8"cp852"sv, static_cast<CodePage>(852u)},
        {u8"cp855"sv, static_cast<CodePage>(855u)},         {u8"cp857"sv, static_cast<CodePage>(857u)},
        {u8"cp858"sv, static_cast<CodePage>(858u)},         {u8"cp860"sv, static_cast<CodePage>(860u)},
        {u8"cp861"sv, static_cast<CodePage>(861u)},         {u8"cp862"sv, static_cast<CodePage>(862u)},
        {u8"cp863"sv, static_cast<CodePage>(863u)},         {u8"cp864"sv, static_cast<CodePage>(864u)},
        {u8"cp865"sv, static_cast<CodePage>(865u)},         {u8"cp866"sv, static_cast<CodePage>(866u)},
        {u8"cp869"sv, static_cast<CodePage>(869u)},         {u8"cp874"sv, static_cast<CodePage>(874u)},
        {u8"cp875"sv, static_cast<CodePage>(875u)},         {u8"cp932"sv, static_cast<CodePage>(932u)},
        {u8"cp949"sv, static_cast<CodePage>(949u)},         {u8"cp950"sv, static_cast<CodePage>(950u)},
        {u8"cp1026"sv, static_cast<CodePage>(1026u)},       {u8"cp1140"sv, static_cast<CodePage>(1140u)},
        {u8"cp1250"sv, static_cast<CodePage>(1250u)},       {u8"cp1251"sv, static_cast<CodePage>(1251u)},
        {u8"cp1252"sv, static_cast<CodePage>(1252u)},       {u8"cp1253"sv, static_cast<CodePage>(1253u)},
        {u8"cp1254"sv, static_cast<CodePage>(1254u)},       {u8"cp1255"sv, static_cast<CodePage>(1255u)},
        {u8"cp1256"sv, static_cast<CodePage>(1256u)},       {u8"cp1257"sv, static_cast<CodePage>(1257u)},
        {u8"cp1258"sv, static_cast<CodePage>(1258u)},       {u8"euc_jp"sv, static_cast<CodePage>(20932u)},
        {u8"euc_kr"sv, static_cast<CodePage>(51949u)},      {u8"gb2312"sv, static_cast<CodePage>(936u)},
        {u8"gbk"sv, static_cast<CodePage>(936u)},           {u8"gb18030"sv, static_cast<CodePage>(54936u)},
        {u8"hz"sv, static_cast<CodePage>(52936u)},          {u8"iso2022_jp"sv, static_cast<CodePage>(50220u)},
        {u8"iso2022_kr"sv, static_cast<CodePage>(50225u)},  {u8"latin_1"sv, static_cast<CodePage>(28591u)},
        {u8"iso8859_2"sv, static_cast<CodePage>(28592u)},   {u8"iso8859_3"sv, static_cast<CodePage>(28593u)},
        {u8"iso8859_4"sv, static_cast<CodePage>(28594u)},   {u8"iso8859_5"sv, static_cast<CodePage>(28595u)},
        {u8"iso8859_6"sv, static_cast<CodePage>(28596u)},   {u8"iso8859_7"sv, static_cast<CodePage>(28597u)},
        {u8"iso8859_8"sv, static_cast<CodePage>(28598u)},   {u8"iso8859_9"sv, static_cast<CodePage>(28599u)},
        {u8"iso8859_13"sv, static_cast<CodePage>(28603u)},  {u8"iso8859_15"sv, static_cast<CodePage>(28605u)},
        {u8"johab"sv, static_cast<CodePage>(1361u)},        {u8"mac_cyrillic"sv, static_cast<CodePage>(10007u)},
        {u8"mac_greek"sv, static_cast<CodePage>(10006u)},   {u8"mac_iceland"sv, static_cast<CodePage>(10079u)},
        {u8"mac_turkish"sv, static_cast<CodePage>(10081u)}, {u8"shift_jis"sv, static_cast<CodePage>(932u)},
        {u8"utf_7"sv, static_cast<CodePage>(65000u)},       {u8"utf_8"sv, static_cast<CodePage>(65001u)},
    };

    static FetchResult<CodePage> fetch_code_page(const std::u8string_view& enc_name) {
        // resolve alias
        std::u8string resolved_name = resolve_encoding_alias(enc_name);
        // find code page
        op::lower(resolved_name);
        auto finder = WINCP_MAP.find(resolved_name);
        if (finder == WINCP_MAP.end()) return std::unexpected(FetchError::NoSuchName);
        // okey, we found it.
        return finder->second;
    }

#else

    static const std::map<std::u8string_view, std::string_view> ICONV_MAP{
        {u8"ascii"sv, "ASCII"sv},
        {u8"big5"sv, "BIG5"sv},
        {u8"big5hkscs"sv, "BIG5-HKSCS"sv},
        {u8"cp850"sv, "CP850"sv},
        {u8"cp862"sv, "CP862"sv},
        {u8"cp866"sv, "CP866"sv},
        {u8"cp874"sv, "CP874"sv},
        {u8"cp932"sv, "CP932"sv},
        {u8"cp949"sv, "CP949"sv},
        {u8"cp950"sv, "CP950"sv},
        {u8"cp1250"sv, "CP1250"sv},
        {u8"cp1251"sv, "CP1251"sv},
        {u8"cp1252"sv, "CP1252"sv},
        {u8"cp1253"sv, "CP1253"sv},
        {u8"cp1254"sv, "CP1254"sv},
        {u8"cp1255"sv, "CP1255"sv},
        {u8"cp1256"sv, "CP1256"sv},
        {u8"cp1257"sv, "CP1257"sv},
        {u8"cp1258"sv, "CP1258"sv},
        {u8"euc_jp"sv, "EUC-JP"sv},
        {u8"euc_kr"sv, "EUC-KR"sv},
        {u8"gb2312"sv, "CP936"sv},
        {u8"gbk"sv, "GBK"sv},
        {u8"gb18030"sv, "GB18030"sv},
        {u8"hz"sv, "HZ"sv},
        {u8"iso2022_jp"sv, "ISO-2022-JP"sv},
        {u8"iso2022_jp_1"sv, "ISO-2022-JP-1"sv},
        {u8"iso2022_jp_2"sv, "ISO-2022-JP-2"sv},
        {u8"iso2022_kr"sv, "ISO-2022-KR"sv},
        {u8"latin_1"sv, "ISO-8859-1"sv},
        {u8"iso8859_2"sv, "ISO-8859-2"sv},
        {u8"iso8859_3"sv, "ISO-8859-3"sv},
        {u8"iso8859_4"sv, "ISO-8859-4"sv},
        {u8"iso8859_5"sv, "ISO-8859-5"sv},
        {u8"iso8859_6"sv, "ISO-8859-6"sv},
        {u8"iso8859_7"sv, "ISO-8859-7"sv},
        {u8"iso8859_8"sv, "ISO-8859-8"sv},
        {u8"iso8859_9"sv, "ISO-8859-9"sv},
        {u8"iso8859_10"sv, "ISO-8859-10"sv},
        {u8"iso8859_11"sv, "ISO-8859-11"sv},
        {u8"iso8859_13"sv, "ISO-8859-13"sv},
        {u8"iso8859_14"sv, "ISO-8859-14"sv},
        {u8"iso8859_15"sv, "ISO-8859-15"sv},
        {u8"iso8859_16"sv, "ISO-8859-16"sv},
        {u8"johab"sv, "JOHAB"sv},
        {u8"koi8_t"sv, "KOI8-T"sv},
        {u8"mac_cyrillic"sv, "MacCyrillic"sv},
        {u8"mac_greek"sv, "MacGreek"sv},
        {u8"mac_iceland"sv, "MacIceland"sv},
        {u8"mac_roman"sv, "MacRoman"sv},
        {u8"mac_turkish"sv, "MacTurkish"sv},
        {u8"ptcp154"sv, "PT154"sv},
        {u8"shift_jis"sv, "SHIFT_JIS"sv},
        {u8"utf_32"sv, "UTF-32"sv},
        {u8"utf_32_be"sv, "UTF-32BE"sv},
        {u8"utf_32_le"sv, "UTF-32LE"sv},
        {u8"utf_16"sv, "UTF16"sv},
        {u8"utf_16_be"sv, "UTF-16BE"sv},
        {u8"utf_16_le"sv, "UTF-16LE"sv},
        {u8"utf_7"sv, "UTF-7"sv},
        {u8"utf_8"sv, "UTF-8"sv},
    };

    static FetchResult<std::string_view> fetch_iconv_name(const std::u8string_view& enc_name) {
        // resolve alias
        std::u8string resolved_name = resolve_encoding_alias(enc_name);
        // find code page
        op::lower(resolved_name);
        auto finder = ICONV_MAP.find(resolved_name);
        if (finder == ICONV_MAP.end()) return std::unexpected(FetchError::NoSuchName);
        // okey, we found it.
        return finder->second;
    }

#endif

#pragma endregion

#pragma region Misc

    ConvError::ConvError(const ConvBackendError& err) : inner(err) {}

    ConvError::ConvError(const ConvFrontendError& err) : inner(err) {}

    ConvError::ConvError(ConvBackendError&& err) noexcept : inner(std::move(err)) {}

    ConvError::ConvError(ConvFrontendError&& err) noexcept : inner(std::move(err)) {}

    bool is_valid_encoding_name(const EncodingName& name) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return fetch_code_page(name).has_value();
#else
        return fetch_iconv_name(name).has_value();
#endif
    }

#pragma endregion

#pragma region Char -> UTF8

    //     CharToUtf8::CharToUtf8(const EncodingName& name) :
    // #if defined(YYCC_PYCODEC_WIN32_BACKEND)
    //         code_page(fetch)
    // #else
    //         inner(fetch_iconv_name())
    // #endif
    //     {}

    CharToUtf8::CharToUtf8(const EncodingName& name) : inner(std::nullopt) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        auto rv = fetch_code_page(name);
        if (rv.has_value()) inner = rv.value();
#else
        auto rv = fetch_iconv_name(name);
        if (rv.has_value()) inner = YYCC_PYCODEC_BACKEND_NS::CharToUtf8(rv.value());
#endif
    }

    CharToUtf8::~CharToUtf8() {}

    ConvResult<std::u8string> CharToUtf8::to_utf8(const std::string_view& src) {
        if (!inner.has_value()) return std::unexpected(ConvFrontendError::NoSuchName);
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf8(src, inner.value());
#else
        return inner.value().to_utf8(src);
#endif
    }

#pragma endregion

#pragma region UTF8 -> Char

    Utf8ToChar::Utf8ToChar(const EncodingName& name) : inner(std::nullopt) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        auto rv = fetch_code_page(name);
        if (rv.has_value()) inner = rv.value();
#else
        auto rv = fetch_iconv_name(name);
        if (rv.has_value()) inner = YYCC_PYCODEC_BACKEND_NS::CharToUtf8(rv.value());
#endif
    }

    Utf8ToChar::~Utf8ToChar() {}

    ConvResult<std::string> Utf8ToChar::to_char(const std::u8string_view& src) {
        if (!inner.has_value()) return std::unexpected(ConvFrontendError::NoSuchName);
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_char(src, inner.value());
#else
        return inner.value().to_char(src);
#endif
    }

#pragma endregion

#pragma region WChar -> UTF8

    WcharToUtf8::WcharToUtf8() : inner() {}

    WcharToUtf8::~WcharToUtf8() {}

    ConvResult<std::u8string> WcharToUtf8::to_utf8(const std::wstring_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf8(src);
#else
        return inner.to_utf8(src);
#endif
    }

#pragma endregion

#pragma region UTF8 -> WChar

    Utf8ToWchar::Utf8ToWchar() : inner() {}

    Utf8ToWchar::~Utf8ToWchar() {}

    ConvResult<std::wstring> Utf8ToWchar::to_wchar(const std::u8string_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_wchar(src);
#else
        return inner.to_wchar(src);
#endif
    }

#pragma endregion

#pragma region UTF8 -> UTF16

    Utf8ToUtf16::Utf8ToUtf16() : inner() {}

    Utf8ToUtf16::~Utf8ToUtf16() {}

    ConvResult<std::u16string> Utf8ToUtf16::to_utf16(const std::u8string_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf16(src);
#else
        return inner.to_utf16(src);
#endif
    }

#pragma endregion

#pragma region UTF16 -> UTF8

    Utf16ToUtf8::Utf16ToUtf8() : inner() {}

    Utf16ToUtf8::~Utf16ToUtf8() {}

    ConvResult<std::u8string> Utf16ToUtf8::to_utf8(const std::u16string_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf8(src);
#else
        return inner.to_utf8(src);
#endif
    }

#pragma endregion

#pragma region UTF8 -> UTF32

    Utf8ToUtf32::Utf8ToUtf32() : inner() {}

    Utf8ToUtf32::~Utf8ToUtf32() {}

    ConvResult<std::u32string> Utf8ToUtf32::to_utf32(const std::u8string_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf32(src);
#else
        return inner.to_utf32(src);
#endif
    }

#pragma endregion

#pragma region UTF32 -> UTF8

    Utf32ToUtf8::Utf32ToUtf8() : inner() {}

    Utf32ToUtf8::~Utf32ToUtf8() {}

    ConvResult<std::u8string> Utf32ToUtf8::to_utf8(const std::u32string_view& src) {
#if defined(YYCC_PYCODEC_WIN32_BACKEND)
        return YYCC_PYCODEC_BACKEND_NS::to_utf8(src);
#else
        return inner.to_utf8(src);
#endif
    }

#pragma endregion

} // namespace yycc::carton::pycodec
