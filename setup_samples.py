import os
import pathlib
import subprocess

import setup


def is_sample(root_folder: str, sample: str):
    return os.path.isdir(f'{root_folder}\\Samples\\{sample}') and os.path.exists(f'{root_folder}\\Samples\\{sample}\\{sample}.proj.sharpheus')


def setup_sample(root_folder: str, sample: str):
    print('Generating sample', sample)
    setup.copy_folder('external\\wxWidgets\\debug_dlls', f'Samples\\{sample}\\bin\\Debug')
    setup.copy_folder('external\\wxWidgets\\release_dlls', f'Samples\\{sample}\\bin\\Release')
    setup.gen_solution(f'Samples\\{sample}\\Solution\\premake5.lua')
    setup.build_solution(f'Samples\\{sample}\\Solution\\{sample}.sln', 'Debug')


def main():
    root_folder = str(pathlib.Path(os.path.abspath(__file__)).parent.absolute())
    for dir in os.listdir(root_folder + '\\Samples'):
        if is_sample(root_folder, dir):
            setup_sample(root_folder, dir)


if __name__ == '__main__':
    main()
