PROJECTNAME = Serval

TEMPLATE = app
TARGET = serval
VERSION = 0.0.1

QT += network declarative
CONFIG += mobility
MOBILITY += systeminfo

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# For Nokia Store builds
# also uncomment the line symbian:DEPLOYMENT.installer_header when building
#UID3 = 200651B6

# For development use (self-signed)
UID3 = E63942F9

symbian:TARGET.UID3 = 0x$$UID3

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
}

symbian {
    CONFIG += qt-components

    LIBS += \
	-lesock \
	-lconnmon \
	-lcone \
	-lavkon

    TARGET.UID3 = 0xE5C40926
    TARGET.EPOCHEAPSIZE = 0x20000 0x8000000
    TARGET.EPOCSTACKSIZE = 0x14000

    symbian-abld|symbian-sbsv2 {
	# ro-section in gui can exceed default allocated space,
	# so move rw-section a little further
	QMAKE_LFLAGS.ARMCC += --rw-base 0x800000
	QMAKE_LFLAGS.GCCE += -Tdata 0xC00000
    }

    ICON += ../data/images/serval.svg

    #RESOURCES += \
	#../data/images/640x360/resource.qrc \
	#../qml/symbian/symbian.qrc
} else {

    #RESOURCES += \
	#../data/images/800x480/resource.qrc \
	#../qml/harmattan/harmattan.qrc
}

RESOURCES += \
    ../qml/qml.qrc

contains(MEEGO_EDITION,harmattan): DEFINES += MEEGO_EDITION_HARMATTAN

unix:!symbian {
    isEmpty(PREFIX) {
	contains(MEEGO_EDITION,harmattan) {
	    PREFIX = /opt/serval
	} else {
	    PREFIX = /usr/local
	}
    }

    target.path = $$PREFIX/bin
    INSTALLS += target
}

HEADERS += \
    mainwindow.h

SOURCES += \
    $$PWD/main.cpp \
    mainwindow.cpp
