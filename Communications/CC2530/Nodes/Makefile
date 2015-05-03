DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

HAVE_BANKING=1
UIP_CONF_IPV6=1
UIP_CONF_RPL=1

NODE_PROJECT = node

ifeq ($(RPL_ROOT), 1)
  CFLAGS += -DIS_RPL_ROOT
endif

all: $(NODE_PROJECT)

CONTIKI = ../../contiki
include $(CONTIKI)/Makefile.include
