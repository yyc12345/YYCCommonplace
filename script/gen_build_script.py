import jinja2
import argparse
import os
import io
import re
import shlex

def validate_cpp_ver(ver: str) -> str:
    if re.match(r'^[0-9]+$', ver) is not None: return ver
    else: raise argparse.ArgumentTypeError('invalid version of C++ standard.')

def write_line(f: io.TextIOWrapper, val: str) -> None:
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

class ScriptSettings:
    m_CppVersion: str
    m_BuildDoc: bool
    m_PIC: bool

    def __init__(self, cpp_ver: str, build_doc: bool, pic: bool):
        self.m_CppVersion = cpp_ver
        self.m_BuildDoc = build_doc
        self.m_PIC = pic

class TemplateRender:
    m_Loader: jinja2.BaseLoader
    m_Environment: jinja2.Environment

    m_WinTemplate: jinja2.Template
    m_LinuxTemplate: jinja2.Template

    m_Settings: ScriptSettings

    def __init__(self, settings: ScriptSettings) -> None:
        self.m_Loader = jinja2.FileSystemLoader(self.__get_dir())
        self.m_Environment = jinja2.Environment(loader=self.m_Loader)

        self.m_WinTemplate = self.m_Environment.get_template('win_build.template.bat')
        self.m_LinuxTemplate = self.m_Environment.get_template('linux_build.template.sh')

        self.m_Settings = settings

    def __get_dir(self) -> str:
        return os.path.dirname(__file__)

    def __escape_path(self, val: str, is_win: bool) -> str:
        if is_win: return escape_cmd_argument(val)
        else: return escape_sh_argument(val)

    def __render(self, template: jinja2.Template, dest_file: str, is_win: bool) -> None:
        with open(os.path.join(self.__get_dir(), dest_file), 'w', encoding='utf-8') as f:
            f.write(template.render(
                repo_root_dir = self.__escape_path(os.path.dirname(self.__get_dir()), is_win),
                cpp_version = self.m_Settings.m_CppVersion,
                build_doc = self.m_Settings.m_BuildDoc,
                pic = settings.m_PIC
            ))

    def render_win_script(self) -> None:
        self.__render(self.m_WinTemplate, 'win_build.bat', True)
    
    def render_linux_script(self) -> None:
        self.__render(self.m_LinuxTemplate, 'linux_build.sh', False)
    

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
    

