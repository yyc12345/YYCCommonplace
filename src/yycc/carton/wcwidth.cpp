#include "wcwidth.hpp"
#include "../encoding/stl.hpp"
#include <utility>
#include <vector>
#include <optional>

#define ENC ::yycc::encoding::stl

namespace yycc::carton::wcwidth {

    using Boundary = std::pair<char32_t, char32_t>;
    using BoundaryVector = std::vector<Boundary>;

    // YYC MARK:
    // Following table and code are copied from Python package "wcwidth".
    // Although the code of this package are also copied from the original "wcwidth" C implementation.
    //
    // I do not need so much exact measurement.
    // I just want a "it works" wcwdith in all platforms.
    // So these tables are coming from the table with lowest UNICODE version
    // (original package provides different tables for different UNICODE versions).

    // clang-format off
    static const BoundaryVector ZERO_WIDTH{
        {U'\x00000', U'\x00000'}, // (nil)
        {U'\x000ad', U'\x000ad'}, // Soft Hyphen
        {U'\x00300', U'\x0036f'}, // Combining Grave Accent  ..Combining Latin Small Le
        {U'\x00483', U'\x00486'}, // Combining Cyrillic Titlo..Combining Cyrillic Psili
        {U'\x00488', U'\x00489'}, // Combining Cyrillic Hundr..Combining Cyrillic Milli
        {U'\x00591', U'\x005b9'}, // Hebrew Accent Etnahta   ..Hebrew Point Holam
        {U'\x005bb', U'\x005bd'}, // Hebrew Point Qubuts     ..Hebrew Point Meteg
        {U'\x005bf', U'\x005bf'}, // Hebrew Point Rafe
        {U'\x005c1', U'\x005c2'}, // Hebrew Point Shin Dot   ..Hebrew Point Sin Dot
        {U'\x005c4', U'\x005c5'}, // Hebrew Mark Upper Dot   ..Hebrew Mark Lower Dot
        {U'\x005c7', U'\x005c7'}, // Hebrew Point Qamats Qatan
        {U'\x00600', U'\x00603'}, // Arabic Number Sign      ..Arabic Sign Safha
        {U'\x00610', U'\x00615'}, // Arabic Sign Sallallahou ..Arabic Small High Tah
        {U'\x0064b', U'\x0065e'}, // Arabic Fathatan         ..Arabic Fatha With Two Do
        {U'\x00670', U'\x00670'}, // Arabic Letter Superscript Alef
        {U'\x006d6', U'\x006e4'}, // Arabic Small High Ligatu..Arabic Small High Madda
        {U'\x006e7', U'\x006e8'}, // Arabic Small High Yeh   ..Arabic Small High Noon
        {U'\x006ea', U'\x006ed'}, // Arabic Empty Centre Low ..Arabic Small Low Meem
        {U'\x0070f', U'\x0070f'}, // Syriac Abbreviation Mark
        {U'\x00711', U'\x00711'}, // Syriac Letter Superscript Alaph
        {U'\x00730', U'\x0074a'}, // Syriac Pthaha Above     ..Syriac Barrekh
        {U'\x007a6', U'\x007b0'}, // Thaana Abafili          ..Thaana Sukun
        {U'\x00901', U'\x00903'}, // Devanagari Sign Candrabi..Devanagari Sign Visarga
        {U'\x0093c', U'\x0093c'}, // Devanagari Sign Nukta
        {U'\x0093e', U'\x0094d'}, // Devanagari Vowel Sign Aa..Devanagari Sign Virama
        {U'\x00951', U'\x00954'}, // Devanagari Stress Sign U..Devanagari Acute Accent
        {U'\x00962', U'\x00963'}, // Devanagari Vowel Sign Vo..Devanagari Vowel Sign Vo
        {U'\x00981', U'\x00983'}, // Bengali Sign Candrabindu..Bengali Sign Visarga
        {U'\x009bc', U'\x009bc'}, // Bengali Sign Nukta
        {U'\x009be', U'\x009c4'}, // Bengali Vowel Sign Aa   ..Bengali Vowel Sign Vocal
        {U'\x009c7', U'\x009c8'}, // Bengali Vowel Sign E    ..Bengali Vowel Sign Ai
        {U'\x009cb', U'\x009cd'}, // Bengali Vowel Sign O    ..Bengali Sign Virama
        {U'\x009d7', U'\x009d7'}, // Bengali Au Length Mark
        {U'\x009e2', U'\x009e3'}, // Bengali Vowel Sign Vocal..Bengali Vowel Sign Vocal
        {U'\x00a01', U'\x00a03'}, // Gurmukhi Sign Adak Bindi..Gurmukhi Sign Visarga
        {U'\x00a3c', U'\x00a3c'}, // Gurmukhi Sign Nukta
        {U'\x00a3e', U'\x00a42'}, // Gurmukhi Vowel Sign Aa  ..Gurmukhi Vowel Sign Uu
        {U'\x00a47', U'\x00a48'}, // Gurmukhi Vowel Sign Ee  ..Gurmukhi Vowel Sign Ai
        {U'\x00a4b', U'\x00a4d'}, // Gurmukhi Vowel Sign Oo  ..Gurmukhi Sign Virama
        {U'\x00a70', U'\x00a71'}, // Gurmukhi Tippi          ..Gurmukhi Addak
        {U'\x00a81', U'\x00a83'}, // Gujarati Sign Candrabind..Gujarati Sign Visarga
        {U'\x00abc', U'\x00abc'}, // Gujarati Sign Nukta
        {U'\x00abe', U'\x00ac5'}, // Gujarati Vowel Sign Aa  ..Gujarati Vowel Sign Cand
        {U'\x00ac7', U'\x00ac9'}, // Gujarati Vowel Sign E   ..Gujarati Vowel Sign Cand
        {U'\x00acb', U'\x00acd'}, // Gujarati Vowel Sign O   ..Gujarati Sign Virama
        {U'\x00ae2', U'\x00ae3'}, // Gujarati Vowel Sign Voca..Gujarati Vowel Sign Voca
        {U'\x00b01', U'\x00b03'}, // Oriya Sign Candrabindu  ..Oriya Sign Visarga
        {U'\x00b3c', U'\x00b3c'}, // Oriya Sign Nukta
        {U'\x00b3e', U'\x00b43'}, // Oriya Vowel Sign Aa     ..Oriya Vowel Sign Vocalic
        {U'\x00b47', U'\x00b48'}, // Oriya Vowel Sign E      ..Oriya Vowel Sign Ai
        {U'\x00b4b', U'\x00b4d'}, // Oriya Vowel Sign O      ..Oriya Sign Virama
        {U'\x00b56', U'\x00b57'}, // Oriya Ai Length Mark    ..Oriya Au Length Mark
        {U'\x00b82', U'\x00b82'}, // Tamil Sign Anusvara
        {U'\x00bbe', U'\x00bc2'}, // Tamil Vowel Sign Aa     ..Tamil Vowel Sign Uu
        {U'\x00bc6', U'\x00bc8'}, // Tamil Vowel Sign E      ..Tamil Vowel Sign Ai
        {U'\x00bca', U'\x00bcd'}, // Tamil Vowel Sign O      ..Tamil Sign Virama
        {U'\x00bd7', U'\x00bd7'}, // Tamil Au Length Mark
        {U'\x00c01', U'\x00c03'}, // Telugu Sign Candrabindu ..Telugu Sign Visarga
        {U'\x00c3e', U'\x00c44'}, // Telugu Vowel Sign Aa    ..Telugu Vowel Sign Vocali
        {U'\x00c46', U'\x00c48'}, // Telugu Vowel Sign E     ..Telugu Vowel Sign Ai
        {U'\x00c4a', U'\x00c4d'}, // Telugu Vowel Sign O     ..Telugu Sign Virama
        {U'\x00c55', U'\x00c56'}, // Telugu Length Mark      ..Telugu Ai Length Mark
        {U'\x00c82', U'\x00c83'}, // Kannada Sign Anusvara   ..Kannada Sign Visarga
        {U'\x00cbc', U'\x00cbc'}, // Kannada Sign Nukta
        {U'\x00cbe', U'\x00cc4'}, // Kannada Vowel Sign Aa   ..Kannada Vowel Sign Vocal
        {U'\x00cc6', U'\x00cc8'}, // Kannada Vowel Sign E    ..Kannada Vowel Sign Ai
        {U'\x00cca', U'\x00ccd'}, // Kannada Vowel Sign O    ..Kannada Sign Virama
        {U'\x00cd5', U'\x00cd6'}, // Kannada Length Mark     ..Kannada Ai Length Mark
        {U'\x00d02', U'\x00d03'}, // Malayalam Sign Anusvara ..Malayalam Sign Visarga
        {U'\x00d3e', U'\x00d43'}, // Malayalam Vowel Sign Aa ..Malayalam Vowel Sign Voc
        {U'\x00d46', U'\x00d48'}, // Malayalam Vowel Sign E  ..Malayalam Vowel Sign Ai
        {U'\x00d4a', U'\x00d4d'}, // Malayalam Vowel Sign O  ..Malayalam Sign Virama
        {U'\x00d57', U'\x00d57'}, // Malayalam Au Length Mark
        {U'\x00d82', U'\x00d83'}, // Sinhala Sign Anusvaraya ..Sinhala Sign Visargaya
        {U'\x00dca', U'\x00dca'}, // Sinhala Sign Al-lakuna
        {U'\x00dcf', U'\x00dd4'}, // Sinhala Vowel Sign Aela-..Sinhala Vowel Sign Ketti
        {U'\x00dd6', U'\x00dd6'}, // Sinhala Vowel Sign Diga Paa-pilla
        {U'\x00dd8', U'\x00ddf'}, // Sinhala Vowel Sign Gaett..Sinhala Vowel Sign Gayan
        {U'\x00df2', U'\x00df3'}, // Sinhala Vowel Sign Diga ..Sinhala Vowel Sign Diga
        {U'\x00e31', U'\x00e31'}, // Thai Character Mai Han-akat
        {U'\x00e34', U'\x00e3a'}, // Thai Character Sara I   ..Thai Character Phinthu
        {U'\x00e47', U'\x00e4e'}, // Thai Character Maitaikhu..Thai Character Yamakkan
        {U'\x00eb1', U'\x00eb1'}, // Lao Vowel Sign Mai Kan
        {U'\x00eb4', U'\x00eb9'}, // Lao Vowel Sign I        ..Lao Vowel Sign Uu
        {U'\x00ebb', U'\x00ebc'}, // Lao Vowel Sign Mai Kon  ..Lao Semivowel Sign Lo
        {U'\x00ec8', U'\x00ecd'}, // Lao Tone Mai Ek         ..Lao Niggahita
        {U'\x00f18', U'\x00f19'}, // Tibetan Astrological Sig..Tibetan Astrological Sig
        {U'\x00f35', U'\x00f35'}, // Tibetan Mark Ngas Bzung Nyi Zla
        {U'\x00f37', U'\x00f37'}, // Tibetan Mark Ngas Bzung Sgor Rtags
        {U'\x00f39', U'\x00f39'}, // Tibetan Mark Tsa -phru
        {U'\x00f3e', U'\x00f3f'}, // Tibetan Sign Yar Tshes  ..Tibetan Sign Mar Tshes
        {U'\x00f71', U'\x00f84'}, // Tibetan Vowel Sign Aa   ..Tibetan Mark Halanta
        {U'\x00f86', U'\x00f87'}, // Tibetan Sign Lci Rtags  ..Tibetan Sign Yang Rtags
        {U'\x00f90', U'\x00f97'}, // Tibetan Subjoined Letter..Tibetan Subjoined Letter
        {U'\x00f99', U'\x00fbc'}, // Tibetan Subjoined Letter..Tibetan Subjoined Letter
        {U'\x00fc6', U'\x00fc6'}, // Tibetan Symbol Padma Gdan
        {U'\x0102c', U'\x01032'}, // Myanmar Vowel Sign Aa   ..Myanmar Vowel Sign Ai
        {U'\x01036', U'\x01039'}, // Myanmar Sign Anusvara   ..Myanmar Sign Virama
        {U'\x01056', U'\x01059'}, // Myanmar Vowel Sign Vocal..Myanmar Vowel Sign Vocal
        {U'\x01160', U'\x011ff'}, // Hangul Jungseong Filler ..Hangul Jongseong Ssangni
        {U'\x0135f', U'\x0135f'}, // Ethiopic Combining Gemination Mark
        {U'\x01712', U'\x01714'}, // Tagalog Vowel Sign I    ..Tagalog Sign Virama
        {U'\x01732', U'\x01734'}, // Hanunoo Vowel Sign I    ..Hanunoo Sign Pamudpod
        {U'\x01752', U'\x01753'}, // Buhid Vowel Sign I      ..Buhid Vowel Sign U
        {U'\x01772', U'\x01773'}, // Tagbanwa Vowel Sign I   ..Tagbanwa Vowel Sign U
        {U'\x017b4', U'\x017d3'}, // Khmer Vowel Inherent Aq ..Khmer Sign Bathamasat
        {U'\x017dd', U'\x017dd'}, // Khmer Sign Atthacan
        {U'\x0180b', U'\x0180d'}, // Mongolian Free Variation..Mongolian Free Variation
        {U'\x018a9', U'\x018a9'}, // Mongolian Letter Ali Gali Dagalga
        {U'\x01920', U'\x0192b'}, // Limbu Vowel Sign A      ..Limbu Subjoined Letter W
        {U'\x01930', U'\x0193b'}, // Limbu Small Letter Ka   ..Limbu Sign Sa-i
        {U'\x019b0', U'\x019c0'}, // New Tai Lue Vowel Sign V..New Tai Lue Vowel Sign I
        {U'\x019c8', U'\x019c9'}, // New Tai Lue Tone Mark-1 ..New Tai Lue Tone Mark-2
        {U'\x01a17', U'\x01a1b'}, // Buginese Vowel Sign I   ..Buginese Vowel Sign Ae
        {U'\x01dc0', U'\x01dc3'}, // Combining Dotted Grave A..Combining Suspension Mar
        {U'\x0200b', U'\x0200f'}, // Zero Width Space        ..Right-to-left Mark
        {U'\x02028', U'\x0202e'}, // Line Separator          ..Right-to-left Override
        {U'\x02060', U'\x02063'}, // Word Joiner             ..Invisible Separator
        {U'\x0206a', U'\x0206f'}, // Inhibit Symmetric Swappi..Nominal Digit Shapes
        {U'\x020d0', U'\x020eb'}, // Combining Left Harpoon A..Combining Long Double So
        {U'\x0302a', U'\x0302f'}, // Ideographic Level Tone M..Hangul Double Dot Tone M
        {U'\x03099', U'\x0309a'}, // Combining Katakana-hirag..Combining Katakana-hirag
        {U'\x0a802', U'\x0a802'}, // Syloti Nagri Sign Dvisvara
        {U'\x0a806', U'\x0a806'}, // Syloti Nagri Sign Hasanta
        {U'\x0a80b', U'\x0a80b'}, // Syloti Nagri Sign Anusvara
        {U'\x0a823', U'\x0a827'}, // Syloti Nagri Vowel Sign ..Syloti Nagri Vowel Sign
        {U'\x0d7b0', U'\x0d7ff'}, // Hangul Jungseong O-yeo  ..(nil)
        {U'\x0fb1e', U'\x0fb1e'}, // Hebrew Point Judeo-spanish Varika
        {U'\x0fe00', U'\x0fe0f'}, // Variation Selector-1    ..Variation Selector-16
        {U'\x0fe20', U'\x0fe23'}, // Combining Ligature Left ..Combining Double Tilde R
        {U'\x0feff', U'\x0feff'}, // Zero Width No-break Space
        {U'\x0fff9', U'\x0fffb'}, // Interlinear Annotation A..Interlinear Annotation T
        {U'\x10a01', U'\x10a03'}, // Kharoshthi Vowel Sign I ..Kharoshthi Vowel Sign Vo
        {U'\x10a05', U'\x10a06'}, // Kharoshthi Vowel Sign E ..Kharoshthi Vowel Sign O
        {U'\x10a0c', U'\x10a0f'}, // Kharoshthi Vowel Length ..Kharoshthi Sign Visarga
        {U'\x10a38', U'\x10a3a'}, // Kharoshthi Sign Bar Abov..Kharoshthi Sign Dot Belo
        {U'\x10a3f', U'\x10a3f'}, // Kharoshthi Virama
        {U'\x1d165', U'\x1d169'}, // Musical Symbol Combining..Musical Symbol Combining
        {U'\x1d16d', U'\x1d182'}, // Musical Symbol Combining..Musical Symbol Combining
        {U'\x1d185', U'\x1d18b'}, // Musical Symbol Combining..Musical Symbol Combining
        {U'\x1d1aa', U'\x1d1ad'}, // Musical Symbol Combining..Musical Symbol Combining
        {U'\x1d242', U'\x1d244'}, // Combining Greek Musical ..Combining Greek Musical
        {U'\xe0001', U'\xe0001'}, // Language Tag
        {U'\xe0020', U'\xe007f'}, // Tag Space               ..Cancel Tag
        {U'\xe0100', U'\xe01ef'}, // Variation Selector-17   ..Variation Selector-256
    };

