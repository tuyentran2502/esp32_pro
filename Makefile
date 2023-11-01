#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#
PROJECT_NAME := mqtt_tcp

EXTRA_COMPONENT_DIRS = $(IDF_PATH)/examples/common_components/protocol_examples_common
EXTRA_COMPONENT_DIRS = $(IDF_PATH)/components/
COMPONENT_EMBED_TXTFILES := ../html/config_wifi.html

include $(IDF_PATH)/make/project.mk
