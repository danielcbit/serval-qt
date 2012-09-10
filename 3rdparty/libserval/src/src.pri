INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/asterisk_include

PUBLIC_HEADERS += \
    $$PWD/strbuf.h \
    $$PWD/serval.h \
    $$PWD/str.h \
    $$PWD/fifo.h \
    $$PWD/log.h \
    $$PWD/monitor-client.h \
    $$PWD/net.h \
    $$PWD/sha2.h \
    $$PWD/xprintf.h \
    $$PWD/strlcpy.h \
    $$PWD/socket.h \
    $$PWD/rhizome.h \
    $$PWD/strbuf_helpers.h \
    $$PWD/constants.h\
    $$PWD/asterisk_include/asterisk/utils.h \
    $$PWD/asterisk_include/asterisk/lock.h \
    $$PWD/asterisk_include/asterisk/file.h \
    $$PWD/asterisk_include/asterisk/logger.h \
    $$PWD/asterisk_include/asterisk/channel.h \
    $$PWD/asterisk_include/asterisk/callerid.h \
    $$PWD/asterisk_include/asterisk/pbx.h \
    $$PWD/asterisk_include/asterisk/module.h \
    $$PWD/asterisk_include/asterisk/translate.h \
    $$PWD/asterisk_include/asterisk/features.h \
    $$PWD/asterisk_include/asterisk/options.h \
    $$PWD/asterisk_include/asterisk/cli.h \
    $$PWD/asterisk_include/asterisk/config.h \
    $$PWD/asterisk_include/asterisk/say.h \
    $$PWD/asterisk_include/asterisk/localtime.h \
    $$PWD/asterisk_include/asterisk/cdr.h \
    $$PWD/asterisk_include/asterisk/options.h \
    $$PWD/asterisk_include/asterisk.h


HEADERS += $$PUBLIC_HEADERS

SOURCES += \
    $$PWD/overlay_mdp.c \
    $$PWD/audio_reflector.c \
    $$PWD/server.c \
    $$PWD/monitor-cli.c \
    $$PWD/strbuf_helpers.c \
    $$PWD/conf.c \
    $$PWD/servalwrap.c \
    $$PWD/rhizome_packetformats.c \
    $$PWD/strbuf.c \
    $$PWD/overlay_payload.c \
    $$PWD/jni.c \
    $$PWD/rhizome_bundle.c \
    $$PWD/performance_timing.c \
    $$PWD/strlcpy.c \
    $$PWD/srandomdev.c \
    $$PWD/main.c \
    $$PWD/lsif.c \
    $$PWD/vomp.c \
    $$PWD/client.c \
    $$PWD/simulate.c \
    $$PWD/dna_identity.c \
    $$PWD/overlay_packetformats.c \
    $$PWD/batman.c \
    $$PWD/audio_msm_g1.c \
    $$PWD/audio_alsa.c \
    $$PWD/overlay_abbreviations.c \
    $$PWD/overlay_interface.c \
    $$PWD/keyring.c \
    $$PWD/fdqueue.c \
    $$PWD/ciphers.c \
    $$PWD/rhizome_database.c \
    $$PWD/peers.c \
    $$PWD/log.c \
    $$PWD/net.c \
    $$PWD/overlay_buffer.c \
    $$PWD/serval_packetvisualise.c \
    $$PWD/codecs.c \
    $$PWD/commandline.c \
    $$PWD/monitor-client.c \
    $$PWD/rhizome_fetch.c \
    $$PWD/socket.c \
    $$PWD/str.c \
    $$PWD/pa_phone.c \
    $$PWD/overlay_advertise.c \
    $$PWD/dataformats.c \
    $$PWD/packetformats.c \
    $$PWD/rhizome_crypto.c \
    $$PWD/overlay.c \
    $$PWD/xprintf.c \
    $$PWD/overlay_route.c \
    $$PWD/rhizome_http.c \
    $$PWD/mkdir.c \
    $$PWD/audiodevices.c \
    $$PWD/randombytes.c \
    $$PWD/responses.c \
    $$PWD/dna_helper.c \
    $$PWD/monitor.c \
    $$PWD/sha2.c \
    $$PWD/asterisk_app.c \
    $$PWD/dnawrap.c \
    $$PWD/gateway.c \
    $$PWD/overlay_broadcast.c \
    $$PWD/rhizome.c \
    $$PWD/fifo.c \
    $$PWD/testnacl.c \
    $$PWD/sighandlers.c
