#############################################################
# Required variables for each makefile
# Discard this section from all parent makefiles
# Expected variables (with automatic defaults):
#   CSRCS (all "C" files in the dir)
#   SUBDIRS (all subdirs with a Makefile)
#   GEN_LIBS - list of libs to be generated ()
#   GEN_IMAGES - list of object file images to be generated ()
#   GEN_BINS - list of binaries to be generated ()
#   COMPONENTS_xxx - a list of libs/objs in the form
#     subdir/lib to be extracted and rolled up into
#     a generated lib/image xxx.a ()
#
ifndef PDIR
FB_SRC_DIR = $(abspath ../../../../flameboss/Sources/flameboss)
FB_SRC_DIR = $(abspath ../../../../bsp/nau7802)
VPATH = $(FB_SRC_DIR)
CSRCS = \
	cli.c \
	configuration.c \
	fb.c \
	fb_meat_alarm.c \
	fb_open_pit.c \
	fb_pit_alarm.c \
	fb_wifi.c \
	pid.c \
	adc_nau7802.c

GEN_LIBS = libflameboss.a
endif

#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#
#DEFINES +=



#############################################################
# Recursion Magic - Don't touch this!!
#
# Each subtree potentially has an include directory
#   corresponding to the common APIs applicable to modules
#   rooted at that subtree. Accordingly, the INCLUDE PATH
#   of a module can only contain the include directories up
#   its parent path, and not its siblings
#
# Required for each makefile to inherit from the parent
#

INCLUDES += \
	-I . \
	-I ../bsp/esp8266-sdk \
	-I ../bsp/ee_printf \
	-I ../bsp/crc32 \
	-I ../bsp/common \
	-I ../../driver_lib/include/driver

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

