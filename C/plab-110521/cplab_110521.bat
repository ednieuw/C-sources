SET PATH=PATH=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\ATI Technologies\ATI.ACE\Core-Static;c:\c\bin;c:\util
if exist error del error
    lib\tcc -w -c -C -mh -ff -a -d -G- -k- plab.c    >error
    lib\tcc -w -c -C -mh -ff -a -d -G- -k- plab2.c    >>error
    lib\tcc -w -c -C -mh -ff -a -d -G- -k- $wmf.C    >>error
    lib\tlink @plablink >>error
    SET PATH=C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Microsoft SQL Server\90\Tools\binn\;C:\Program Files\QuickTime\QTSystem\;C:\Program Files\Calibre2\
notepad error
