##############################################################################

OBJS +=	plug_main.o plug_key_convert.o

LIB=libbbcvcloud.a

##############################################################################

CC     = $(CROSS_COMPILE)gcc
CPP    = $(CROSS_COMPILE)g++
LD     = $(CROSS_COMPILE)gcc
AR     = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib

##############################################################################

CFLAGS += -I$(BLCSRC_PATH)/include 
CFLAGS += -I$(BLCSRC_PATH)/include/os 
CFLAGS += -I$(BLCSRC_PATH)/include/plug
CFLAGS += -D__USE_GNU
CFLAGS += -DDEBUGTS

CFLAGS += -O2 -Wundef -Wall

##############################################################################

$(LIB): $(OBJS)
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@
	@echo Generate $(LIB) ok
	
install:
	cp $(LIB) $(BLCSRC_PATH)/lib
	
uninstall:
	rm -rf $(BLCSRC_PATH)/lib/$(LIB)
	
clean:
	rm -rf $(OBJS) $(LIB)

##############################################################################

