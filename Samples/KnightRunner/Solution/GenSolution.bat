@echo off
call "D:\Programming\Sharpheus\external\Premake\premake5.exe" --file="D:\Programming\Sharpheus\Samples\KnightRunner\Solution\premake5.lua" vs2022
if /i "%1%"=="-nopause" GOTO end
PAUSE
:end
