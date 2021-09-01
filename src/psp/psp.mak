LD = psp-ld
CC       = psp-gcc
CXX      = psp-g++
AS       = psp-gcc
LD       = psp-gcc
AR       = psp-ar
RANLIB   = psp-ranlib
STRIP    = psp-strip
MKSFO    = mksfo
PACK_PBP = pack-pbp
FIXUP    = psp-fixup-imports

CFLAGS += -D_HUGE=HUGE_VAL -DCLIB_DECL=

# add libs
LIBS += -L$(PSPSDK)/lib -lpspgu -lpspge -lpsppower -lpspaudiolib -lpspaudio -lm
PSPSDK_LIBS = -lpspdebug -lpspdisplay -lpspctrl -lpspsdk
LIBS     := $(LIBS) $(PSPSDK_LIBS) -lc -lpspnet \
		-lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsputility \
		-lpspuser -lpspkernel

# only PSP specific output files and rules
OSOBJS = $(OBJ)/psp/psp.o $(OBJ)/psp/video.o $(OBJ)/psp/blit.o \
	$(OBJ)/psp/sound.o $(OBJ)/psp/input.o $(OBJ)/psp/rc.o \
	$(OBJ)/psp/misc.o $(OBJ)/psp/ticker.o $(OBJ)/psp/config.o \
	$(OBJ)/psp/fileio.o \
	$(OBJ)/psp/pg.o $(OBJ)/psp/pspmain.o $(OBJ)/psp/menu.o 

