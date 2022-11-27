##############################################################
# Script to refresh wxWidgets binaries from wxWidgets_src
# wxWidgets_src is not uploaded to git, but can be downloaded from https://www.wxwidgets.org/downloads/
##############################################################

import os
import shutil
import pathlib


def is_debug(name: str) -> bool:
    return name.split('_')[0][-2:] == 'ud'


target_folder = str(pathlib.Path(os.path.abspath(__file__)).parent.absolute())
source_folder = str(pathlib.Path(target_folder).parent.absolute()) + '\\wxWidgets_src'

print('Removing old wxWidgets binaries...')

shutil.rmtree(target_folder + '\\include', ignore_errors=True)
shutil.rmtree(target_folder + '\\lib', ignore_errors=True)
shutil.rmtree(target_folder + '\\debug_dlls', ignore_errors=True)
shutil.rmtree(target_folder + '\\release_dlls', ignore_errors=True)

print('Copying wxWidgets binaries from wxWidgets_src...')

shutil.copytree(source_folder + '\\include', target_folder + '\\include', dirs_exist_ok=True)
shutil.copytree(source_folder + '\\lib\\vc_x64_dll\\mswu', target_folder + '\\lib\\vc_x64_dll\\mswu', dirs_exist_ok=True)
shutil.copytree(source_folder + '\\lib\\vc_x64_dll\\mswud', target_folder + '\\lib\\vc_x64_dll\\mswud', dirs_exist_ok=True)

os.mkdir(target_folder + '\\debug_dlls')
os.mkdir(target_folder + '\\release_dlls')

for entry in os.listdir(source_folder + '\\lib\\vc_x64_dll'):
    if not os.path.isfile(source_folder + '\\lib\\vc_x64_dll\\' + entry):
        continue
    ext = entry.split('.')[-1]
    if ext == 'dll':
        if is_debug(entry):
            shutil.copyfile(source_folder + '\\lib\\vc_x64_dll\\' + entry, target_folder + '\\debug_dlls\\' + entry)
        else:
            shutil.copyfile(source_folder + '\\lib\\vc_x64_dll\\' + entry, target_folder + '\\release_dlls\\' + entry)
    elif ext == 'pdb' and is_debug(entry):
        shutil.copyfile(source_folder + '\\lib\\vc_x64_dll\\' + entry, target_folder + '\\debug_dlls\\' + entry)
    elif ext == 'lib' or ext == 'exp':
        shutil.copyfile(source_folder + '\\lib\\vc_x64_dll\\' + entry, target_folder + '\\lib\\vc_x64_dll\\' + entry) 

print('Copying finished')
