TEMPLATE = lib
QT      -= gui
TARGET   = serval
DESTDIR  = ../lib
CONFIG += create_prl precompile_header staticlib

INCLUDEPATH += \
    $$PWD/../libnacl/src/

LIBS += \
    -lnacl \
    -lasound \
    -L$$DESTDIR \

symbian: {
  DEFINES += LIBSERVAL_MAKEDLL
  #export public header to \epocroot\epoc32\include to be able to use them
  headers.files = $$PUBLIC_HEADERS
  headers.path = $$PWD
  for(header, headers.files) {
    {BLD_INF_RULES.prj_exports += "$$header"}
  }

  TARGET.EPOCALLOWDLLDATA = 1
  # uid for the dll
  #TARGET.UID3=
  TARGET.CAPABILITY = ReadDeviceData WriteDeviceData

  # do not freeze api-> no libs produced. Comment when freezing!
  # run "abld freeze winscw" to create def files
  symbian:MMP_RULES += "EXPORTUNFROZEN"

  #use-3rdparty {
    CONFIG += staticlib

    # workaround to generate Smart Installer via Qt SDK
    installer.target = installer_sis
    QMAKE_EXTRA_TARGETS += installer

  #} else {
  #  # add dll to the sis
  #  QjsonDeployment.sources = $${TARGET}.dll
  #  QjsonDeployment.path = /sys/bin

  #  DEPLOYMENT += QjsonDeployment
  #}
} else:unix {
    DEFINES += _GNU_SOURCE \
    HAVE_SYS_MMAN_H=1 \
    HAVE_SYS_SOCKET_H=1 \
    HAVE_NETINET_IN_H=1 \
    HAVE_ARPA_INET_H=1 \
    HAVE_POLL_H=1 \
    HAVE_SYS_TIME_H=1 \
    HAVE_ALSA_ASOUNDLIB_H=1

    target.path = /opt/serval/lib
    INSTALLS += target
}

include(src/src.pri)
