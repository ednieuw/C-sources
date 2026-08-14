
echo off
if "%1"=="" GOTO :error
pic2prn %1
pe pic.prn
prn2pic pic.prn
drawpic pic.pic
GOTO :end
:error
echo Geef een filenaam.PIC op !!
goto :end
:end