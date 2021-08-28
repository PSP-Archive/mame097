
#namco system NA only

# a tiny compile is without Neogeo games

COREDEFS += -DNANCO_NA=1

COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="\
driver_bkrtmaq,\
driver_cgangpzl,\
driver_cgangpzj,\
driver_exvania,\
driver_fghtatck,\
driver_fa,\
driver_swcourt,\
driver_swcourtj,\
driver_emeraldj,\
driver_emerldja,\
driver_tinklpit,\
driver_knckhead,\
driver_knckhedj,\
driver_emeralda,\
driver_numanath,\
driver_numanatj,\
driver_quiztou,\
driver_xday,\
driver_xday2"

COREDEFS += -DTINY_POINTER="\
&driver_bkrtmaq,\
&driver_cgangpzl,\
&driver_cgangpzj,\
&driver_exvania,\
&driver_fghtatck,\
&driver_fa,\
&driver_swcourt,\
&driver_swcourtj,\
&driver_emeraldj,\
&driver_emerldja,\
&driver_tinklpit,\
&driver_knckhead,\
&driver_knckhedj,\
&driver_emeralda,\
&driver_numanath,\
&driver_numanatj,\
&driver_quiztou,\
&driver_xday,\
&driver_xday2"

# uses these CPUs
#CPUS+=M68000@
CPUS+=N68000@


# uses these SOUNDs
SOUNDS+=NAMCONA@

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


#	$(OBJ)/drivers/namcoic.o  

DRVLIBS = \
	$(OBJ)/vidhrdw/namcona1.o $(OBJ)/drivers/namcona1.o 

#	$(OBJ)/machine/random.o 



# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
