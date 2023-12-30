# Makefile for fproftest
# To build: "make" or "make all"
# To clean: "make clean"

.PHONY: all clean

#ALLEGRO_FLAGS=-I/usr/local/include/allegro5
#DAP_FLAGS=-I/usr/local/include/dap
#FONT_FLAGS=-I/usr/local/include/gdcfonts

CC=gcc
CFLAGS=-Wall -O0 -ggdb -std=gnu99 -DDEBUG -finstrument-functions -export-dynamic -Wl,--export-dynamic
DFLAGS=-MP -MD

#TLIBS=-L/usr/local/lib/gdcfonts -Wl,-R/usr/local/lib/gdcfonts -lgdcfonts
#TLIBS+=-L/usr/local/lib/dap -Wl,-R/usr/local/lib/dap -ldapterm -ldapiou
#TLIBS+=-L/usr/local/lib -Wl,-R/usr/local/lib -lallegro_primitives -lallegro -lallegro_image
#TLIBS+=-lallegro_color -lallegro_main
TLIBS=-L/usr/local/lib -Wl,-R/usr/local/lib -lfprof
TLIBS+= -lpthread -lm -ldl -lrt
# Test builds
TSRC=src
TSRCS=$(wildcard $(TSRC)/*.c)
TBIN=bin
TBINS=$(patsubst $(TSRC)/%.c, $(TBIN)/%, $(TSRCS))
TDEPFILES=$(patsubst $(TSRC)/%.c, $(TBIN)/%.d, $(TSRCS))

$(shell mkdir -p $(TBIN))

all: $(TBINS)

$(TBIN)/%: $(TSRC)/%.c
	@echo "=== Building Test program $<"
	$(CC) -o $@ $< $(CFLAGS) $(DFLAGS) $(TLIBS)

clean:
	@echo "=== Clean everything"
	$(RM) -r bin/*

-include $(TDEPFILES)