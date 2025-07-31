import csv
from pathlib import Path
import jinja2


class LanguageToken:
    name: str
    alias: tuple[str, ...]
    code_page: str | None
    iconv_code: str | None

    def __init__(self, row: list[str]):
        """Init language token from CSV row."""
        self.name = row[0].lower()
        code_page = row[1]
        self.code_page = None if len(code_page) == 0 else code_page
        iconv_code = row[2]
        self.iconv_code = None if len(iconv_code) == 0 else iconv_code
        # For alias, we strip and to lower them first, and remove all empty entries
        alias = row[3:]
        self.alias = tuple(
            filter(lambda x: len(x) != 0,
                   map(lambda x: x.strip().lower(), alias)))


def _get_self_dir() -> Path:
    return Path(__file__).resolve().parent


def _extract_tokens() -> list[LanguageToken]:
    rv: list[LanguageToken] = []
    csv_file = _get_self_dir() / 'encoding_table.csv'

    with open(csv_file, 'r', encoding='utf-8', newline='') as f:
        reader = csv.reader(f, delimiter='\t')
        for row in reader:
            rv.append(LanguageToken(row))

    return rv


def _render_cpp(tokens: list[LanguageToken]) -> None:
    loader = jinja2.FileSystemLoader(_get_self_dir())
    environment = jinja2.Environment(loader=loader)
    template = environment.get_template('encoding_table.cpp.jinja')

    cpp_file = _get_self_dir() / 'encoding_table.cpp'
    with open(cpp_file, 'w', encoding='utf-8') as f:
        f.write(template.render(tokens=tokens))


if __name__ == '__main__':
    tokens = _extract_tokens()
    _render_cpp(tokens)
