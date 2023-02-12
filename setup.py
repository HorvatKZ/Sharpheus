import os
import sys
import shutil
import pathlib
import subprocess


PROGRAM_FILES = [ 'C:\\Program Files', 'C:\\Program Files (x86)' ]
VS_VERSIONS = [ '2022', '2019', '2017' ]
VS_EDITIONS = [ 'Community', 'Professional', 'Enterprise' ]


def find_msbuild():
    msbuild_path = os.getenv('MSBUILD_PATH')
    if msbuild_path is not None and os.path.exists(msbuild_path):
        return msbuild_path
    for program_files in PROGRAM_FILES:
        for vs_version in VS_VERSIONS:
            for vs_edition in VS_EDITIONS:
                msbuild_path = f'{program_files}\\Microsoft Visual Studio\\{vs_version}\\{vs_edition}\\MSBuild\\Current\\Bin\\MSBuild.exe'
                if os.path.exists(msbuild_path):
                    return msbuild_path
    print('WANRING: Could not locate MSBuild.exe')
    return input('Folder of MSBuild: ')


def get_vs_version():
    for program_files in PROGRAM_FILES:
        for vs_version in VS_VERSIONS:
            if os.path.exists(f'{program_files}\\Microsoft Visual Studio\\{vs_version}'):
                return vs_version
    return '2022'


def get_root_folder():
    return str(pathlib.Path(os.path.abspath(__file__)).parent.absolute())


def set_env_variable(var_name: str, value: str):
    subprocess.call([ 'set', f'{var_name}="{value}"' ], shell=True)
    subprocess.run([ 'setx', var_name, value ])


def copy_folder(src: str, dest: str):
    root_folder = get_root_folder()
    shutil.copytree(f'{root_folder}\\{src}', f'{root_folder}\\{dest}', dirs_exist_ok=True)


def gen_solution(premake_file_path: str):
    root_folder = get_root_folder()
    subprocess.run([ root_folder + '\\external\\Premake\\premake5.exe', f'--file={root_folder}\\{premake_file_path}', 'vs' + get_vs_version() ])


def build_solution(sln_path: str, config: str):
    msbuild_path = find_msbuild()
    root_folder = get_root_folder()
    subprocess.run([ msbuild_path, f'{root_folder}\\{sln_path}', f'/p:Configuration={config}'])

    
def main():
    print('Sharpheus Game Engine setup...')

    if sys.version_info.major != 3 or sys.version_info.minor < 10:
        print('ERROR: At least python 3.10 is required')
        exit(1)

    print('Setting environment variables...')
    set_env_variable('PY_DIR', str(pathlib.Path(sys.executable).parent.absolute()))
    msbuild_path = find_msbuild()
    print('MSBuild path:', msbuild_path)
    set_env_variable('MSBUILD_PATH', msbuild_path)

    print('Copying wxWidgets dlls...')
    copy_folder('external\\wxWidgets\\debug_dlls', 'bin\\Debug')
    copy_folder('external\\wxWidgets\\release_dlls', 'bin\\Release')
    copy_folder('external\\wxWidgets\\debug_dlls', 'Template\\bin\\Debug')
    copy_folder('external\\wxWidgets\\release_dlls', 'Template\\bin\\Release')

    print('Generation sln...')
    gen_solution('SharpheusStarter\\premake5.lua')

    print('Building SharpheusStarter...')
    build_solution('SharpheusStarter.sln', 'Release')

    print('Setup done')
    print('Now you can open bin/Release/SharpheusStarter.exe')
    print('Have fun using Sharpheus :)')


if __name__ == '__main__':
    main()
