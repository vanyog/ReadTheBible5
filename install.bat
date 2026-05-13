set "INSTALL_DIR=C:\Program Files (x86)\VanyoG\ReadTheBibleFree"
set "RELEASE_DIR=C:\Users\Suny Cat\source\repos\ReadTheBible5\build\rel"

rmdir /s/q "%INSTALL_DIR%"
mkdir "%INSTALL_DIR%"

robocopy "%RELEASE_DIR%" "%INSTALL_DIR%" /E

exit /b 0