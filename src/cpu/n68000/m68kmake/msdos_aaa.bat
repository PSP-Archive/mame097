@echo off
rem このバッチは、コマンドプロンプト（ＭＳ－ＤＯＳ）で動かした。
rem This executes the bat files, used to under ms-dos on windows98SE.

del m68kopac.c
del m68kopdm.c
del m68kopnz.c
del m68kops.c
del m68kops.h
m68kmake

move m68kopac.c .\..\m68kopac.c
move m68kopdm.c .\..\m68kopdm.c
move m68kopnz.c .\..\m68kopnz.c
move m68kops.c  .\..\m68kops.c 
move m68kops.h  .\..\m68kops.h 

del msdos_aaa.pif
