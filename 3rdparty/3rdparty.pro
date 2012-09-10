TEMPLATE = subdirs

SUBDIRS += \
    $$PWD/libnacl \
    $$PWD/libserval


symbian {
    # workaround to generate Smart Installer via Qt SDK
    installer.target = installer_sis
    QMAKE_EXTRA_TARGETS += installer
}
