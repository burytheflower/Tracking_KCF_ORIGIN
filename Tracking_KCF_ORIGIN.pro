TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    TrackerKCF/fhog.cpp \
    main.cpp \
    TrackerKCF/trackerKCF.cpp

INCLUDEPATH += \
        D:/Lib/OpenCV-4.0.1/include

win32 {
#.dll路径加到Path环境变量中
#编译出来的程序其实是64位的
    CONFIG(debug, debug|release): {
    LIBS += \
            -LD:/Lib/OpenCV-4.0.1/x64/vc14/lib \
            -lopencv_world401d
    }
    CONFIG(release, debug|release): {
    LIBS += \
            -LD:/Lib/OpenCV-4.0.1/x64/vc14/lib \
            -lopencv_world401
    }
}

HEADERS += \
    TrackerKCF/ffttools.hpp \
    TrackerKCF/fhog.hpp \
    TrackerKCF/recttools.hpp \
    TrackerKCF/trackerKCF.hpp

DISTFILES += \
    README.txt
