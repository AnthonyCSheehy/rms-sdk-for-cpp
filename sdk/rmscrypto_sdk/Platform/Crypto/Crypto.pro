REPO_ROOT = $$PWD/../../../..
DESTDIR   = $$REPO_ROOT/bin/crypto/platform
TARGET    = platformcrypto

TEMPLATE  = lib

win32:INCLUDEPATH += $$REPO_ROOT/third_party/include
INCLUDEPATH += $$REPO_ROOT/sdk/rmscrypto_sdk/CryptoAPI

DEFINES  += QTFRAMEWORK

CONFIG += staticlib warn_on c++11 debug_and_release

QT += core
QT -= gui

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

HEADERS += CryptoEngine.h \
    CryptoHash.h \
    AESCryptoKey.h \
    RSAKeyBlob.h

SOURCES += \
    CryptoEngine.cpp \
    CryptoHash.cpp \
    AESCryptoKey.cpp \
    RSAKeyBlob.cpp
