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
SUBDIRS =
FB_SRC_DIR = $(abspath ../../../../paho.mqtt.embedded-c/MQTTPacket/src) \
           $(abspath ../../../../paho.mqtt.embedded-c/MQTTClient-C/src)
VPATH = $(FB_SRC_DIR)
CSRCS = \
	MQTTConnectServer.c \
	MQTTSubscribeServer.c \
	MQTTSerializePublish.c \
	MQTTDeserializePublish.c \
	MQTTUnsubscribeClient.c \
	MQTTUnsubscribeServer.c \
	MQTTPacket.c \
	MQTTSubscribeClient.c \
	MQTTFormat.c \
	MQTTConnectClient.c \
	MQTTClient.c

GEN_LIBS = libpaho.a
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

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