    static const BoundaryVector WIDE_EAST_ASIAN{
        {U'\x01100', U'\x01159'}, // Hangul Choseong Kiyeok  ..Hangul Choseong Yeorinhi
        {U'\x0115f', U'\x0115f'}, // Hangul Choseong Filler
        {U'\x02329', U'\x0232a'}, // Left-pointing Angle Brac..Right-pointing Angle Bra
        {U'\x02e80', U'\x02e99'}, // Cjk Radical Repeat      ..Cjk Radical Rap
        {U'\x02e9b', U'\x02ef3'}, // Cjk Radical Choke       ..Cjk Radical C-simplified
        {U'\x02f00', U'\x02fd5'}, // Kangxi Radical One      ..Kangxi Radical Flute
        {U'\x02ff0', U'\x02ffb'}, // Ideographic Description ..Ideographic Description
        {U'\x03000', U'\x03029'}, // Ideographic Space       ..Hangzhou Numeral Nine
        {U'\x03030', U'\x0303e'}, // Wavy Dash               ..Ideographic Variation In
        {U'\x03041', U'\x03096'}, // Hiragana Letter Small A ..Hiragana Letter Small Ke
        {U'\x0309b', U'\x030ff'}, // Katakana-hiragana Voiced..Katakana Digraph Koto
        {U'\x03105', U'\x0312c'}, // Bopomofo Letter B       ..Bopomofo Letter Gn
        {U'\x03131', U'\x0318e'}, // Hangul Letter Kiyeok    ..Hangul Letter Araeae
        {U'\x03190', U'\x031b7'}, // Ideographic Annotation L..Bopomofo Final Letter H
        {U'\x031c0', U'\x031cf'}, // Cjk Stroke T            ..Cjk Stroke N
        {U'\x031f0', U'\x0321e'}, // Katakana Letter Small Ku..Parenthesized Korean Cha
        {U'\x03220', U'\x03243'}, // Parenthesized Ideograph ..Parenthesized Ideograph
        {U'\x03250', U'\x032fe'}, // Partnership Sign        ..Circled Katakana Wo
        {U'\x03300', U'\x04db5'}, // Square Apaato           ..Cjk Unified Ideograph-4d
        {U'\x04e00', U'\x09fbb'}, // Cjk Unified Ideograph-4e..Cjk Unified Ideograph-9f
        {U'\x0a000', U'\x0a48c'}, // Yi Syllable It          ..Yi Syllable Yyr
        {U'\x0a490', U'\x0a4c6'}, // Yi Radical Qot          ..Yi Radical Ke
        {U'\x0ac00', U'\x0d7a3'}, // Hangul Syllable Ga      ..Hangul Syllable Hih
        {U'\x0f900', U'\x0fa2d'}, // Cjk Compatibility Ideogr..Cjk Compatibility Ideogr
        {U'\x0fa30', U'\x0fa6a'}, // Cjk Compatibility Ideogr..Cjk Compatibility Ideogr
        {U'\x0fa70', U'\x0fad9'}, // Cjk Compatibility Ideogr..Cjk Compatibility Ideogr
        {U'\x0fe10', U'\x0fe19'}, // Presentation Form For Ve..Presentation Form For Ve
        {U'\x0fe30', U'\x0fe52'}, // Presentation Form For Ve..Small Full Stop
        {U'\x0fe54', U'\x0fe66'}, // Small Semicolon         ..Small Equals Sign
        {U'\x0fe68', U'\x0fe6b'}, // Small Reverse Solidus   ..Small Commercial At
        {U'\x0ff01', U'\x0ff60'}, // Fullwidth Exclamation Ma..Fullwidth Right White Pa
        {U'\x0ffe0', U'\x0ffe6'}, // Fullwidth Cent Sign     ..Fullwidth Won Sign
        {U'\x20000', U'\x2fffd'}, // Cjk Unified Ideograph-20..(nil)
        {U'\x30000', U'\x3fffd'}, // Cjk Unified Ideograph-30..(nil)
    };

