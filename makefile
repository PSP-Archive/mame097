# set this to mame, mess or the destination you want to build

 TARGET = namcoNA
#TARGET = taitoAX
#TARGET = m72
#TARGET = namcos86

#################↑ここを変えるのでっす。

#TARGET = taitoF2
#TARGET = taitoF1
#TARGET = namcos1
#TARGET = sys16
#TARGET = darius2d
#TARGET = nightstr
#TARGET = taitoZ
#TARGET = taitoA2
#TARGET = taito2
#TARGET = namco

# TARGET = mame
# TARGET = mess
# example for a tiny compile
# TARGET = tiny
ifeq ($(TARGET),)
TARGET = mame
endif

# customize option
OPT_DEFS = -D$(TARGET)=1
### [game fixed 16 bit]入出力共に１６ビット色固定なら１。（アルファがあれば使えない。例えばF3。）
# ( MAMEのアルファブレンディング（半透明、加減算色付合成）は一部の実装が32ビット色固定だから。 )
 OPT_DEFS += -DPSP_FORCE16=1
### [game nouse ROZ]ROZ(拡大縮小付回転)機能、使わないなら１。(基本は１)
 OPT_DEFS += -DPSP_FORCE_NO_ROZ=1
#OPT_DEFS += -DPSP_FORCE_NO_ROZ=0
### [game nouse ZOOM]ZOOM(拡大縮小だが回転なし)機能、使わないなら１。(基本は１)
 OPT_DEFS += -DPSP_FORCE_NO_ZOOM=1
#OPT_DEFS += -DPSP_FORCE_NO_ZOOM=0
### [for F2(chr have roz)]F2の場合は１。それ以外は０。(基本は０)
# ( F2専用ののROZなしだが、「CHRダケROZ有り」とゆー変なフラグ。)
#OPT_DEFS += -DPSP_F2_ROZ_TILE=1
 OPT_DEFS += -DPSP_F2_ROZ_TILE=0
### [more bank namco S1, sega_sys16] sega_sys16用,namco S1用にバンク増やす。（当然メモリが減る）。使わないなら１。(基本は１)
#OPT_DEFS += -DPSP_LIGHT_BANK=1
 OPT_DEFS += -DPSP_LIGHT_BANK=0
## [no MAHJONG]これは強制１でしょ？不満なら、ＵＩ作ってよ。使わないなら１。(基本は１)
 OPT_DEFS += -DPSP_NO_MAHJONG=1
#OPT_DEFS += -DPSP_NO_MAHJONG=0
## [extention 32]32bit CPU。使わないなら１。(基本は１)
 OPT_DEFS += -DPSP_NO_CPU32=1
#OPT_DEFS += -DPSP_NO_CPU32=0
## [tilemap88]8x8CHR専用なら１。(高速は１)8x8CHR専用で反転機能使わないなら２。(最高速は２)（不明なら０：汎用）
 OPT_DEFS += -DPSP_CHR_MAP88=2
#OPT_DEFS += -DPSP_CHR_MAP88=0

# uncomment next line to use the new multiwindow debugger
NEW_DEBUGGER = 1

# uncomment next line to build expat as part of MAME build
BUILD_EXPAT = 1

# uncomment next line to build zlib as part of MAME build
BUILD_ZLIB = 1


# set this the operating system you're building for
MAMEOS = psp
MAMEOS2 = psp
ifeq ($(MAMEOS),)
MAMEOS = windows
endif

# extension for executables
EXE = .exe

