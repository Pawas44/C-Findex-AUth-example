@echo off
cd /d "%~dp0"

git add .
git commit -m "Auto Update"
git push origin main

pause