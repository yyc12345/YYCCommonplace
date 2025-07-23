#include "pycodec.hpp"
#include <map>

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::pycodec {

#pragma region Encoding Name

    static const std::map<NS_YYCC_STRING::u8string, NS_YYCC_STRING::u8string> ALISA_MAP {
        { YYCC_U8("646"), YYCC_U8("ascii") },
        { YYCC_U8("us-ascii"), YYCC_U8("ascii") },
        { YYCC_U8("big5-tw"), YYCC_U8("big5") },
        { YYCC_U8("csbig5"), YYCC_U8("big5") },
        { YYCC_U8("big5-hkscs"), YYCC_U8("big5hkscs") },
        { YYCC_U8("hkscs"), YYCC_U8("big5hkscs") },
        { YYCC_U8("ibm037"), YYCC_U8("cp037") },
        { YYCC_U8("ibm039"), YYCC_U8("cp037") },
        { YYCC_U8("273"), YYCC_U8("cp273") },
        { YYCC_U8("ibm273"), YYCC_U8("cp273") },
        { YYCC_U8("csibm273"), YYCC_U8("cp273") },
        { YYCC_U8("ebcdic-cp-he"), YYCC_U8("cp424") },
        { YYCC_U8("ibm424"), YYCC_U8("cp424") },
        { YYCC_U8("437"), YYCC_U8("cp437") },
        { YYCC_U8("ibm437"), YYCC_U8("cp437") },
        { YYCC_U8("ebcdic-cp-be"), YYCC_U8("cp500") },
        { YYCC_U8("ebcdic-cp-ch"), YYCC_U8("cp500") },
        { YYCC_U8("ibm500"), YYCC_U8("cp500") },
        { YYCC_U8("ibm775"), YYCC_U8("cp775") },
        { YYCC_U8("850"), YYCC_U8("cp850") },
        { YYCC_U8("ibm850"), YYCC_U8("cp850") },
        { YYCC_U8("852"), YYCC_U8("cp852") },
        { YYCC_U8("ibm852"), YYCC_U8("cp852") },
        { YYCC_U8("855"), YYCC_U8("cp855") },
        { YYCC_U8("ibm855"), YYCC_U8("cp855") },
        { YYCC_U8("857"), YYCC_U8("cp857") },
        { YYCC_U8("ibm857"), YYCC_U8("cp857") },
        { YYCC_U8("858"), YYCC_U8("cp858") },
        { YYCC_U8("ibm858"), YYCC_U8("cp858") },
        { YYCC_U8("860"), YYCC_U8("cp860") },
        { YYCC_U8("ibm860"), YYCC_U8("cp860") },
        { YYCC_U8("861"), YYCC_U8("cp861") },
        { YYCC_U8("cp-is"), YYCC_U8("cp861") },
        { YYCC_U8("ibm861"), YYCC_U8("cp861") },
        { YYCC_U8("862"), YYCC_U8("cp862") },
        { YYCC_U8("ibm862"), YYCC_U8("cp862") },
        { YYCC_U8("863"), YYCC_U8("cp863") },
        { YYCC_U8("ibm863"), YYCC_U8("cp863") },
        { YYCC_U8("ibm864"), YYCC_U8("cp864") },
        { YYCC_U8("865"), YYCC_U8("cp865") },
        { YYCC_U8("ibm865"), YYCC_U8("cp865") },
        { YYCC_U8("866"), YYCC_U8("cp866") },
        { YYCC_U8("ibm866"), YYCC_U8("cp866") },
        { YYCC_U8("869"), YYCC_U8("cp869") },
        { YYCC_U8("cp-gr"), YYCC_U8("cp869") },
        { YYCC_U8("ibm869"), YYCC_U8("cp869") },
        { YYCC_U8("932"), YYCC_U8("cp932") },
        { YYCC_U8("ms932"), YYCC_U8("cp932") },
        { YYCC_U8("mskanji"), YYCC_U8("cp932") },
        { YYCC_U8("ms-kanji"), YYCC_U8("cp932") },
        { YYCC_U8("windows-31j"), YYCC_U8("cp932") },
        { YYCC_U8("949"), YYCC_U8("cp949") },
        { YYCC_U8("ms949"), YYCC_U8("cp949") },
        { YYCC_U8("uhc"), YYCC_U8("cp949") },
        { YYCC_U8("950"), YYCC_U8("cp950") },
        { YYCC_U8("ms950"), YYCC_U8("cp950") },
        { YYCC_U8("ibm1026"), YYCC_U8("cp1026") },
        { YYCC_U8("1125"), YYCC_U8("cp1125") },
        { YYCC_U8("ibm1125"), YYCC_U8("cp1125") },
        { YYCC_U8("cp866u"), YYCC_U8("cp1125") },
        { YYCC_U8("ruscii"), YYCC_U8("cp1125") },
        { YYCC_U8("ibm1140"), YYCC_U8("cp1140") },
        { YYCC_U8("windows-1250"), YYCC_U8("cp1250") },
        { YYCC_U8("windows-1251"), YYCC_U8("cp1251") },
        { YYCC_U8("windows-1252"), YYCC_U8("cp1252") },
        { YYCC_U8("windows-1253"), YYCC_U8("cp1253") },
        { YYCC_U8("windows-1254"), YYCC_U8("cp1254") },
        { YYCC_U8("windows-1255"), YYCC_U8("cp1255") },
        { YYCC_U8("windows-1256"), YYCC_U8("cp1256") },
        { YYCC_U8("windows-1257"), YYCC_U8("cp1257") },
        { YYCC_U8("windows-1258"), YYCC_U8("cp1258") },
        { YYCC_U8("eucjp"), YYCC_U8("euc_jp") },
        { YYCC_U8("ujis"), YYCC_U8("euc_jp") },
        { YYCC_U8("u-jis"), YYCC_U8("euc_jp") },
        { YYCC_U8("jisx0213"), YYCC_U8("euc_jis_2004") },
        { YYCC_U8("eucjis2004"), YYCC_U8("euc_jis_2004") },
        { YYCC_U8("eucjisx0213"), YYCC_U8("euc_jisx0213") },
        { YYCC_U8("euckr"), YYCC_U8("euc_kr") },
        { YYCC_U8("korean"), YYCC_U8("euc_kr") },
        { YYCC_U8("ksc5601"), YYCC_U8("euc_kr") },
        { YYCC_U8("ks_c-5601"), YYCC_U8("euc_kr") },
        { YYCC_U8("ks_c-5601-1987"), YYCC_U8("euc_kr") },
        { YYCC_U8("ksx1001"), YYCC_U8("euc_kr") },
        { YYCC_U8("ks_x-1001"), YYCC_U8("euc_kr") },
        { YYCC_U8("chinese"), YYCC_U8("gb2312") },
        { YYCC_U8("csiso58gb231280"), YYCC_U8("gb2312") },
        { YYCC_U8("euc-cn"), YYCC_U8("gb2312") },
        { YYCC_U8("euccn"), YYCC_U8("gb2312") },
        { YYCC_U8("eucgb2312-cn"), YYCC_U8("gb2312") },
        { YYCC_U8("gb2312-1980"), YYCC_U8("gb2312") },
        { YYCC_U8("gb2312-80"), YYCC_U8("gb2312") },
        { YYCC_U8("iso-ir-58"), YYCC_U8("gb2312") },
        { YYCC_U8("936"), YYCC_U8("gbk") },
        { YYCC_U8("cp936"), YYCC_U8("gbk") },
        { YYCC_U8("ms936"), YYCC_U8("gbk") },
        { YYCC_U8("gb18030-2000"), YYCC_U8("gb18030") },
        { YYCC_U8("hzgb"), YYCC_U8("hz") },
        { YYCC_U8("hz-gb"), YYCC_U8("hz") },
        { YYCC_U8("hz-gb-2312"), YYCC_U8("hz") },
        { YYCC_U8("csiso2022jp"), YYCC_U8("iso2022_jp") },
        { YYCC_U8("iso2022jp"), YYCC_U8("iso2022_jp") },
        { YYCC_U8("iso-2022-jp"), YYCC_U8("iso2022_jp") },
        { YYCC_U8("iso2022jp-1"), YYCC_U8("iso2022_jp_1") },
        { YYCC_U8("iso-2022-jp-1"), YYCC_U8("iso2022_jp_1") },
        { YYCC_U8("iso2022jp-2"), YYCC_U8("iso2022_jp_2") },
        { YYCC_U8("iso-2022-jp-2"), YYCC_U8("iso2022_jp_2") },
        { YYCC_U8("iso2022jp-2004"), YYCC_U8("iso2022_jp_2004") },
        { YYCC_U8("iso-2022-jp-2004"), YYCC_U8("iso2022_jp_2004") },
        { YYCC_U8("iso2022jp-3"), YYCC_U8("iso2022_jp_3") },
        { YYCC_U8("iso-2022-jp-3"), YYCC_U8("iso2022_jp_3") },
        { YYCC_U8("iso2022jp-ext"), YYCC_U8("iso2022_jp_ext") },
        { YYCC_U8("iso-2022-jp-ext"), YYCC_U8("iso2022_jp_ext") },
        { YYCC_U8("csiso2022kr"), YYCC_U8("iso2022_kr") },
        { YYCC_U8("iso2022kr"), YYCC_U8("iso2022_kr") },
        { YYCC_U8("iso-2022-kr"), YYCC_U8("iso2022_kr") },
        { YYCC_U8("iso-8859-1"), YYCC_U8("latin_1") },
        { YYCC_U8("iso8859-1"), YYCC_U8("latin_1") },
        { YYCC_U8("8859"), YYCC_U8("latin_1") },
        { YYCC_U8("cp819"), YYCC_U8("latin_1") },
        { YYCC_U8("latin"), YYCC_U8("latin_1") },
        { YYCC_U8("latin1"), YYCC_U8("latin_1") },
        { YYCC_U8("l1"), YYCC_U8("latin_1") },
        { YYCC_U8("iso-8859-2"), YYCC_U8("iso8859_2") },
        { YYCC_U8("latin2"), YYCC_U8("iso8859_2") },
        { YYCC_U8("l2"), YYCC_U8("iso8859_2") },
        { YYCC_U8("iso-8859-3"), YYCC_U8("iso8859_3") },
        { YYCC_U8("latin3"), YYCC_U8("iso8859_3") },
        { YYCC_U8("l3"), YYCC_U8("iso8859_3") },
        { YYCC_U8("iso-8859-4"), YYCC_U8("iso8859_4") },
        { YYCC_U8("latin4"), YYCC_U8("iso8859_4") },
        { YYCC_U8("l4"), YYCC_U8("iso8859_4") },
        { YYCC_U8("iso-8859-5"), YYCC_U8("iso8859_5") },
        { YYCC_U8("cyrillic"), YYCC_U8("iso8859_5") },
        { YYCC_U8("iso-8859-6"), YYCC_U8("iso8859_6") },
        { YYCC_U8("arabic"), YYCC_U8("iso8859_6") },
        { YYCC_U8("iso-8859-7"), YYCC_U8("iso8859_7") },
        { YYCC_U8("greek"), YYCC_U8("iso8859_7") },
        { YYCC_U8("greek8"), YYCC_U8("iso8859_7") },
        { YYCC_U8("iso-8859-8"), YYCC_U8("iso8859_8") },
        { YYCC_U8("hebrew"), YYCC_U8("iso8859_8") },
        { YYCC_U8("iso-8859-9"), YYCC_U8("iso8859_9") },
        { YYCC_U8("latin5"), YYCC_U8("iso8859_9") },
        { YYCC_U8("l5"), YYCC_U8("iso8859_9") },
        { YYCC_U8("iso-8859-10"), YYCC_U8("iso8859_10") },
        { YYCC_U8("latin6"), YYCC_U8("iso8859_10") },
        { YYCC_U8("l6"), YYCC_U8("iso8859_10") },
        { YYCC_U8("iso-8859-11"), YYCC_U8("iso8859_11") },
        { YYCC_U8("thai"), YYCC_U8("iso8859_11") },
        { YYCC_U8("iso-8859-13"), YYCC_U8("iso8859_13") },
        { YYCC_U8("latin7"), YYCC_U8("iso8859_13") },
        { YYCC_U8("l7"), YYCC_U8("iso8859_13") },
        { YYCC_U8("iso-8859-14"), YYCC_U8("iso8859_14") },
        { YYCC_U8("latin8"), YYCC_U8("iso8859_14") },
        { YYCC_U8("l8"), YYCC_U8("iso8859_14") },
        { YYCC_U8("iso-8859-15"), YYCC_U8("iso8859_15") },
        { YYCC_U8("latin9"), YYCC_U8("iso8859_15") },
        { YYCC_U8("l9"), YYCC_U8("iso8859_15") },
        { YYCC_U8("iso-8859-16"), YYCC_U8("iso8859_16") },
        { YYCC_U8("latin10"), YYCC_U8("iso8859_16") },
        { YYCC_U8("l10"), YYCC_U8("iso8859_16") },
        { YYCC_U8("cp1361"), YYCC_U8("johab") },
        { YYCC_U8("ms1361"), YYCC_U8("johab") },
        { YYCC_U8("kz_1048"), YYCC_U8("kz1048") },
        { YYCC_U8("strk1048_2002"), YYCC_U8("kz1048") },
        { YYCC_U8("rk1048"), YYCC_U8("kz1048") },
        { YYCC_U8("maccyrillic"), YYCC_U8("mac_cyrillic") },
        { YYCC_U8("macgreek"), YYCC_U8("mac_greek") },
        { YYCC_U8("maciceland"), YYCC_U8("mac_iceland") },
        { YYCC_U8("maclatin2"), YYCC_U8("mac_latin2") },
        { YYCC_U8("maccentraleurope"), YYCC_U8("mac_latin2") },
        { YYCC_U8("mac_centeuro"), YYCC_U8("mac_latin2") },
        { YYCC_U8("macroman"), YYCC_U8("mac_roman") },
        { YYCC_U8("macintosh"), YYCC_U8("mac_roman") },
        { YYCC_U8("macturkish"), YYCC_U8("mac_turkish") },
        { YYCC_U8("csptcp154"), YYCC_U8("ptcp154") },
        { YYCC_U8("pt154"), YYCC_U8("ptcp154") },
        { YYCC_U8("cp154"), YYCC_U8("ptcp154") },
        { YYCC_U8("cyrillic-asian"), YYCC_U8("ptcp154") },
        { YYCC_U8("csshiftjis"), YYCC_U8("shift_jis") },
        { YYCC_U8("shiftjis"), YYCC_U8("shift_jis") },
        { YYCC_U8("sjis"), YYCC_U8("shift_jis") },
        { YYCC_U8("s_jis"), YYCC_U8("shift_jis") },
        { YYCC_U8("shiftjis2004"), YYCC_U8("shift_jis_2004") },
        { YYCC_U8("sjis_2004"), YYCC_U8("shift_jis_2004") },
        { YYCC_U8("sjis2004"), YYCC_U8("shift_jis_2004") },
        { YYCC_U8("shiftjisx0213"), YYCC_U8("shift_jisx0213") },
        { YYCC_U8("sjisx0213"), YYCC_U8("shift_jisx0213") },
        { YYCC_U8("s_jisx0213"), YYCC_U8("shift_jisx0213") },
        { YYCC_U8("u32"), YYCC_U8("utf_32") },
        { YYCC_U8("utf32"), YYCC_U8("utf_32") },
        { YYCC_U8("utf-32be"), YYCC_U8("utf_32_be") },
        { YYCC_U8("utf-32le"), YYCC_U8("utf_32_le") },
        { YYCC_U8("u16"), YYCC_U8("utf_16") },
        { YYCC_U8("utf16"), YYCC_U8("utf_16") },
        { YYCC_U8("utf-16be"), YYCC_U8("utf_16_be") },
        { YYCC_U8("utf-16le"), YYCC_U8("utf_16_le") },
        { YYCC_U8("u7"), YYCC_U8("utf_7") },
        { YYCC_U8("unicode-1-1-utf-7"), YYCC_U8("utf_7") },
        { YYCC_U8("u8"), YYCC_U8("utf_8") },
        { YYCC_U8("utf"), YYCC_U8("utf_8") },
        { YYCC_U8("utf8"), YYCC_U8("utf_8") },
        { YYCC_U8("utf-8"), YYCC_U8("utf_8") },
        { YYCC_U8("cp65001"), YYCC_U8("utf_8") },
    };


#if defined(YYCC_PYCODEC_WIN32_BACKEND)

    using CodePage = NS_YYCC_ENCODING_BACKEND::CodePage;

    static const std::map<NS_YYCC_STRING::u8string, CodePage> WINCP_MAP {
        { YYCC_U8("ascii"), static_cast<CodePage>(437u) },
        { YYCC_U8("big5"), static_cast<CodePage>(950u) },
        { YYCC_U8("cp037"), static_cast<CodePage>(037u) },
        { YYCC_U8("cp437"), static_cast<CodePage>(437u) },
        { YYCC_U8("cp500"), static_cast<CodePage>(500u) },
        { YYCC_U8("cp720"), static_cast<CodePage>(720u) },
        { YYCC_U8("cp737"), static_cast<CodePage>(737u) },
        { YYCC_U8("cp775"), static_cast<CodePage>(775u) },
        { YYCC_U8("cp850"), static_cast<CodePage>(850u) },
        { YYCC_U8("cp852"), static_cast<CodePage>(852u) },
        { YYCC_U8("cp855"), static_cast<CodePage>(855u) },
        { YYCC_U8("cp857"), static_cast<CodePage>(857u) },
        { YYCC_U8("cp858"), static_cast<CodePage>(858u) },
        { YYCC_U8("cp860"), static_cast<CodePage>(860u) },
        { YYCC_U8("cp861"), static_cast<CodePage>(861u) },
        { YYCC_U8("cp862"), static_cast<CodePage>(862u) },
        { YYCC_U8("cp863"), static_cast<CodePage>(863u) },
        { YYCC_U8("cp864"), static_cast<CodePage>(864u) },
        { YYCC_U8("cp865"), static_cast<CodePage>(865u) },
        { YYCC_U8("cp866"), static_cast<CodePage>(866u) },
        { YYCC_U8("cp869"), static_cast<CodePage>(869u) },
        { YYCC_U8("cp874"), static_cast<CodePage>(874u) },
        { YYCC_U8("cp875"), static_cast<CodePage>(875u) },
        { YYCC_U8("cp932"), static_cast<CodePage>(932u) },
        { YYCC_U8("cp949"), static_cast<CodePage>(949u) },
        { YYCC_U8("cp950"), static_cast<CodePage>(950u) },
        { YYCC_U8("cp1026"), static_cast<CodePage>(1026u) },
        { YYCC_U8("cp1140"), static_cast<CodePage>(1140u) },
        { YYCC_U8("cp1250"), static_cast<CodePage>(1250u) },
        { YYCC_U8("cp1251"), static_cast<CodePage>(1251u) },
        { YYCC_U8("cp1252"), static_cast<CodePage>(1252u) },
        { YYCC_U8("cp1253"), static_cast<CodePage>(1253u) },
        { YYCC_U8("cp1254"), static_cast<CodePage>(1254u) },
        { YYCC_U8("cp1255"), static_cast<CodePage>(1255u) },
        { YYCC_U8("cp1256"), static_cast<CodePage>(1256u) },
        { YYCC_U8("cp1257"), static_cast<CodePage>(1257u) },
        { YYCC_U8("cp1258"), static_cast<CodePage>(1258u) },
        { YYCC_U8("euc_jp"), static_cast<CodePage>(20932u) },
        { YYCC_U8("euc_kr"), static_cast<CodePage>(51949u) },
        { YYCC_U8("gb2312"), static_cast<CodePage>(936u) },
        { YYCC_U8("gbk"), static_cast<CodePage>(936u) },
        { YYCC_U8("gb18030"), static_cast<CodePage>(54936u) },
        { YYCC_U8("hz"), static_cast<CodePage>(52936u) },
        { YYCC_U8("iso2022_jp"), static_cast<CodePage>(50220u) },
        { YYCC_U8("iso2022_kr"), static_cast<CodePage>(50225u) },
        { YYCC_U8("latin_1"), static_cast<CodePage>(28591u) },
        { YYCC_U8("iso8859_2"), static_cast<CodePage>(28592u) },
        { YYCC_U8("iso8859_3"), static_cast<CodePage>(28593u) },
        { YYCC_U8("iso8859_4"), static_cast<CodePage>(28594u) },
        { YYCC_U8("iso8859_5"), static_cast<CodePage>(28595u) },
        { YYCC_U8("iso8859_6"), static_cast<CodePage>(28596u) },
        { YYCC_U8("iso8859_7"), static_cast<CodePage>(28597u) },
        { YYCC_U8("iso8859_8"), static_cast<CodePage>(28598u) },
        { YYCC_U8("iso8859_9"), static_cast<CodePage>(28599u) },
        { YYCC_U8("iso8859_13"), static_cast<CodePage>(28603u) },
        { YYCC_U8("iso8859_15"), static_cast<CodePage>(28605u) },
        { YYCC_U8("johab"), static_cast<CodePage>(1361u) },
        { YYCC_U8("mac_cyrillic"), static_cast<CodePage>(10007u) },
        { YYCC_U8("mac_greek"), static_cast<CodePage>(10006u) },
        { YYCC_U8("mac_iceland"), static_cast<CodePage>(10079u) },
        { YYCC_U8("mac_turkish"), static_cast<CodePage>(10081u) },
        { YYCC_U8("shift_jis"), static_cast<CodePage>(932u) },
        { YYCC_U8("utf_7"), static_cast<CodePage>(65000u) },
        { YYCC_U8("utf_8"), static_cast<CodePage>(65001u) },
    };

#else

    static const std::map<NS_YYCC_STRING::u8string, std::string> ICONV_MAP {
        { YYCC_U8("ascii"), "ASCII" },
        { YYCC_U8("big5"), "BIG5" },
        { YYCC_U8("big5hkscs"), "BIG5-HKSCS" },
        { YYCC_U8("cp850"), "CP850" },
        { YYCC_U8("cp862"), "CP862" },
        { YYCC_U8("cp866"), "CP866" },
        { YYCC_U8("cp874"), "CP874" },
        { YYCC_U8("cp932"), "CP932" },
        { YYCC_U8("cp949"), "CP949" },
        { YYCC_U8("cp950"), "CP950" },
        { YYCC_U8("cp1250"), "CP1250" },
        { YYCC_U8("cp1251"), "CP1251" },
        { YYCC_U8("cp1252"), "CP1252" },
        { YYCC_U8("cp1253"), "CP1253" },
        { YYCC_U8("cp1254"), "CP1254" },
        { YYCC_U8("cp1255"), "CP1255" },
        { YYCC_U8("cp1256"), "CP1256" },
        { YYCC_U8("cp1257"), "CP1257" },
        { YYCC_U8("cp1258"), "CP1258" },
        { YYCC_U8("euc_jp"), "EUC-JP" },
        { YYCC_U8("euc_kr"), "EUC-KR" },
        { YYCC_U8("gb2312"), "CP936" },
        { YYCC_U8("gbk"), "GBK" },
        { YYCC_U8("gb18030"), "GB18030" },
        { YYCC_U8("hz"), "HZ" },
        { YYCC_U8("iso2022_jp"), "ISO-2022-JP" },
        { YYCC_U8("iso2022_jp_1"), "ISO-2022-JP-1" },
        { YYCC_U8("iso2022_jp_2"), "ISO-2022-JP-2" },
        { YYCC_U8("iso2022_kr"), "ISO-2022-KR" },
        { YYCC_U8("latin_1"), "ISO-8859-1" },
        { YYCC_U8("iso8859_2"), "ISO-8859-2" },
        { YYCC_U8("iso8859_3"), "ISO-8859-3" },
        { YYCC_U8("iso8859_4"), "ISO-8859-4" },
        { YYCC_U8("iso8859_5"), "ISO-8859-5" },
        { YYCC_U8("iso8859_6"), "ISO-8859-6" },
        { YYCC_U8("iso8859_7"), "ISO-8859-7" },
        { YYCC_U8("iso8859_8"), "ISO-8859-8" },
        { YYCC_U8("iso8859_9"), "ISO-8859-9" },
        { YYCC_U8("iso8859_10"), "ISO-8859-10" },
        { YYCC_U8("iso8859_11"), "ISO-8859-11" },
        { YYCC_U8("iso8859_13"), "ISO-8859-13" },
        { YYCC_U8("iso8859_14"), "ISO-8859-14" },
        { YYCC_U8("iso8859_15"), "ISO-8859-15" },
        { YYCC_U8("iso8859_16"), "ISO-8859-16" },
        { YYCC_U8("johab"), "JOHAB" },
        { YYCC_U8("koi8_t"), "KOI8-T" },
        { YYCC_U8("mac_cyrillic"), "MacCyrillic" },
        { YYCC_U8("mac_greek"), "MacGreek" },
        { YYCC_U8("mac_iceland"), "MacIceland" },
        { YYCC_U8("mac_roman"), "MacRoman" },
        { YYCC_U8("mac_turkish"), "MacTurkish" },
        { YYCC_U8("ptcp154"), "PT154" },
        { YYCC_U8("shift_jis"), "SHIFT_JIS" },
        { YYCC_U8("utf_32"), "UTF-32" },
        { YYCC_U8("utf_32_be"), "UTF-32BE" },
        { YYCC_U8("utf_32_le"), "UTF-32LE" },
        { YYCC_U8("utf_16"), "UTF16" },
        { YYCC_U8("utf_16_be"), "UTF-16BE" },
        { YYCC_U8("utf_16_le"), "UTF-16LE" },
        { YYCC_U8("utf_7"), "UTF-7" },
        { YYCC_U8("utf_8"), "UTF-8" },
    };

#endif


#pragma endregion

#pragma region Misc

    ConvError::ConvError(const ConvError::Error& err) : inner(err) {}

    bool is_valid_encoding_name(const EncodingName& name) {

    }

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

#pragma region

#pragma endregion

} // namespace yycc::encoding::pycodec
