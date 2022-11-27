import os
import sys
import shutil
import pathlib
import subprocess


PROGRAM_FILES = [ 'C:\\Program Files', 'C:\\Program Files (x86)' ]
VS_VERSIONS = [ '2022', '2019', '2017' ]
VS_EDITIONS = [ 'Community', 'Professional', 'Enterprise' ]

def find_msbuild():
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
    

print('Sharpheus Game Engine setup...')

if sys.version_info.major != 3 or sys.version_info.minor < 10:
    print('ERROR: At least python 3.10 is required')
    exit(1)

print('Setting environment variables...')
subprocess.call([ 'set', 'PY_DIR="' + str(pathlib.Path(sys.executable).parent.absolute()) + '"' ], shell=True)
subprocess.run([ 'setx', 'PY_DIR', str(pathlib.Path(sys.executable).parent.absolute()) ])
msbuild_path = find_msbuild()
print('MSBuild path:', msbuild_path)
subprocess.call([ 'set', 'MSBUILD_PATH=' + msbuild_path ], shell=True)
subprocess.run([ 'setx', 'MSBUILD_PATH', msbuild_path ])

print('Copying wxWidgets dlls...')
root_folder = str(pathlib.Path(os.path.abspath(__file__)).parent.absolute())
shutil.copytree(root_folder + '\\external\\wxWidgets\\debug_dlls', root_folder + '\\bin\\Debug', dirs_exist_ok=True)
shutil.copytree(root_folder + '\\external\\wxWidgets\\release_dlls', root_folder + '\\bin\\Release', dirs_exist_ok=True)
shutil.copytree(root_folder + '\\external\\wxWidgets\\debug_dlls', root_folder + '\\Template\\bin\\Debug', dirs_exist_ok=True)
shutil.copytree(root_folder + '\\external\\wxWidgets\\release_dlls', root_folder + '\\Template\\bin\\Release', dirs_exist_ok=True)

print('Generation sln...')
subprocess.run([ root_folder + '\\external\\Premake\\premake5.exe', f'--file={root_folder}\\SharpheusStarter\\premake5.lua', 'vs' + get_vs_version() ])

print('Building SharpheusStarter...')
subprocess.run([ msbuild_path, root_folder + '\\SharpheusStarter.sln', '/p:Configuration=Release'])

print('Setup done')
print('Now you can open bin/Release/SharpheusStarter.exe')
print('Have fun using Sharpheus :)')
