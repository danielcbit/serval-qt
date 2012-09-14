contains(MEEGO_EDITION, harmattan) {
    VERSION = 1.0.0
    TARGET = servald
    TEMPLATE = app

    QT -= core gui
    CONFIG += console
    DEFINES += Q_WS_MAEMO_6

    servald.path = /etc/init/apps
    servald.files = $$PWD/servald.conf

    target.path = /opt/serval/bin
    INSTALLS += target servald

    LIBS += \
	-L$$OUT_PWD/../3rdparty/lib \
	-lnacl \
	-lserval \
	-lsqlite3


    SOURCES += \
	$$PWD/main.c \

    INCLUDEPATH += \
	$$PWD/../3rdparty/libserval/src/ \
	$$PWD/../3rdparty/libnacl/src/

    DEFINES += _GNU_SOURCE \
	HAVE_SYS_MMAN_H=1 \
	HAVE_SYS_SOCKET_H=1 \
	HAVE_NETINET_IN_H=1 \
	HAVE_ARPA_INET_H=1 \
	HAVE_POLL_H=1
} else:symbian {
    # Hack to not generate a SIS package in Qt Creator
    TEMPLATE = subdirs
}
