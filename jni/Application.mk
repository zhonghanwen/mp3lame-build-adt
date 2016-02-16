APP_ABI := x86 x86_64 armeabi-v7a arm64-v8a armeabi mips64 mips
APP_PLATFORM := android-14
APP_STL += gnustl_static
APP_CPPFLAGS += -fexceptions
APP_CFLAGS += -DSTDC_HEADERS