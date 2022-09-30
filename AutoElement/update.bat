@echo off
if exist E:\VsPro\CerviceScript\AutoMemTest\use.zip goto st
echo "no zip file"
goto end
:st
copy /Y E:\VsPro\CerviceScript\AutoMemTest\use.zip .\zip.zip
D:\7-Zip\7z.exe -y x *.zip
echo "[+] File Name is "
(for /f "delims=" %%i in ('dir /b /s /a-d  *.zip') do (
    echo %%~ni
))

:end
pause
exit