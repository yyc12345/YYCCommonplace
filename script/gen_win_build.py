import argparse
import os
import io
import re

def validate_cpp_ver(ver: str) -> str:
    if re.match(r'^[0-9]+$', ver) is not None: return ver
    else: raise argparse.ArgumentTypeError('invalid version of C++ standard.')

def write_line(f: io.TextIOWrapper, val: str) -> None:
    f.write(val)
    f.write('\n')

# Reference: https://stackoverflow.com/questions/29213106/how-to-securely-escape-command-line-arguments-for-the-cmd-exe-shell-on-windows
def escape_argument(arg):
    if not arg or re.search(r'(["\s])', arg):
        arg = '"' + arg.replace('"', r'\"') + '"'
    return escape_for_cmd_exe(arg)
def escape_for_cmd_exe(arg):
    meta_re = re.compile(r'([()%!^"<>&|])')
    return meta_re.sub('^\1', arg)

class ScriptSettings:
    m_CppVersion: str
    m_NoDoc: bool

    def __init__(self, cpp_ver: str, no_doc: bool):
        self.m_CppVersion = cpp_ver
        self.m_NoDoc = no_doc

def script_head(f: io.TextIOWrapper, s: ScriptSettings) -> None:
    # change directory to root folder
    write_line(f, ':: Navigate to project root directory')
    root_dir: str = os.path.dirname(os.path.dirname(__file__))
    write_line(f, f'CD /d {escape_argument(root_dir)}')
    # create build directory and enter
    write_line(f, ':: Create build directory and enter it')
    write_line(f, 'MKDIR bin')
    write_line(f, 'CD bin')
    cpp_dir: str = f'cpp{s.m_CppVersion}'
    write_line(f, f'MKDIR {cpp_dir}')
    write_line(f, f'CD {cpp_dir}')
    # blank line
    write_line(f, '')

def script_tail(f: io.TextIOWrapper, s: ScriptSettings) -> None:
    # leave build directory and report success
    write_line(f, ':: Leave build directory and report')
    write_line(f, 'CD ..\\..')
    write_line(f, 'ECHO Windows CMake Build Done')

def create_directory(f: io.TextIOWrapper, s: ScriptSettings) -> None:
    # create build directory
    write_line(f, ':: Create internal build directory')
    write_line(f, 'MKDIR Win32')
    write_line(f, 'MKDIR x64')
    write_line(f, 'MKDIR documentation')
    # create install directory
    write_line(f, ':: Create internal install directory')
    write_line(f, 'MKDIR install')
    write_line(f, 'CD install')
    write_line(f, 'MKDIR Win32_Debug')
    write_line(f, 'MKDIR Win32_Release')
    write_line(f, 'MKDIR x64_Debug')
    write_line(f, 'MKDIR x64_Release')
    write_line(f, 'CD ..')
    # create msvc install directory
    write_line(f, ':: Create internal MSVC specific install directory')
    write_line(f, 'MKDIR msvc_install')
    write_line(f, 'CD msvc_install')
    write_line(f, 'MKDIR bin')
    write_line(f, 'MKDIR include')
    write_line(f, 'MKDIR lib')
    write_line(f, 'MKDIR share')
    write_line(f, 'CD bin')
    write_line(f, 'MKDIR Win32')
    write_line(f, 'MKDIR x64')
    write_line(f, 'CD ..')
    write_line(f, 'CD lib')
    write_line(f, 'MKDIR Win32\\Debug')
    write_line(f, 'MKDIR Win32\\Release')
    write_line(f, 'MKDIR x64\\Debug')
    write_line(f, 'MKDIR x64\\Release')
    write_line(f, 'CD ..')
    write_line(f, 'CD ..')
    # blank line
    write_line(f, '')

