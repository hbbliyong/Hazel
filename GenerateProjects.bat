@echo off
cd /d ./scripts
call Win-GenProjects.bat
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)
