
#Taito Asuka & Taito X system  only

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="\
driver_bonzeadv,\
driver_bonzeadu,\
driver_jigkmgri,\
driver_asuka,\
driver_mofflott,\
driver_cadash,\
driver_cadashj,\
driver_cadashu,\
driver_cadashi,\
driver_cadashf,\
driver_galmedes,\
driver_earthjkr,\
driver_eto,\
\
driver_superman,\
driver_suprmanj,\
driver_twinhawk,\
driver_twinhwku,\
driver_daisenpu,\
driver_gigandes,\
driver_kyustrkr,\
driver_ballbros"

#&driver_topland,&driver_ainferno,
#&driver_fgoal,&driver_fgoala,

COREDEFS += -DTINY_POINTER="\
&driver_bonzeadv,\
&driver_bonzeadu,\
&driver_jigkmgri,\
&driver_asuka,\
&driver_mofflott,\
&driver_cadash,\
&driver_cadashj,\
&driver_cadashu,\
&driver_cadashi,\
&driver_cadashf,\
&driver_galmedes,\
&driver_earthjkr,\
&driver_eto,\
\
&driver_superman,\
&driver_suprmanj,\
&driver_twinhawk,\
&driver_twinhwku,\
&driver_daisenpu,\
&driver_gigandes,\
&driver_kyustrkr,\
&driver_ballbros"

# uses these CPUs
#CPUS+=M68000@
 CPUS+=N68000@
 CPUS+=Z80@


# uses these SOUNDs
SOUNDS+=MSM5205@
SOUNDS+=YM2151@
SOUNDS+=YM2610@



#SOUNDS+=OKIM6295@
#SOUNDS+=YM2203@

#SOUNDS+=YM2610B@

#SOUNDS+=DAC@
#SOUNDS+=UPD7759@
#SOUNDS+=YM2413@
#SOUNDS+=SN76496@
#SOUNDS+=YM3812@
#SOUNDS+=YM3438@
#SOUNDS+=YM2612@

#SOUNDS+=X1_010@
#SOUNDS+=CUSTOM@

fgDUMMY=\
	$(OBJ)/machine/rainbow.o $(OBJ)/drivers/rainbow.o \
	$(OBJ)/vidhrdw/rastan.o $(OBJ)/drivers/rastan.o \
	$(OBJ)/drivers/opwolf.o \
	$(OBJ)/vidhrdw/othunder.o $(OBJ)/drivers/othunder.o \
	$(OBJ)/machine/volfied.o $(OBJ)/vidhrdw/volfied.o $(OBJ)/drivers/volfied.o \
	$(OBJ)/vidhrdw/wgp.o $(OBJ)/drivers/wgp.o \
\
	$(OBJ)/vidhrdw/topspeed.o $(OBJ)/drivers/topspeed.o \
	$(OBJ)/vidhrdw/darius.o $(OBJ)/drivers/darius.o \
	$(OBJ)/vidhrdw/ninjaw.o $(OBJ)/drivers/ninjaw.o \
	$(OBJ)/vidhrdw/warriorb.o $(OBJ)/drivers/warriorb.o \
\
	$(OBJ)/vidhrdw/taito_b.o $(OBJ)/drivers/taito_b.o $(OBJ)/machine/mb87078.o \
	$(OBJ)/vidhrdw/taito_z.o $(OBJ)/drivers/taito_z.o \
	$(OBJ)/vidhrdw/slapshot.o $(OBJ)/drivers/slapshot.o $(OBJ)/machine/timekpr.o \


DRVLIBS = \
	$(OBJ)/vidhrdw/taitoic.o $(OBJ)/sndhrdw/taitosnd.o \
	$(OBJ)/machine/bonzeadv.o $(OBJ)/vidhrdw/asuka.o $(OBJ)/drivers/asuka.o \
	$(OBJ)/drivers/taito_x.o $(OBJ)/vidhrdw/taito_s.o $(OBJ)/machine/cchip.o \

#	
#	$(OBJ)/vidhrdw/taito_f2.o $(OBJ)/drivers/taito_f2.o 

#	$(OBJ)/vidhrdw/taito_f3.o $(OBJ)/sndhrdw/taito_f3.o $(OBJ)/drivers/taito_f3.o 
#	$(OBJ)/vidhrdw/groundfx.o $(OBJ)/drivers/groundfx.o 
#	$(OBJ)/vidhrdw/undrfire.o $(OBJ)/drivers/undrfire.o 
#	$(OBJ)/vidhrdw/gunbustr.o $(OBJ)/drivers/gunbustr.o 
#	$(OBJ)/vidhrdw/superchs.o $(OBJ)/drivers/superchs.o 
#	$(OBJ)/drivers/taitojc.o 

#	$(OBJ)/vidhrdw/taito_l.o $(OBJ)/drivers/taito_l.o 


# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
