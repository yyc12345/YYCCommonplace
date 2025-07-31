import argparse
import typing
import re
import shlex
from pathlib import Path
from dataclasses import dataclass
import jinja2

def validate_cpp_ver(ver: str) -> str:
    if re.match(r'^[0-9]+$', ver) is not None: return ver
    else: raise argparse.ArgumentTypeError('invalid version of C++ standard.')

def write_line(f: typing.TextIO, val: str) -> None:
    f.write(val)
    f.write('\n')

# Reference: https://stackoverflow.com/questions/29213106/how-to-securely-escape-command-line-arguments-for-the-cmd-exe-shell-on-windows
def escape_for_cmd_exe(arg):
    meta_re = re.compile(r'([()%!^"<>&|])')
    return meta_re.sub('^\1', arg)
def escape_cmd_argument(arg):
    if not arg or re.search(r'(["\s])', arg):
        arg = '"' + arg.replace('"', r'\"') + '"'
    return escape_for_cmd_exe(arg)
def escape_sh_argument(arg):
    return shlex.quote(arg)

@dataclass(frozen=True)
class ScriptSettings:
    cpp_version: str
    build_doc: bool
    pic: bool

class TemplateRender:
    loader: jinja2.BaseLoader
    environment: jinja2.Environment

    win_template: jinja2.Template
    linux_template: jinja2.Template

    settings: ScriptSettings

    def __init__(self, settings: ScriptSettings) -> None:
        self.loader = jinja2.FileSystemLoader(self.__get_dir())
        self.environment = jinja2.Environment(loader=self.loader)

        self.win_template = self.environment.get_template('win_build.bat.jinja')
        self.linux_template = self.environment.get_template('linux_build.sh.jinja')

        self.settings = settings

    def __get_dir(self) -> Path:
        return Path(__file__).resolve().parent

    def __escape_path(self, val: str, is_win: bool) -> str:
        if is_win: return escape_cmd_argument(val)
        else: return escape_sh_argument(val)

    def __render(self, template: jinja2.Template, dest_file: str, is_win: bool) -> None:
        with open(self.__get_dir() / dest_file, 'w', encoding='utf-8') as f:
            f.write(template.render(
                repo_root_dir = self.__escape_path(str(self.__get_dir().parent), is_win),
                cpp_version = self.settings.cpp_version,
                build_doc = self.settings.build_doc,
                pic = settings.pic
            ))

    def render_win_script(self) -> None:
        self.__render(self.win_template, 'win_build.bat', True)
    
    def render_linux_script(self) -> None:
        self.__render(self.linux_template, 'linux_build.sh', False)
    

if __name__ == '__main__':
    # parse argument
    parser = argparse.ArgumentParser(
        prog='YYCC Windows Build Script Generator',
        description='YYCC Windows Build Script Generator'
    )
    parser.add_argument(
        '-c', '--cpp',
        action='store', default='17', dest='cpp', type=validate_cpp_ver,
        help='The version of C++ standard used when building.'
    )
    parser.add_argument(
        '-d', '--build-doc',
        action='store_true', dest='build_doc',
        help='Build YYCC without documentation.'
    )
    parser.add_argument(
        '-p', '--pic',
        action='store_true', dest='pic',
        help='Enable Position Independent Code flag on non-Windows platform. This is crucial for compiling dynamic library using this library.'
    )
    args = parser.parse_args()

    # build settings
    settings = ScriptSettings(args.cpp, args.build_doc, args.pic)
    # build template render and render result
    render = TemplateRender(settings)
    render.render_win_script()
    render.render_linux_script()
    

