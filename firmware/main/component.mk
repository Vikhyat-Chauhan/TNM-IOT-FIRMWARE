#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
COMPONENT_ADD_INCLUDEDIRS := . models/include mqtt/include wifi/include i2c/include system/include ntp/include json/include
COMPONENT_SRCDIRS := . models mqtt wifi i2c system ntp json

