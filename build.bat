@echo off
where gcc
if %ERRORLEVEL%==1 goto no_gcc
gcc main.c -o ./out/main


:no_gcc
echo You need gcc.
pause
goto eof
