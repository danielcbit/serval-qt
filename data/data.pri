#include(images/images.pri)

desktop.files = $$PWD/serval.desktop
desktop.path = /usr/share/applications

icon.files = $$PWD/images/serval.png
icon.path = /usr/share/icons/hicolor/80x80/apps

INSTALLS += desktop icon
