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
TARGET = eagle
#FLAVOR = release
FLAVOR = debug

#EXTRA_CCFLAGS += -u

ifndef PDIR # {
GEN_IMAGES= eagle.app.v6.out
GEN_BINS= eagle.app.v6.bin
SPECIAL_MKTARGETS=$(APP_MKTARGETS)
SUBDIRS=    \
	bsp/fb400 \
	bsp/fan \
	bsp/common \
	bsp/crc32 \
	bsp/ee_printf \
	bsp/esp8266-sdk \
	flameboss \
	driver \
	paho

endif # } PDIR

APPDIR = .
LDDIR = ../ld

ifeq ($(APP), 1)
APP_ADDR=0x1000
APP_PATH=../bin/upgrade/user1.2048.new.3.bin
else
APP_ADDR=0x81000
APP_PATH=../bin/upgrade/user2.2048.new.3.bin
endif
ESPTOOL=esptool -v
CCFLAGS += -Og

TARGET_LDFLAGS =		\
	-nostdlib		\
	-Wl,-EL \
	--longcalls \
	--text-section-literals

ifeq ($(FLAVOR),debug)
    TARGET_LDFLAGS += -ggdb -Og
endif

ifeq ($(FLAVOR),release)
    TARGET_LDFLAGS += -g -O0
endif

COMPONENTS_eagle.app.v6 = \
	flameboss/libflameboss.a \
	bsp/crc32/libbsp_crc32.a \
	bsp/ee_printf/libbsp_ee_printf.a \
	bsp/esp8266-sdk/libbsp_esp8266-sdk.a \
	bsp/fan/libbsp_fan.a \
	bsp/fb400/libbsp_fb400.a \
	bsp/common/libbsp_common.a \
	driver/libdriver.a \
	paho/libpaho.a

LINKFLAGS_eagle.app.v6 = \
	-Xlinker -Map=$(ODIR)/app.map \
	-Xlinker ../../version.ld \
	-L../lib        \
	-nostdlib	\
    -T$(LD_FILE)   \
	-Wl,--no-check-sections	\
	-Wl,--gc-sections	\
    -u call_user_start	\
	-Wl,-static						\
	-Wl,--start-group					\
	-lc					\
	-lgcc					\
	-lhal					\
	-lphy	\
	-lpp	\
	-lnet80211	\
	-llwip	\
	-lwpa	\
	-lcrypto	\
	-lmain	\
	-ljson	\
	-lupgrade\
	-lssl	\
	-lpwm	\
	-lsmartconfig \
	$(DEP_LIBS_eagle.app.v6)					\
	-Wl,--end-group

DEPENDS_eagle.app.v6 = \
                $(LD_FILE) \
                $(LDDIR)/eagle.rom.addr.v6.ld

#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#

#UNIVERSAL_TARGET_DEFINES =		\

# Other potential configuration flags include:
#	-DTXRX_TXBUF_DEBUG
#	-DTXRX_RXBUF_DEBUG
#	-DWLAN_CONFIG_CCX
GDBSTUB_DEFS = \
	-DGDBSTUB_FREERTOS=0 \
	-DGDBSTUB_USE_OWN_STACK=1 \
	-DGDBSTUB_CTRLC_BREAK=0 \
	-DGDBSTUB_REDIRECT_CONSOLE_OUTPUT=0 \
	-DGDBSTUB_BREAK_ON_INIT=0
FLAMEBOSS_DEFS = -DCONFIG_CC3000=0 -DMQTTCLIENT_PLATFORM_HEADER=fb_mqtt_paho.h
CONFIGURATION_DEFINES =	-DICACHE_FLASH -DCONFIG_ESP=1 $(GDBSTUB_DEFS) $(FLAMEBOSS_DEFS)

DEFINES +=				\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)

DDEFINES +=				\
	$(UNIVERSAL_TARGET_DEFINES)	\
	$(CONFIGURATION_DEFINES)


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

INCLUDES := $(INCLUDES) \
	-I$(PDIR)../driver_lib/include \
	-I$(PDIR)../../../flameboss/Sources/flameboss \
	-I$(PDIR)../../../flameboss/Sources/unit_tests \
	-I$(PDIR)../../../bsp/fb400 \
	-I$(PDIR)../../../bsp/fan \
	-I$(PDIR)../../../bsp/esp8266-sdk \
	-I$(PDIR)../../../bsp/ee_printf \
	-I$(PDIR)../../../bsp/crc32 \
	-I$(PDIR)../../../bsp/common \
	-I$(PDIR)../../../paho.mqtt.embedded-c/MQTTPacket/src \
	-I$(PDIR)../../../paho.mqtt.embedded-c/MQTTClient-C/src \
	-I$(PDIR)../..
PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

.PHONY: FORCE
FORCE:

gdb:
	cp esp-gdbstub/gdbcmds ~/tmp/
	sed -i 's%/dev/ttyUSB0%$(ESP_PORT)%' ~/tmp/gdbcmds
	cd esp-gdbstub && xtensa-lx106-elf-gdb -x ~/tmp/gdbcmds

gdbserver: .gdbserver

.gdbserver:
	touch .gdbserver
	-socat -v $(ESP_PORT),clocal=0 TCP-LISTEN:9980
	rm .gdbserver

program_once:
	$(ESPTOOL) -cs 3 -cp $(ESP_PORT) \
		-ca 0 -cf ../bin/boot_v1.7.bin \
		-ca 0x1000 -cf ../bin/upgrade/user1.2048.new.3.bin \
		-ca 0x1FB000 -cf ../bin/blank.bin \
		-ca 0x1FC000 -cf ../bin/esp_init_data_default_v08.bin \
		-ca 0x1FE000 -cf ../bin/blank.bin

program:
	$(ESPTOOL) -cs 3 -cp $(ESP_PORT) -ca $(APP_ADDR) -cf $(APP_PATH)

erase:
	LANG=C dd if=/dev/zero bs=1m count=1 | tr "\000" "\377" >/tmp/ff.bin
	$(ESPTOOL) -cp $(ESP_PORT) -ca 0xfb000 -cf /tmp/ff.bin

