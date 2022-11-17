@echo off
call ..\external\Premake\premake5.exe vs2022
if /i "%1%"=="-nopause" GOTO end
PAUSE
:end