    static const BoundaryVector VS16_NARROW_TO_WIDE{
        {U'\x00023', U'\x00023'}, // Number Sign
        {U'\x0002a', U'\x0002a'}, // Asterisk
        {U'\x00030', U'\x00039'}, // Digit Zero              ..Digit Nine
        {U'\x000a9', U'\x000a9'}, // Copyright Sign
        {U'\x000ae', U'\x000ae'}, // Registered Sign
        {U'\x0203c', U'\x0203c'}, // Double Exclamation Mark
        {U'\x02049', U'\x02049'}, // Exclamation Question Mark
        {U'\x02122', U'\x02122'}, // Trade Mark Sign
        {U'\x02139', U'\x02139'}, // Information Source
        {U'\x02194', U'\x02199'}, // Left Right Arrow        ..South West Arrow
        {U'\x021a9', U'\x021aa'}, // Leftwards Arrow With Hoo..Rightwards Arrow With Ho
        {U'\x02328', U'\x02328'}, // Keyboard
        {U'\x023cf', U'\x023cf'}, // Eject Symbol
        {U'\x023ed', U'\x023ef'}, // Black Right-pointing Dou..Black Right-pointing Tri
        {U'\x023f1', U'\x023f2'}, // Stopwatch               ..Timer Clock
        {U'\x023f8', U'\x023fa'}, // Double Vertical Bar     ..Black Circle For Record
        {U'\x024c2', U'\x024c2'}, // Circled Latin Capital Letter M
        {U'\x025aa', U'\x025ab'}, // Black Small Square      ..White Small Square
        {U'\x025b6', U'\x025b6'}, // Black Right-pointing Triangle
        {U'\x025c0', U'\x025c0'}, // Black Left-pointing Triangle
        {U'\x025fb', U'\x025fc'}, // White Medium Square     ..Black Medium Square
        {U'\x02600', U'\x02604'}, // Black Sun With Rays     ..Comet
        {U'\x0260e', U'\x0260e'}, // Black Telephone
        {U'\x02611', U'\x02611'}, // Ballot Box With Check
        {U'\x02618', U'\x02618'}, // Shamrock
        {U'\x0261d', U'\x0261d'}, // White Up Pointing Index
        {U'\x02620', U'\x02620'}, // Skull And Crossbones
        {U'\x02622', U'\x02623'}, // Radioactive Sign        ..Biohazard Sign
        {U'\x02626', U'\x02626'}, // Orthodox Cross
        {U'\x0262a', U'\x0262a'}, // Star And Crescent
        {U'\x0262e', U'\x0262f'}, // Peace Symbol            ..Yin Yang
        {U'\x02638', U'\x0263a'}, // Wheel Of Dharma         ..White Smiling Face
        {U'\x02640', U'\x02640'}, // Female Sign
        {U'\x02642', U'\x02642'}, // Male Sign
        {U'\x0265f', U'\x02660'}, // Black Chess Pawn        ..Black Spade Suit
        {U'\x02663', U'\x02663'}, // Black Club Suit
        {U'\x02665', U'\x02666'}, // Black Heart Suit        ..Black Diamond Suit
        {U'\x02668', U'\x02668'}, // Hot Springs
        {U'\x0267b', U'\x0267b'}, // Black Universal Recycling Symbol
        {U'\x0267e', U'\x0267e'}, // Permanent Paper Sign
        {U'\x02692', U'\x02692'}, // Hammer And Pick
        {U'\x02694', U'\x02697'}, // Crossed Swords          ..Alembic
        {U'\x02699', U'\x02699'}, // Gear
        {U'\x0269b', U'\x0269c'}, // Atom Symbol             ..Fleur-de-lis
        {U'\x026a0', U'\x026a0'}, // Warning Sign
        {U'\x026a7', U'\x026a7'}, // Male With Stroke And Male And Female Sign
        {U'\x026b0', U'\x026b1'}, // Coffin                  ..Funeral Urn
        {U'\x026c8', U'\x026c8'}, // Thunder Cloud And Rain
        {U'\x026cf', U'\x026cf'}, // Pick
        {U'\x026d1', U'\x026d1'}, // Helmet With White Cross
        {U'\x026d3', U'\x026d3'}, // Chains
        {U'\x026e9', U'\x026e9'}, // Shinto Shrine
        {U'\x026f0', U'\x026f1'}, // Mountain                ..Umbrella On Ground
        {U'\x026f4', U'\x026f4'}, // Ferry
        {U'\x026f7', U'\x026f9'}, // Skier                   ..Person With Ball
        {U'\x02702', U'\x02702'}, // Black Scissors
        {U'\x02708', U'\x02709'}, // Airplane                ..Envelope
        {U'\x0270c', U'\x0270d'}, // Victory Hand            ..Writing Hand
        {U'\x0270f', U'\x0270f'}, // Pencil
        {U'\x02712', U'\x02712'}, // Black Nib
        {U'\x02714', U'\x02714'}, // Heavy Check Mark
        {U'\x02716', U'\x02716'}, // Heavy Multiplication X
        {U'\x0271d', U'\x0271d'}, // Latin Cross
        {U'\x02721', U'\x02721'}, // Star Of David
        {U'\x02733', U'\x02734'}, // Eight Spoked Asterisk   ..Eight Pointed Black Star
        {U'\x02744', U'\x02744'}, // Snowflake
        {U'\x02747', U'\x02747'}, // Sparkle
        {U'\x02763', U'\x02764'}, // Heavy Heart Exclamation ..Heavy Black Heart
        {U'\x027a1', U'\x027a1'}, // Black Rightwards Arrow
        {U'\x02934', U'\x02935'}, // Arrow Pointing Rightward..Arrow Pointing Rightward
        {U'\x02b05', U'\x02b07'}, // Leftwards Black Arrow   ..Downwards Black Arrow
        {U'\x1f170', U'\x1f171'}, // Negative Squared Latin C..Negative Squared Latin C
        {U'\x1f17e', U'\x1f17f'}, // Negative Squared Latin C..Negative Squared Latin C
        {U'\x1f321', U'\x1f321'}, // Thermometer
        {U'\x1f324', U'\x1f32c'}, // White Sun With Small Clo..Wind Blowing Face
        {U'\x1f336', U'\x1f336'}, // Hot Pepper
        {U'\x1f37d', U'\x1f37d'}, // Fork And Knife With Plate
        {U'\x1f396', U'\x1f397'}, // Military Medal          ..Reminder Ribbon
        {U'\x1f399', U'\x1f39b'}, // Studio Microphone       ..Control Knobs
        {U'\x1f39e', U'\x1f39f'}, // Film Frames             ..Admission Tickets
        {U'\x1f3cb', U'\x1f3ce'}, // Weight Lifter           ..Racing Car
        {U'\x1f3d4', U'\x1f3df'}, // Snow Capped Mountain    ..Stadium
        {U'\x1f3f3', U'\x1f3f3'}, // Waving White Flag
        {U'\x1f3f5', U'\x1f3f5'}, // Rosette
        {U'\x1f3f7', U'\x1f3f7'}, // Label
        {U'\x1f43f', U'\x1f43f'}, // Chipmunk
        {U'\x1f441', U'\x1f441'}, // Eye
        {U'\x1f4fd', U'\x1f4fd'}, // Film Projector
        {U'\x1f549', U'\x1f54a'}, // Om Symbol               ..Dove Of Peace
        {U'\x1f56f', U'\x1f570'}, // Candle                  ..Mantelpiece Clock
        {U'\x1f573', U'\x1f579'}, // Hole                    ..Joystick
        {U'\x1f587', U'\x1f587'}, // Linked Paperclips
        {U'\x1f58a', U'\x1f58d'}, // Lower Left Ballpoint Pen..Lower Left Crayon
        {U'\x1f590', U'\x1f590'}, // Raised Hand With Fingers Splayed
        {U'\x1f5a5', U'\x1f5a5'}, // Desktop Computer
        {U'\x1f5a8', U'\x1f5a8'}, // Printer
        {U'\x1f5b1', U'\x1f5b2'}, // Three Button Mouse      ..Trackball
        {U'\x1f5bc', U'\x1f5bc'}, // Frame With Picture
        {U'\x1f5c2', U'\x1f5c4'}, // Card Index Dividers     ..File Cabinet
        {U'\x1f5d1', U'\x1f5d3'}, // Wastebasket             ..Spiral Calendar Pad
        {U'\x1f5dc', U'\x1f5de'}, // Compression             ..Rolled-up Newspaper
        {U'\x1f5e1', U'\x1f5e1'}, // Dagger Knife
        {U'\x1f5e3', U'\x1f5e3'}, // Speaking Head In Silhouette
        {U'\x1f5e8', U'\x1f5e8'}, // Left Speech Bubble
        {U'\x1f5ef', U'\x1f5ef'}, // Right Anger Bubble
        {U'\x1f5f3', U'\x1f5f3'}, // Ballot Box With Ballot
        {U'\x1f5fa', U'\x1f5fa'}, // World Map
        {U'\x1f6cb', U'\x1f6cb'}, // Couch And Lamp
        {U'\x1f6cd', U'\x1f6cf'}, // Shopping Bags           ..Bed
        {U'\x1f6e0', U'\x1f6e5'}, // Hammer And Wrench       ..Motor Boat
        {U'\x1f6e9', U'\x1f6e9'}, // Small Airplane
        {U'\x1f6f0', U'\x1f6f0'}, // Satellite
        {U'\x1f6f3', U'\x1f6f3'}, // Passenger Ship
    };
    // clang-format on

