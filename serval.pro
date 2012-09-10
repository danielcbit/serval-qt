TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    $$PWD/3rdparty \
    $$PWD/src

# workarounds to generate Smart Installer via Qt SDK
symbian {
    coreinstaller.target = sub-c--weather-qml-src-installer_sis_target
    coreinstaller.commands = cd src && $(MAKE) installer_sis

    installer.target = installer_sis
    installer.depends = sub-c--weather-qml-src-installer_sis_target

    QMAKE_EXTRA_TARGETS += coreinstaller installer

} else:unix {
    include(data/data.pri)
}

OTHER_FILES += \
    $$PWD/qml/main.qml \
    $$PWD/qml/MainPage.qml
