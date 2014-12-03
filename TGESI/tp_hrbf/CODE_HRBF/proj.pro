
HEADERS += \
    src/fileloaders/*.h \
    src/rendersystem/*.h \
    src/rendersystem/hrbf/*.h \
    src/rendersystem/mcubes/*.h

INCLUDEPATH += \
            src/ \
            src/glm/ \
            lib_ext/include/

SOURCES += \
    src/fileloaders/*.cpp \
    src/rendersystem/*.cpp \
    src/rendersystem/hrbf/*.cpp \
    src/rendersystem/mcubes/*.cpp

OTHER_FILES += \
    shaders/vertexdefault.glsl \
    shaders/fragmentdefault.glsl \
    *.txt
