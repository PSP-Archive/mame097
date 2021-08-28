
# namco system 86

# a tiny compile is without Neogeo games
COREDEFS += -DTINY_COMPILE=1
COREDEFS += -DTINY_NAME="\
driver_skykiddx,\
driver_skykiddo,\
driver_hopmappy,\
driver_roishtar,\
driver_genpeitd,\
driver_rthunder,\
driver_rthundro,\
driver_wndrmomo"

COREDEFS += -DTINY_POINTER="\
&driver_skykiddx,\
&driver_skykiddo,\
&driver_hopmappy,\
&driver_roishtar,\
&driver_genpeitd,\
&driver_rthunder,\
&driver_rthundro,\
&driver_wndrmomo"


# uses these CPUs
CPUS+=M6809Z@
CPUS+=HD63701@


# uses these SOUNDs
SOUNDS+=YM2151@
SOUNDS+=NAMCO_CUS30@
SOUNDS+=NAMCO_63701X@


DRVLIBS = \
$(OBJ)/vidhrdw/namcos86.o \
$(OBJ)/drivers/namcos86.o \
$(OBJ)/sound/namco.o \
$(OBJ)/sound/n63701x.o \


# MAME specific core objs
COREOBJS += $(OBJ)/tiny.o $(OBJ)/cheat.o
