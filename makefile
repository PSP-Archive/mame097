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

#if (0==PSP_NO_CPU32)
#endif //(0==PSP_NO_CPU32)

# uncomment one of the next lines to build a target-optimized build
# ATHLON = 1
# I686 = 1
# P4 = 1
# PM = 1
  PSP =1
# PSP2 =1

# uncomment next line to include the symbols for symify
# SYMBOLS = 1

# uncomment next line to generate a link map for exception handling in windows
# MAP = 1

# uncomment next line to include the debugger
# DEBUG = 1

# uncomment next line to use the new multiwindow debugger
NEW_DEBUGGER = 1

# uncomment next line to use DRC MIPS3 engine
#X86_MIPS3_DRC = 1

# uncomment next line to use DRC PowerPC engine
#X86_PPC_DRC = 1

# uncomment next line to use cygwin compiler
# COMPILESYSTEM_CYGWIN	= 1

# uncomment next line to build expat as part of MAME build
BUILD_EXPAT = 1

# uncomment next line to build zlib as part of MAME build
BUILD_ZLIB = 1


# set this the operating system you're building for
# MAMEOS = msdos
# MAMEOS = windows
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
AR = @ar
CC = @gcc
LD = @gcc
ASM = @nasm
#TMK ASMFLAGS = -f coff
#ASMFLAGS = -march=r4000 -g -mabi=eabi -mgp32 -c -xassembler -O
MD = -mkdir.exe
RM = @rm -f


ifeq ($(MAMEOS),msdos)
PREFIX = d
else
PREFIX =
endif

# by default, compile for Pentium target and add no suffix
NAME = $(PREFIX)$(TARGET)$(SUFFIX)
ARCH = -march=pentium

# architecture-specific builds get extra options and a suffix
ifdef ATHLON
NAME = $(PREFIX)$(TARGET)$(SUFFIX)at
ARCH = -march=athlon
endif

ifdef I686
NAME = $(PREFIX)$(TARGET)$(SUFFIX)pp
ARCH = -march=pentiumpro
endif

ifdef P4
NAME = $(PREFIX)$(TARGET)$(SUFFIX)p4
ARCH = -march=pentium4
endif

ifdef PM
NAME = $(PREFIX)$(TARGET)$(SUFFIX)pm
ARCH = -march=pentium3 -msse2
endif

ifdef PSP
NAME = $(PREFIX)$(TARGET)$(SUFFIX)psp
ARCH = -march=r4000 -mabi=eabi -mgp32 -mlong32 -msingle-float -include src/psp/floatonly.h
endif

ifdef PSP2
NAME = $(PREFIX)$(TARGET)$(SUFFIX)psp2
ARCH = -mabi=eabi -mgp32 -mlong32 -msingle-float -include src/psp/floatonly.h -include src/psp/stdout.h
MAMEOS2 = psp2
endif

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

CFLAGS = -std=gnu99 -Isrc -Isrc/includes -Isrc/debug -Isrc/$(MAMEOS) -I$(OBJ)/cpu/m68000 -Isrc/cpu/m68000

ifdef SYMBOLS
CFLAGS += -O0 -Wall -Wno-unused -g
else
CFLAGS += -DNDEBUG \
	$(ARCH) -O3 -fomit-frame-pointer -fno-strict-aliasing \
	-Wno-sign-compare \
	-Wunused \
	-Wpointer-arith  \
	-Wstrict-prototypes \
	-Wundef \
	-Wformat \
	-Wwrite-strings \
	-Wdisabled-optimization \
	-Wbad-function-cast \

#	-Wcast-align 		# ←ＭＡＭＥの構造上、直すのめんどくさいからＯＦＦ。
#	-Wformat-security 	# ←よくわかんない。

#TMK	-Werror -Wall -Wno-sign-compare -Wunused -Wno-unused-functions 
#	-Wredundant-decls
#	-Wfloat-equal
#	-Wunreachable-code -Wpadded
#	-W had to remove because of the "missing initializer" warning
#	-Wlarger-than-262144  
#	-Wcast-qual 
#	-Wconversion 
#	-Wmissing-prototypes 
#	-Wmissing-declarations
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
$(EMULATOR): $(OBJ)/psp/startup.o $(OBJS) $(COREOBJS) $(OSOBJS) $(DRVLIBS) $(EXPAT) $(ZLIB) $(OSDBGOBJS)
	@echo Linking $@...
	$(LD) -O0 $^ $(LIBS) -M -Ttext 8810000 -q -o out > main.map
	outpatch
	$(STRIP) outp
	./elf2pbp outp "MAME 0.97 $(TARGET)"
	cp EBOOT.PBP pbp/$(TARGET)

#	cp icon/$(TARGET).png ./Icon0.png

#TMK	$(LD) $(LDFLAGS) $(OSDBGLDFLAGS) $^ $(LIBS) -o $@ $(MAPFLAGS)

#TMK romcmp$(EXE): $(OBJ)/romcmp.o $(OBJ)/unzip.o $(ZLIB) $(OSDBGOBJS)
#	@echo Linking $@...
#	$(LD) $(LDFLAGS) $(OSDBGLDFLAGS) $^ $(LIBS) -o $@
#
#chdman$(EXE): $(OBJ)/chdman.o $(OBJ)/chd.o $(OBJ)/chdcd.o $(OBJ)/cdrom.o $(OBJ)/md5.o $(OBJ)/sha1.o $(OBJ)/version.o $(ZLIB) $(OSDBGOBJS)
#	@echo Linking $@...
#	$(LD) $(LDFLAGS) $(OSDBGLDFLAGS) $^ $(LIBS) -o $@
#
#xml2info$(EXE): $(OBJ)/xml2info.o $(EXPAT) $(OSDBGOBJS)
#	@echo Linking $@...
#	$(LD) $(LDFLAGS) $(OSDBGLDFLAGS) $^ $(LIBS) -o $@

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

#$(OBJ)/cpu/m6809/m6809.o: src/cpu/m6809/m6809.c src/cpu/m6809/m6809.h src/cpu/m6809/6809ops.c src/cpu/m6809/6809tbl.c
#	@echo Compiling $<...
#	$(CC) $(CDEFS) $(CFLAGS) -c $< -o $@


# compile generated C files for the 68000 emulator
##$(M68000_GENERATED_OBJS): $(OBJ)/cpu/m68000/m68kmake$(EXE)
##	@echo Compiling $(subst .o,.c,$@)...
##	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -c $*.c -o $@

# additional rule, because m68kcpu.c includes the generated m68kops.h :-/
###$(OBJ)/cpu/m68000/m68kcpu.o: $(OBJ)/cpu/m68000/m68kmake$(EXE)

# generate C source files for the 68000 emulator
###$(OBJ)/cpu/m68000/m68kmake$(EXE): src/cpu/m68000/m68kmake.c $(OSDBGOBJS)
###	@echo M68K make $<...
###	cp obj/m68000/* $(OBJ)/cpu/m68000

#TMK	$(CC) $(CDEFS) $(CFLAGSPEDANTIC) -DDOS -o $(OBJ)/cpu/m68000/m68kmake$(EXE) $< $(OSDBGOBJS)
#	@echo Generating M68K source files...
#	$(OBJ)/cpu/m68000/m68kmake$(EXE) $(OBJ)/cpu/m68000 src/cpu/m68000/m68k_in.c

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
