#
# Component Makefile
#

COMPONENT_ADD_INCLUDEDIRS := platform_include
COMPONENT_SRCDIRS := src

ifdef CONFIG_NEWLIB_NANO_FORMAT
LIBC := c_nano
else  # CONFIG_NEWLIB_NANO_FORMAT
LIBC := c
endif  # CONFIG_NEWLIB_NANO_FORMAT

# Order of linking matters: libnewlib.a should go before libc.a 
# New flags were added by Vikhyat to prevent issue in cJSON function cJSON_AddNumberToObject [https://github.com/DaveGamble/cJSON/issues/585#issuecomment-869781285]
COMPONENT_ADD_LDFLAGS :=-u_printf_float -u_scanf_float -lnewlib -l$(LIBC) -lm -u __errno
