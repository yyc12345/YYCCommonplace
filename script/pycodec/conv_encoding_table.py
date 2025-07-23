import typing
from pathlib import Path
import os

class LanguageToken:
    name: str
    alias: tuple[str, ...]
    code_page: str | None
    iconv_code: str | None

    def __init__(self, name: str, alias: typing.Iterator[str], code_page: str, iconv_code: str):
        self.name = name.lower()
        self.alias = tuple(map(lambda x: x.lower(), alias))
        self.code_page = None if code_page == '' else code_page
        self.iconv_code = None if iconv_code == '' else iconv_code

def extract_data(fs: typing.TextIO) -> list[str]:
    # remove first line to remove table header
    return fs.readlines()[1:]

def extract_token(csv_data: list[str]) -> tuple[LanguageToken, ...]:
    ret: list[LanguageToken] = []
    for line in csv_data:
        line = line.strip('\n')
        line_sp = line.split('\t')
        alias_sp = filter(lambda x: len(x) != 0, map(lambda x: x.strip(), line_sp[1].split(',')))
        ret.append(LanguageToken(line_sp[0], alias_sp, line_sp[2], line_sp[3]))
    return tuple(ret)

def write_alias_map(fs: typing.TextIO, data: tuple[LanguageToken, ...]) -> None:
    fs.write('static const std::map<NS_YYCC_STRING::u8string, NS_YYCC_STRING::u8string> ALISA_MAP {\n')
    for i in data:
        for j in i.alias:
            fs.write(f'\t{{ YYCC_U8("{j}"), YYCC_U8("{i.name}") }},\n')
    fs.write('};\n')

def write_win_cp_map(fs: typing.TextIO, data: tuple[LanguageToken, ...]) -> None:
    fs.write('static const std::map<NS_YYCC_STRING::u8string, CodePage> WINCP_MAP {\n')
    for i in data:
        if i.code_page is not None:
            fs.write(f'\t{{ YYCC_U8("{i.name}"), static_cast<CodePage>({i.code_page}u) }},\n')
    fs.write('};\n')

def write_iconv_map(fs: typing.TextIO, data: tuple[LanguageToken, ...]) -> None:
    fs.write('static const std::map<NS_YYCC_STRING::u8string, std::string> ICONV_MAP {\n')
    for i in data:
        if i.iconv_code is not None:
            fs.write(f'\t{{ YYCC_U8("{i.name}"), "{i.iconv_code}" }},\n')
    fs.write('};\n')

if __name__ == '__main__':
    # get file path
    self_path = Path(__file__).resolve().parent
    csv_file = self_path / 'encoding_table.csv'
    cpp_file = self_path / 'encoding_table.cpp'
    # process files
    with open(csv_file, 'r', encoding='utf-8') as fr:
        with open(cpp_file, 'w', encoding='utf-8') as fw:
            data = extract_data(fr)
            token = extract_token(data)
            write_alias_map(fw, token)
            write_win_cp_map(fw, token)
            write_iconv_map(fw, token)
