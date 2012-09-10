TEMPLATE = lib
QT      -= gui
TARGET   = sqlite3
DESTDIR  = ../lib
CONFIG += create_prl precompile_header staticlib

symbian: {
  DEFINES += LIBSQLITE_MAKEDLL
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
    DEFINES += _GNU_SOURCE

    target.path = /opt/serval/lib
    INSTALLS += target
}

include(src/src.pri)
