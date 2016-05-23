QT += network widgets


HEADERS       = client.h \
    include/os/blccore_net.h \
    include/os/blccore_os.h \
    include/plug/bbcv_plug_keymap.h \
    include/plug/bbcv_plugapi.h \
    include/plug/ipanel_key.h \
    include/blccore.h \
    include/blctype.h \
    enum.h \
    include/eterm_porting.h \
    src_bbc/plug_comm.h \
    src_bbc/cJSON.h
SOURCES       = client.cpp \
                main.cpp \
    samples/os/netapi.c \
    samples/os/osapi.c \
    samples/plug/bbcv_plugapi.c \
    src_bbc/plug_key_convert.c \
    src_bbc/plug_main.c \
    src_bbc/plug_comm.c \
    src_bbc/cJSON.c

#INCLUDEPATH += C:\Users\jon\Dropbox\work\fortuneclient\
#LIBS += -lvlc -lvlccore(可能系统目录时ok)

#LIBS += C:\Users\jon\Documents\dev\term\CTS\StbSimulator\v0.1.2.2\libvlc.dll \
       # C:\Users\jon\Documents\dev\term\CTS\StbSimulator\v0.1.2.2\libvlccore.dll
LIBS += .\libvlc.dll \
        .\libvlccore.dll

win32:LIBS += -lsetupapi -lwsock32 #-lws2_32  #-lsetupapi

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target