def cmake_build(f: io.TextIOWrapper, s: ScriptSettings) -> None:
    # build for Win32
    write_line(f, ':: Build for Win32')
    write_line(f, 'CD Win32')
    write_line(f, f'cmake -G "Visual Studio 16 2019" -A Win32 -DCMAKE_CXX_STANDARD={s.m_CppVersion} -DYYCC_BUILD_TESTBENCH=ON ../..')
    write_line(f, 'cmake --build . --config Debug')
    write_line(f, 'cmake --install . --prefix=../install/Win32_Debug --config Debug')
    write_line(f, 'cmake --build . --config Release')
    write_line(f, 'cmake --install . --prefix=../install/Win32_Release --config Release')
    write_line(f, 'CD ..')
    # build for x64
    write_line(f, ':: Build for x64')
    write_line(f, 'CD x64')
    write_line(f, f'cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_CXX_STANDARD={s.m_CppVersion} -DYYCC_BUILD_TESTBENCH=ON ../..')
    write_line(f, 'cmake --build . --config Debug')
    write_line(f, 'cmake --install . --prefix=../install/x64_Debug --config Debug')
    write_line(f, 'cmake --build . --config Release')
    write_line(f, 'cmake --install . --prefix=../install/x64_Release --config Release')
    write_line(f, 'CD ..')
    # build for documentation
    if not s.m_NoDoc:
        write_line(f, ':: Build for documentation')
        write_line(f, 'CD documentation')
        write_line(f, f'cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_CXX_STANDARD={s.m_CppVersion} -DYYCC_BUILD_DOC=ON ../..')
        write_line(f, 'cmake --build . --config Release')
        write_line(f, 'cmake --build . --target YYCCDocumentation')
        write_line(f, 'cmake --install . --prefix=../install/x64_Release --config Release')
        write_line(f, 'CD ..')
    # blank line
    write_line(f, '')

def msvc_build(f: io.TextIOWrapper, s: ScriptSettings) -> None:
    # copy include from x64_Release build
    write_line(f, ':: Copy header files')
    write_line(f, 'XCOPY install\\x64_Release\\include msvc_install\\include\\ /E /Y')
    # copy binary testbench
    write_line(f, ':: Copy binary files')
    write_line(f, 'COPY install\\Win32_Release\\bin\\YYCCTestbench.exe msvc_install\\bin\\Win32\\YYCCTestbench.exe /Y')
    write_line(f, 'COPY install\\x64_Release\\bin\\YYCCTestbench.exe msvc_install\\bin\\x64\\YYCCTestbench.exe /Y')
    # copy static library
    write_line(f, ':: Copy library files')
    write_line(f, 'COPY install\\Win32_Debug\\lib\\YYCCommonplace.lib msvc_install\\lib\\Win32\\Debug\\YYCCommonplace.lib /Y')
    write_line(f, 'COPY install\\Win32_Release\\lib\\YYCCommonplace.lib msvc_install\\lib\\Win32\\Release\\YYCCommonplace.lib /Y')
    write_line(f, 'COPY install\\x64_Debug\\lib\\YYCCommonplace.lib msvc_install\\lib\\x64\\Debug\\YYCCommonplace.lib /Y')
    write_line(f, 'COPY install\\x64_Release\\lib\\YYCCommonplace.lib msvc_install\\lib\\x64\\Release\\YYCCommonplace.lib /Y')
    # Copy document from x64_Release build
    if not s.m_NoDoc:
        write_line(f, ':: Copy documentation files')
        write_line(f, 'XCOPY install\\x64_Release\\share msvc_install\\share\\ /E /Y')
    # blank line
    write_line(f, '')

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
        '-d', '--no-doc',
        action='store_true', dest='no_doc',
        help='Build YYCC without documentation.'
    )
    args = parser.parse_args()

    # build settings
    settings = ScriptSettings(args.cpp, args.no_doc)
    # write result
    filepath = os.path.join(os.path.dirname(__file__), 'win_build.new.bat')
    with open(filepath, 'w') as f:
        script_head(f, settings)
        create_directory(f, settings)
        cmake_build(f, settings)
        msvc_build(f, settings)
        script_tail(f, settings)