    static size_t bisearch(char32_t ucs, const BoundaryVector& table) {
        // TODO: Use STD algorithm to optimize this function

        // YYC MARK:
        // Do not change this "int" to "size_t" casually,
        // because the result of arithmetic operation may be negative.
        // Do not change this type before using new algorithm.
        int lbound = 0, ubound = table.size() - 1;

        if (ucs < table.front().first || ucs > table.back().second) return 0;

        while (ubound >= lbound) {
            int mid = (lbound + ubound) / 2;
            if (ucs > table[mid].second) lbound = mid + 1;
            else if (ucs < table[mid].first) ubound = mid - 1;
            else return 1;
        }

        return 0;
    }

    size_t wcwidth(char32_t wc) {
        // TODO: Add lru_cache(maxsize=1000) for this function

        // Small optimize for ASCII
        if (U'\x20' <= wc && wc < U'\x7F') [[likely]]
            return 1;

        // C0/C1 control char
        // NOTE: Not vanilla implementation. Return 0 instead of 1.
        if ((wc && wc < L'\x20') || (L'\x7F' <= wc && wc < L'\xA0')) return 0;

        // Zero-width char
        if (bisearch(wc, ZERO_WIDTH)) return 0;

        // Width 1 or 2
        return 1 + bisearch(wc, WIDE_EAST_ASIAN);
    }

