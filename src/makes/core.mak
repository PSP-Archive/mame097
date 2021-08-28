# the core object files (without target specific objects;
# those are added in the target.mak files)
COREOBJS =  $(OBJ)/mame.o \
	$(OBJ)/drawgfx.o $(OBJ)/common.o $(OBJ)/usrintrf.o $(OBJ)/ui_text.o \
	$(OBJ)/cpuintrf.o $(OBJ)/cpuexec.o $(OBJ)/cpuint.o $(OBJ)/memory.o $(OBJ)/timer.o \
	$(OBJ)/palette.o $(OBJ)/input.o $(OBJ)/inptport.o $(OBJ)/config.o $(OBJ)/unzip.o \
	$(OBJ)/audit.o $(OBJ)/png.o 

COREOBJS += $(OBJ)/tilemap00.o 

COREOBJS += $(OBJ)/fileio.o \
	$(OBJ)/state.o $(OBJ)/datafile.o \
	$(sort $(CPUOBJS)) \
	$(OBJ)/sndintrf.o \
	$(OBJ)/sound/streams.o \
	$(sort $(SOUNDOBJS)) \
	$(OBJ)/vidhrdw/generic.o \
	$(OBJ)/machine/eeprom.o \
	$(OBJ)/profiler.o \
	$(OBJ)/hash.o \
	$(OBJ)/chd.o \
	$(OBJ)/sound/wavwrite.o
#
# $(OBJ)/hiscore.o
#$(OBJ)/info.o 
# $(OBJ)/sha1.o
# $(OBJ)/md5.o
#
#$(OBJ)/version.o
# $(OBJ)/artwork.o 
#
#	$(OBJ)/vidhrdw/vector.o
#	$(OBJ)/harddisk.o $(OBJ)/cdrom.o 

#$(OBJ)/sound/filter.o $(OBJ)/sound/flt_vol.o $(OBJ)/sound/flt_rc.o 

#ifdef NEW_DEBUGGER
#COREOBJS += $(OBJ)/debug/debugcmd.o $(OBJ)/debug/debugcpu.o $(OBJ)/debug/debugexp.o \
#			$(OBJ)/debug/debugvw.o $(OBJ)/debug/debughlp.o $(OBJ)/debug/debugcon.o
#else
#COREOBJS += $(OBJ)/debug/mamedbg.o $(OBJ)/debug/window.o
#endif


#COREOBJS += $(sort $(DBGOBJS))

TOOLS = 
#romcmp$(EXE) chdman$(EXE)

