INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/api.h \
    $$PWD/crypto_auth_hmacsha256.h \
    $$PWD/crypto_auth_hmacsha512256.h \
    $$PWD/crypto_box_curve25519xsalsa20poly1305.h \
    $$PWD/crypto_core_hsalsa20.h \
    $$PWD/crypto_core_salsa2012.h \
    $$PWD/crypto_core_salsa208.h \
    $$PWD/crypto_core_salsa20.h \
    $$PWD/crypto_hashblocks_sha256.h \
    $$PWD/crypto_hashblocks_sha512.h \
    $$PWD/crypto_hash_sha256.h \
    $$PWD/crypto_hash_sha512.h \
    $$PWD/crypto_onetimeauth_poly1305.h \
    $$PWD/crypto_scalarmult_curve25519.h \
    $$PWD/crypto_secretbox_xsalsa20poly1305.h \
    $$PWD/crypto_sign_edwards25519sha512batch.h \
    $$PWD/crypto_stream_salsa2012.h \
    $$PWD/crypto_stream_salsa208.h \
    $$PWD/crypto_stream_salsa20.h \
    $$PWD/crypto_stream_xsalsa20.h \
    $$PWD/crypto_uint32.h  \
    $$PWD/crypto_verify_16.h \
    $$PWD/crypto_verify_32.h \
    $$PWD/nacl.h \
    $$PWD/randombytes.h

HEADERS += $$PUBLIC_HEADERS

SOURCES += \
    $$PWD//crypto_auth_hmacsha256_ref/hmac_256.c \
    $$PWD/crypto_auth_hmacsha256_ref/verify_256.c \
    $$PWD/crypto_auth_hmacsha512256_ref/hmac_512.c \
    $$PWD/crypto_auth_hmacsha512256_ref/verify_512.c \
    $$PWD/crypto_box_curve25519xsalsa20poly1305_ref/after.c \
    $$PWD/crypto_box_curve25519xsalsa20poly1305_ref/before.c \
    $$PWD/crypto_box_curve25519xsalsa20poly1305_ref/box_b.c \
    $$PWD/crypto_box_curve25519xsalsa20poly1305_ref/keypair.c \
    $$PWD/crypto_core_hsalsa20_ref/core_h20r.c \
    $$PWD/crypto_core_salsa2012_ref/core_12r.c \
    $$PWD/crypto_core_salsa208_ref/core_8r.c \
    $$PWD/crypto_core_salsa20_ref/core_20r.c \
    $$PWD/crypto_hashblocks_sha256_ref/blocks_256.c \
    $$PWD/crypto_hashblocks_sha512_ref/blocks_512.c \
    $$PWD/crypto_hash_sha256_ref/hash_256.c \
    $$PWD/crypto_hash_sha512_ref/hash_512.c \
    $$PWD/crypto_onetimeauth_poly1305_ref/auth.c \
    $$PWD/crypto_onetimeauth_poly1305_ref/verify.c \
    $$PWD/crypto_scalarmult_curve25519_ref/base.c \
    $$PWD/crypto_scalarmult_curve25519_ref/smult.c \
    $$PWD/crypto_secretbox_xsalsa20poly1305_ref/box_sb.c \
    $$PWD/crypto_sign_edwards25519sha512batch_ref/fe25519.c \
    $$PWD/crypto_sign_edwards25519sha512batch_ref/ge25519.c \
    $$PWD/crypto_sign_edwards25519sha512batch_ref/sc25519.c \
    $$PWD/crypto_sign_edwards25519sha512batch_ref/sign.c \
    $$PWD/crypto_stream_salsa2012_ref/stream_12r.c \
    $$PWD/crypto_stream_salsa2012_ref/xor_12r.c \
    $$PWD/crypto_stream_salsa208_ref/stream_8r.c \
    $$PWD/crypto_stream_salsa208_ref/xor_8r.c \
    $$PWD/crypto_stream_salsa20_ref/stream_20r.c \
    $$PWD/crypto_stream_salsa20_ref/xor_20r.c \
    $$PWD/crypto_stream_xsalsa20_ref/stream_h20r.c \
    $$PWD/crypto_stream_xsalsa20_ref/xor_h20r.c \
    $$PWD/crypto_verify_16_ref/verify_16.c \
    $$PWD/crypto_verify_32_ref/verify_32.c
