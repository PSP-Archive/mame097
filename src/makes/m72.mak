# Irem M72

# [unsupport] &driver_majtitle,

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="\
driver_rtype,\
driver_rtypepj,\
driver_rtypeu,\
driver_bchopper,\
driver_mrheli,\
driver_nspirit,\
driver_nspiritj,\
driver_imgfight,\
driver_loht,\
driver_xmultipl,\
driver_dbreed,\
driver_rtype2,\
driver_rtype2j,\
driver_hharry,\
driver_hharryu,\
driver_dkgensan,\
driver_dkgenm72,\
driver_poundfor,\
driver_poundfou,\
driver_airduel,\
driver_cosmccop,\
driver_gallop,\
driver_kengo"


COREDEFS += -DTINY_POINTER="\
&driver_rtype,\
&driver_rtypepj,\
&driver_rtypeu,\
&driver_bchopper,\
&driver_mrheli,\
&driver_nspirit,\
&driver_nspiritj,\
&driver_imgfight,\
&driver_loht,\
&driver_xmultipl,\
&driver_dbreed,\
&driver_rtype2,\
&driver_rtype2j,\
&driver_hharry,\
&driver_hharryu,\
&driver_dkgensan,\
&driver_dkgenm72,\
&driver_poundfor,\
&driver_poundfou,\
&driver_airduel,\
&driver_cosmccop,\
&driver_gallop,\
&driver_kengo"


# uses these CPUs
#CPUS+=V30@
CPUS+=NEC30@
CPUS+=Z80@

#CPUS+=M68000@
#CPUS+=M68020@
#CPUS+=M68EC020@
#CPUS+=M6800@
#CPUS+=TMS32025@



# uses these SOUNDs
SOUNDS+=YM2151@
SOUNDS+=DAC@



#SOUNDS+=ES5505@
#SOUNDS+=MSM5205@
#SOUNDS+=OKIM6295@

#SOUNDS+=YM2203@
#SOUNDS+=YM2610@
#SOUNDS+=YM2610B@


#SOUNDS+=UPD7759@
#SOUNDS+=YM2413@
#SOUNDS+=SN76496@
#SOUNDS+=YM3812@
#SOUNDS+=YM3438@
#SOUNDS+=YM2612@

#SOUNDS+=X1_010@
#SOUNDS+=CUSTOM@

DMMY=\
	$(OBJ)/machine/bonzeadv.o $(OBJ)/vidhrdw/asuka.o $(OBJ)/drivers/asuka.o \
	$(OBJ)/machine/rainbow.o $(OBJ)/drivers/rainbow.o \
	$(OBJ)/vidhrdw/rastan.o $(OBJ)/drivers/rastan.o \
	$(OBJ)/drivers/opwolf.o \
	$(OBJ)/vidhrdw/othunder.o $(OBJ)/drivers/othunder.o \
	$(OBJ)/machine/volfied.o $(OBJ)/vidhrdw/volfied.o $(OBJ)/drivers/volfied.o \
	$(OBJ)/vidhrdw/wgp.o $(OBJ)/drivers/wgp.o \
	$(OBJ)/drivers/taito_x.o  $(OBJ)/vidhrdw/taito_s.o $(OBJ)/machine/cchip.o \
	$(OBJ)/vidhrdw/topspeed.o $(OBJ)/drivers/topspeed.o \
\
	$(OBJ)/vidhrdw/taito_h.o $(OBJ)/drivers/taito_h.o \
	$(OBJ)/vidhrdw/taito_b.o $(OBJ)/drivers/taito_b.o \
	$(OBJ)/vidhrdw/taito_z.o $(OBJ)/drivers/taito_z.o \
	$(OBJ)/vidhrdw/taito_f2.o $(OBJ)/drivers/taito_f2.o \
	$(OBJ)/vidhrdw/slapshot.o $(OBJ)/drivers/slapshot.o $(OBJ)/machine/timekpr.o \


DRVLIBS = \
$(OBJ)/machine/irem_cpu.o \
$(OBJ)/vidhrdw/m72.o \
$(OBJ)/drivers/m72.o \
$(OBJ)/sndhrdw/m72.o \

DD=\
	$(OBJ)/vidhrdw/taitoic.o $(OBJ)/sndhrdw/taitosnd.o \
	$(OBJ)/machine/mb87078.o \
	$(OBJ)/vidhrdw/darius.o $(OBJ)/drivers/darius.o \
	$(OBJ)/vidhrdw/ninjaw.o $(OBJ)/drivers/ninjaw.o \
	$(OBJ)/vidhrdw/warriorb.o $(OBJ)/drivers/warriorb.o \

#	$(OBJ)/vidhrdw/taito_f3.o $(OBJ)/sndhrdw/taito_f3.o $(OBJ)/drivers/taito_f3.o 
#	$(OBJ)/vidhrdw/groundfx.o $(OBJ)/drivers/groundfx.o 
#	$(OBJ)/vidhrdw/undrfire.o $(OBJ)/drivers/undrfire.o 
#	$(OBJ)/vidhrdw/gunbustr.o $(OBJ)/drivers/gunbustr.o 
#	$(OBJ)/vidhrdw/superchs.o $(OBJ)/drivers/superchs.o 
#	$(OBJ)/drivers/taitojc.o 

#	$(OBJ)/vidhrdw/taito_l.o $(OBJ)/drivers/taito_l.o 



# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