    enum class WcswidthState {
        /// Normal character.
        Normal,
        /// Under ZWJ control char.
        /// Ignore the width of next char.
        ZeroWidthJoiner,
        /// Under ANSI Escape Sequence.
        /// Following chars should be treated as escape char.
        AnsiEscape,
        /// Under CSI control sequence, a part of ANSI Escape Sequence.
        /// No width was accumulated before terminal char.
        AnsiCsiEscape,
    };

    struct WcswidthContext {
        /// Current state.
        WcswidthState state;
        /// Tract the last computed char.
        /// It will be used for VS16 char.
        std::optional<char32_t> last_measured_char;
    };

    Result<size_t> wcswidth(const std::u32string_view& rhs) {
        WcswidthContext ctx{WcswidthState::Normal, std::nullopt};
        size_t width = 0;

        for (char32_t chr : rhs) {
            // Match char value
            switch (ctx.state) {
                case WcswidthState::Normal: {
                    switch (chr) {
                        case U'\x200D': {
                            // ZWJ control char
                            ctx.state = WcswidthState::ZeroWidthJoiner;
                            break;
                        }
                        case U'\xFE0F': {
                            // VS16 control char
                            // If we have a char which was acknowledged and has width,
                            // analyse it instead of this control char.
                            if (ctx.last_measured_char.has_value()) {
                                width += bisearch(ctx.last_measured_char.value(), VS16_NARROW_TO_WIDE);
                                ctx.last_measured_char = std::nullopt;
                            }
                            break;
                        }
                        case U'\x1B': {
                            // ANSI escape sequence
                            ctx.state = WcswidthState::AnsiEscape;
                            break;
                        }
                        default: {
                            // Fetch widht for normal char
                            int wcw = wcwidth(chr);
                            // Tract the final non-zero char for VS16 control char
                            if (wcw > 0) ctx.last_measured_char = wcw;
                            // Accumulate width
                            width += wcw;
                            break;
                        }
                    }
                    break;
                }
                case WcswidthState::ZeroWidthJoiner: {
                    // Eat this char and back to normal state.
                    // This is what ZWJ does.
                    ctx.state = WcswidthState::Normal;
                    break;
                }
                case WcswidthState::AnsiEscape: {
                    // Check the second char of escape sequence.
                    // If it is '[', we enter CSI state,
                    // otherwise we eat it and back to normal state.
                    // Additionally, there is a range requirement for this char (0x40-0x5F).
                    if (chr == U'[') {
                        ctx.state = WcswidthState::AnsiCsiEscape;
                    } else if (chr >= U'\x40' && chr <= U'\x5F') {
                        ctx.state = WcswidthState::Normal;
                    } else {
                        return std::unexpected(Error::BadAnsiEscSeq);
                    }
                    break;
                }
                case WcswidthState::AnsiCsiEscape: {
                    // CSI sequence is aonsisted by variable Parameter Char (count can be zero),
                    // at least one Middle Char and only one Final Char.
                    // So we eat all chars until we reach the terminal char.
                    if (chr >= U'\x40' && chr <= U'\x7E') {
                        // Final Char. Back to normal state.
                        ctx.state = WcswidthState::Normal;
                    } else if (chr >= U'\x30' && chr <= U'\x3F') {
                        ; // Parameter Char. Do nothing
                    } else if (chr >= U'\x20' && chr <= U'\x2F') {
                        ; // Middle Char. Do nothing
                    } else {
                        return std::unexpected(Error::BadCsiSeq);
                    }
                    break;
                }
            }
        }

        return width;
    }

    Result<size_t> wcswidth(const std::u8string_view& rhs) {
        // Cast encoding
        auto u32str = ENC::to_utf32(rhs);
        if (!u32str.has_value()) return std::unexpected(Error::BadEncoding);
        // Call underlying function
        return wcswidth(u32str.value());
    }

} // namespace yycc::carton::wcwidth
