TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    $$PWD/libnacl \
    $$PWD/libsqlite \
    $$PWD/libserval


symbian {
    # workaround to generate Smart Installer via Qt SDK
    installer.target = installer_sis
    QMAKE_EXTRA_TARGETS += installer
}