# CPU core include paths
VPATH=src $(wildcard src/cpu/*)

# compiler, linker and utilities
PSPSDK=$(shell psp-config --pspsdk-path)

# compiler, linker and utilities
AR = @ar
CC = @gcc
LD = @gcc
ASM = @nasm
#TMK ASMFLAGS = -f coff
#ASMFLAGS = -march=r4000 -g -mabi=eabi -mgp32 -c -xassembler -O
MD = -mkdir
RM = @rm -f


ifeq ($(MAMEOS),msdos)
PREFIX = d
else
PREFIX =
endif

# architecture-specific builds get extra options and a suffix
NAME = $(PREFIX)$(TARGET)$(SUFFIX)psp

# debug builds just get the 'd' suffix and nothing more
ifdef DEBUG
NAME = $(PREFIX)$(TARGET)$(SUFFIX)d
endif


# build the targets in different object dirs, since mess changes
# some structures and thus they can't be linked against each other.
OBJ = obj/$(NAME)

#TMK EMULATOR = $(NAME)$(EXE)
EMULATOR = EBOOT.PBP

#TMK DEFS = -DX86_ASM -DLSB_FIRST -DINLINE="static __inline__" -Dasm=__asm__ -DCRLF=3
DEFS = -DLSB_FIRST -DINLINE="static __inline__" -Dasm=__asm__ -DCRLF=3 -DPI=M_PI -DCLOCKS_PER_SEC=1000000
ifdef NEW_DEBUGGER
DEFS += -DNEW_DEBUGGER
endif

CFLAGS = -G0 -D_HUGE=HUGE_VAL -DCLIB_DECL= -I$(PSPSDK)/include
CFLAGS += -std=gnu99 -Isrc -Isrc/includes -Isrc/debug -Isrc/psp -I$(OBJ)/cpu/m68000 -Isrc/cpu/m68000

ifdef SYMBOLS
CFLAGS += -O0 -Wall -Wno-unused -g
else
CFLAGS += -DNDEBUG \
	$(ARCH) -Og -fomit-frame-pointer -fno-strict-aliasing \
	-Wall -Wno-sign-compare -Wunused \
	-Wpointer-arith -Wbad-function-cast -Wcast-align \
	-Wstrict-prototypes -Wundef \
	-Wformat-security -Wwrite-strings \
	-Wdisabled-optimization \

endif

# extra options needed *only* for the osd files
CFLAGSOSDEPEND = $(CFLAGS)

# the windows osd code at least cannot be compiled with -pedantic
CFLAGSPEDANTIC = $(CFLAGS) -pedantic

ifdef SYMBOLS
LDFLAGS =
else
#LDFLAGS = -s -Wl,--warn-common
LDFLAGS = -s
endif

ifdef MAP
MAPFLAGS = -Wl,-Map,$(NAME).map
else
MAPFLAGS =
endif

OBJDIRS = obj $(OBJ) $(OBJ)/cpu $(OBJ)/sound $(OBJ)/$(MAMEOS) \
	$(OBJ)/drivers $(OBJ)/machine $(OBJ)/vidhrdw $(OBJ)/sndhrdw $(OBJ)/debug pbp pbp/$(TARGET)
ifdef MESS
OBJDIRS += $(OBJ)/mess $(OBJ)/mess/systems $(OBJ)/mess/machine \
	$(OBJ)/mess/vidhrdw $(OBJ)/mess/sndhrdw $(OBJ)/mess/tools
endif

# start with an empty set of libs
LIBS = 

LIBS += -L$(PSPSDK)/lib -lpspgu -lpspge -lpsppower -lpspaudiolib -lpspaudio -lm
PSPSDK_LIBS = -lpspdebug -lpspdisplay -lpspctrl -lpspsdk
LIBS     := $(LIBS) $(PSPSDK_LIBS) -lc -lpspnet \
		-lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsputility \
		-lpspuser -lpspkernel

#ifdef BUILD_EXPAT
#CFLAGS += -Isrc/expat
#OBJDIRS += $(OBJ)/expat
#EXPAT = $(OBJ)/libexpat.a
#else
LIBS += -lexpat
EXPAT =
#endif

ifdef BUILD_ZLIB
CFLAGS += -Isrc/zlib
OBJDIRS += $(OBJ)/zlib
ZLIB = $(OBJ)/libz.a
else
LIBS += -lz
ZLIB =
endif

all:	maketree emulator extra

# include OS-specific rules first
include src/$(MAMEOS)/$(MAMEOS2).mak

# then the various core pieces
# include the various .mak files
include src/makes/core.mak
include src/makes/$(TARGET).mak
include src/makes/o_rules.mak
#include src/cpu/cpu.mak
#include src/sound/sound.mak

#ifdef DEBUG
#DBGDEFS = -DMAME_DEBUG
#else
#DBGDEFS =
DBGOBJS =
#endif

ifdef COMPILESYSTEM_CYGWIN
CFLAGS	+= -mno-cygwin
LDFLAGS	+= -mno-cygwin
endif

emulator: maketree $(EMULATOR)

#TMK extra:	$(TOOLS) $(TEXTS)
extra:	$(TEXTS)

# combine the various definitions to one
CDEFS = $(DEFS) $(COREDEFS) $(CPUDEFS) $(SOUNDDEFS) $(ASMDEFS) $(OPT_DEFS)

# $(DBGDEFS)

## always recompile the version string
##	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -c src/version.c -o $(OBJ)/version.o

# primary target
$(EMULATOR): $(OBJS) $(COREOBJS) $(OSOBJS) $(DRVLIBS) $(EXPAT) $(ZLIB) $(OSDBGOBJS)
	@echo Linking $@...
	$(LD) -O0 $^ $(LIBS) -o $(TARGET).elf
	$(FIXUP) $(TARGET).elf
	$(STRIP) $(TARGET).elf -o $(TARGET)_strip.elf
	$(MKSFO) '$(PSP_EBOOT_TITLE)' PARAM.SFO
	$(PACK_PBP) EBOOT.PBP PARAM.SFO icon0.png  \
		NULL NULL NULL  \
		NULL $(TARGET)_strip.elf NULL
	-rm -f $(TARGET)_strip.elf

# secondary libraries
$(OBJ)/libexpat.a: $(OBJ)/expat/xmlparse.o $(OBJ)/expat/xmlrole.o $(OBJ)/expat/xmltok.o

$(OBJ)/libz.a: $(OBJ)/zlib/adler32.o $(OBJ)/zlib/compress.o $(OBJ)/zlib/crc32.o $(OBJ)/zlib/deflate.o \
				$(OBJ)/zlib/gzio.o $(OBJ)/zlib/inffast.o $(OBJ)/zlib/inflate.o $(OBJ)/zlib/infback.o \
				$(OBJ)/zlib/inftrees.o $(OBJ)/zlib/trees.o $(OBJ)/zlib/uncompr.o $(OBJ)/zlib/zutil.o

$(OBJ)/$(MAMEOS)/%.o: src/$(MAMEOS)/%.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGSOSDEPEND) -c $< -o $@

$(OBJ)/%.o: src/%.c
	@echo Compiling $<...
	$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.a:
	@echo Archiving $@...
	$(RM) $@
	$(AR) cr $@ $^

$(sort $(OBJDIRS)):
	$(MD) $@

maketree: $(sort $(OBJDIRS))

clean:
	@echo Deleting object tree $(OBJ)...
	$(RM) -r $(OBJ)
	@echo Deleting $(EMULATOR)...
	$(RM) $(EMULATOR)

##check: $(EMULATOR) xml2info$(EXE)
##	./$(EMULATOR) -listxml > $(NAME).xml
##	./xml2info < $(NAME).xml > $(NAME).lst
##	./xmllint --valid --noout $(NAME).xml
