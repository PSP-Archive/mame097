/* -------------------- included inline section --------------------- */

/* this is #included to generate 8-bit and 16-bit versions */

#define ADJUST_8													\
	int ydir;														\
	if (flipy)														\
	{																\
		INCREMENT_DST(VMODULO * (dstheight-1))						\
		srcdata += (srcheight - dstheight - topskip) * srcmodulo;	\
		ydir = -1;													\
	}																\
	else															\
	{																\
		srcdata += topskip * srcmodulo; 							\
		ydir = 1;													\
	}																\
	if (flipx)														\
	{																\
		INCREMENT_DST(HMODULO * (dstwidth-1))						\
		srcdata += (srcwidth - dstwidth - leftskip);				\
	}																\
	else															\
		srcdata += leftskip;										\
	srcmodulo -= dstwidth;


#define ADJUST_4													\
	int ydir;														\
	if (flipy)														\
	{																\
		INCREMENT_DST(VMODULO * (dstheight-1))						\
		srcdata += (srcheight - dstheight - topskip) * srcmodulo;	\
		ydir = -1;													\
	}																\
	else															\
	{																\
		srcdata += topskip * srcmodulo; 							\
		ydir = 1;													\
	}																\
	if (flipx)														\
	{																\
		INCREMENT_DST(HMODULO * (dstwidth-1))						\
		srcdata += (srcwidth - dstwidth - leftskip)/2;				\
		leftskip = (srcwidth - dstwidth - leftskip) & 1;			\
	}																\
	else															\
	{																\
		srcdata += leftskip/2;										\
		leftskip &= 1;												\
	}																\
	srcmodulo -= (dstwidth+leftskip)/2;



DECLARE_SWAP_RAW_PRI(blockmove_8toN_opaque,(COMMON_ARGS,
		COLOR_ARG),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (dstdata >= end + 8*HMODULO)
			{
				INCREMENT_DST(-8*HMODULO)
				SETPIXELCOLOR(8*HMODULO,LOOKUP(srcdata[0]))
				SETPIXELCOLOR(7*HMODULO,LOOKUP(srcdata[1]))
				SETPIXELCOLOR(6*HMODULO,LOOKUP(srcdata[2]))
				SETPIXELCOLOR(5*HMODULO,LOOKUP(srcdata[3]))
				SETPIXELCOLOR(4*HMODULO,LOOKUP(srcdata[4]))
				SETPIXELCOLOR(3*HMODULO,LOOKUP(srcdata[5]))
				SETPIXELCOLOR(2*HMODULO,LOOKUP(srcdata[6]))
				SETPIXELCOLOR(1*HMODULO,LOOKUP(srcdata[7]))
				srcdata += 8;
			}
			while (dstdata > end)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata))
				srcdata++;
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (dstdata <= end - 8*HMODULO)
			{
				SETPIXELCOLOR(0*HMODULO,LOOKUP(srcdata[0]))
				SETPIXELCOLOR(1*HMODULO,LOOKUP(srcdata[1]))
				SETPIXELCOLOR(2*HMODULO,LOOKUP(srcdata[2]))
				SETPIXELCOLOR(3*HMODULO,LOOKUP(srcdata[3]))
				SETPIXELCOLOR(4*HMODULO,LOOKUP(srcdata[4]))
				SETPIXELCOLOR(5*HMODULO,LOOKUP(srcdata[5]))
				SETPIXELCOLOR(6*HMODULO,LOOKUP(srcdata[6]))
				SETPIXELCOLOR(7*HMODULO,LOOKUP(srcdata[7]))
				srcdata += 8;
				INCREMENT_DST(8*HMODULO)
			}
			while (dstdata < end)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata))
				srcdata++;
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_4toN_opaque,(COMMON_ARGS,
		COLOR_ARG),
{
	ADJUST_4

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			if (leftskip)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata>>4))
				srcdata++;
				INCREMENT_DST(-HMODULO)
			}
			while (dstdata >= end + 8*HMODULO)
			{
				INCREMENT_DST(-8*HMODULO)
				SETPIXELCOLOR(8*HMODULO,LOOKUP(srcdata[0]&0x0f))
				SETPIXELCOLOR(7*HMODULO,LOOKUP(srcdata[0]>>4))
				SETPIXELCOLOR(6*HMODULO,LOOKUP(srcdata[1]&0x0f))
				SETPIXELCOLOR(5*HMODULO,LOOKUP(srcdata[1]>>4))
				SETPIXELCOLOR(4*HMODULO,LOOKUP(srcdata[2]&0x0f))
				SETPIXELCOLOR(3*HMODULO,LOOKUP(srcdata[2]>>4))
				SETPIXELCOLOR(2*HMODULO,LOOKUP(srcdata[3]&0x0f))
				SETPIXELCOLOR(1*HMODULO,LOOKUP(srcdata[3]>>4))
				srcdata += 4;
			}
			while (dstdata > end)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata&0x0f))
				INCREMENT_DST(-HMODULO)
				if (dstdata > end)
				{
					SETPIXELCOLOR(0,LOOKUP(*srcdata>>4))
					srcdata++;
					INCREMENT_DST(-HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			if (leftskip)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata>>4))
				srcdata++;
				INCREMENT_DST(HMODULO)
			}
			while (dstdata <= end - 8*HMODULO)
			{
				SETPIXELCOLOR(0*HMODULO,LOOKUP(srcdata[0]&0x0f))
				SETPIXELCOLOR(1*HMODULO,LOOKUP(srcdata[0]>>4))
				SETPIXELCOLOR(2*HMODULO,LOOKUP(srcdata[1]&0x0f))
				SETPIXELCOLOR(3*HMODULO,LOOKUP(srcdata[1]>>4))
				SETPIXELCOLOR(4*HMODULO,LOOKUP(srcdata[2]&0x0f))
				SETPIXELCOLOR(5*HMODULO,LOOKUP(srcdata[2]>>4))
				SETPIXELCOLOR(6*HMODULO,LOOKUP(srcdata[3]&0x0f))
				SETPIXELCOLOR(7*HMODULO,LOOKUP(srcdata[3]>>4))
				srcdata += 4;
				INCREMENT_DST(8*HMODULO)
			}
			while (dstdata < end)
			{
				SETPIXELCOLOR(0,LOOKUP(*srcdata&0x0f))
				INCREMENT_DST(HMODULO)
				if (dstdata < end)
				{
					SETPIXELCOLOR(0,LOOKUP(*srcdata>>4))
					srcdata++;
					INCREMENT_DST(HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_transpen,(COMMON_ARGS,
		COLOR_ARG,int transpen),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata > end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata >= end + 4*HMODULO)
			{
				UINT32 col4;

				INCREMENT_DST(-4*HMODULO)
				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(4*HMODULO,LOOKUP((col4>>SHIFT0) & 0xff))
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(3*HMODULO,LOOKUP((col4>>SHIFT1) & 0xff))
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,LOOKUP((col4>>SHIFT2) & 0xff))
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(1*HMODULO,LOOKUP((col4>>SHIFT3) & 0xff))
				}
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO);
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata < end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata <= end - 4*HMODULO)
			{
				UINT32 col4;

				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(0*HMODULO,LOOKUP((col4>>SHIFT0) & 0xff))
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(1*HMODULO,LOOKUP((col4>>SHIFT1) & 0xff))
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,LOOKUP((col4>>SHIFT2) & 0xff))
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(3*HMODULO,LOOKUP((col4>>SHIFT3) & 0xff))
				}
				INCREMENT_DST(4*HMODULO)
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO);
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_4toN_transpen,(COMMON_ARGS,
		COLOR_ARG,int transpen),
{
	ADJUST_4

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			int col;

			end = dstdata - dstwidth*HMODULO;
			if (leftskip)
			{
				col = *(srcdata++)>>4;
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}
			while (dstdata > end)
			{
				col = *(srcdata)&0x0f;
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
				if (dstdata > end)
				{
					col = *(srcdata++)>>4;
					if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
					INCREMENT_DST(-HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			int col;

			end = dstdata + dstwidth*HMODULO;
			if (leftskip)
			{
				col = *(srcdata++)>>4;
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}
			while (dstdata < end)
			{
				col = *(srcdata)&0x0f;
				if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
				if (dstdata < end)
				{
					col = *(srcdata++)>>4;
					if (col != transpen) SETPIXELCOLOR(0,LOOKUP(col))
					INCREMENT_DST(HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_transblend,(COMMON_ARGS,
		COLOR_ARG,int transpen),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata > end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,*dstdata | LOOKUP(col))
				INCREMENT_DST(-HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata >= end + 4*HMODULO)
			{
				UINT32 col4;

				INCREMENT_DST(-4*HMODULO);
				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(4*HMODULO,dstdata[4*HMODULO] | LOOKUP((col4>>SHIFT0) & 0xff))
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(3*HMODULO,dstdata[3*HMODULO] | LOOKUP((col4>>SHIFT1) & 0xff))
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,dstdata[2*HMODULO] | LOOKUP((col4>>SHIFT2) & 0xff))
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(1*HMODULO,dstdata[1*HMODULO] | LOOKUP((col4>>SHIFT3) & 0xff))
				}
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,*dstdata | LOOKUP(col))
				INCREMENT_DST(-HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO);
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata < end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,*dstdata | LOOKUP(col))
				INCREMENT_DST(HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata <= end - 4*HMODULO)
			{
				UINT32 col4;

				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(0*HMODULO,dstdata[0*HMODULO] | LOOKUP((col4>>SHIFT0) & 0xff))
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(1*HMODULO,dstdata[1*HMODULO] | LOOKUP((col4>>SHIFT1) & 0xff))
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,dstdata[2*HMODULO] | LOOKUP((col4>>SHIFT2) & 0xff))
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(3*HMODULO,dstdata[3*HMODULO] | LOOKUP((col4>>SHIFT3) & 0xff))
				}
				INCREMENT_DST(4*HMODULO);
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,*dstdata | LOOKUP(col))
				INCREMENT_DST(HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO);
			dstheight--;
		}
	}
})


#define PEN_IS_OPAQUE ((1<<col)&transmask) == 0

DECLARE_SWAP_RAW_PRI(blockmove_8toN_transmask,(COMMON_ARGS,
		COLOR_ARG,int transmask),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;
		UINT32 *sd4;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata > end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata >= end + 4*HMODULO)
			{
				int col;
				UINT32 col4;

				INCREMENT_DST(-4*HMODULO)
				col4 = *(sd4++);
				col = (col4 >> SHIFT0) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(4*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT1) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(3*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT2) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(2*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT3) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(1*HMODULO,LOOKUP(col))
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;
		UINT32 *sd4;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata < end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata <= end - 4*HMODULO)
			{
				int col;
				UINT32 col4;

				col4 = *(sd4++);
				col = (col4 >> SHIFT0) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(0*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT1) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(1*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT2) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(2*HMODULO,LOOKUP(col))
				col = (col4 >> SHIFT3) & 0xff;
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(3*HMODULO,LOOKUP(col))
				INCREMENT_DST(4*HMODULO)
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (PEN_IS_OPAQUE) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_transcolor,(COMMON_ARGS,
		COLOR_ARG,const UINT16 *colortable,int transcolor),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (dstdata > end)
			{
				if (colortable[*srcdata] != transcolor) SETPIXELCOLOR(0,LOOKUP(*srcdata))
				srcdata++;
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (dstdata < end)
			{
				if (colortable[*srcdata] != transcolor) SETPIXELCOLOR(0,LOOKUP(*srcdata))
				srcdata++;
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_4toN_transcolor,(COMMON_ARGS,
		COLOR_ARG,const UINT16 *colortable,int transcolor),
{
	ADJUST_4

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			int col;

			end = dstdata - dstwidth*HMODULO;
			if (leftskip)
			{
				col = *(srcdata++)>>4;
				if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
			}
			while (dstdata > end)
			{
				col = *(srcdata)&0x0f;
				if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(-HMODULO)
				if (dstdata > end)
				{
					col = *(srcdata++)>>4;
					if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
					INCREMENT_DST(-HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			int col;

			end = dstdata + dstwidth*HMODULO;
			if (leftskip)
			{
				col = *(srcdata++)>>4;
				if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
			}
			while (dstdata < end)
			{
				col = *(srcdata)&0x0f;
				if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
				INCREMENT_DST(HMODULO)
				if (dstdata < end)
				{
					col = *(srcdata++)>>4;
					if (colortable[col] != transcolor) SETPIXELCOLOR(0,LOOKUP(col))
					INCREMENT_DST(HMODULO)
				}
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})

#if DEPTH == 32
DECLARE_SWAP_RAW_PRI(blockmove_8toN_pen_table,(COMMON_ARGS,
		COLOR_ARG,int transcolor),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transcolor)
				{
					switch(gfx_drawmode_table[col])
					{
					case DRAWMODE_SOURCE:
						SETPIXELCOLOR(0,LOOKUP(col))
						break;
					case DRAWMODE_SHADOW:
						afterdrawmask = 0;
						SETPIXELCOLOR(0,*dstdata)
						afterdrawmask = 31;
						break;
					}
				}
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transcolor)
				{
					switch(gfx_drawmode_table[col])
					{
					case DRAWMODE_SOURCE:
						SETPIXELCOLOR(0,LOOKUP(col))
						break;
					case DRAWMODE_SHADOW:
						afterdrawmask = 0;
						SETPIXELCOLOR(0,*dstdata)
						afterdrawmask = 31;
						break;
					}
				}
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})
#else
DECLARE_SWAP_RAW_PRI(blockmove_8toN_pen_table,(COMMON_ARGS,
		COLOR_ARG,int transcolor),
{
	int eax = (pdrawgfx_shadow_lowpri) ? 0 : 0x80;

	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transcolor)
				{
					switch(gfx_drawmode_table[col])
					{
					case DRAWMODE_SOURCE:
						SETPIXELCOLOR(0,LOOKUP(col))
						break;
					case DRAWMODE_SHADOW:
						afterdrawmask = eax;
						SETPIXELCOLOR(0,palette_shadow_table[*dstdata])
						afterdrawmask = 31;
						break;
					}
				}
				INCREMENT_DST(-HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO)
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transcolor)
				{
					switch(gfx_drawmode_table[col])
					{
					case DRAWMODE_SOURCE:
						SETPIXELCOLOR(0,LOOKUP(col))
						break;
					case DRAWMODE_SHADOW:
						afterdrawmask = eax;
						SETPIXELCOLOR(0,palette_shadow_table[*dstdata])
						afterdrawmask = 31;
						break;
					}
				}
				INCREMENT_DST(HMODULO)
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO)
			dstheight--;
		}
	}
})
#endif

#if DEPTH >= 16
DECLARE_SWAP_RAW_PRI(blockmove_8toN_alphaone,(COMMON_ARGS,
		COLOR_ARG,int transpen, int alphapen),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;
		UINT32 alphacolor = LOOKUP(alphapen);

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata > end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (col == alphapen)
						SETPIXELCOLOR(0,alpha_blend(*dstdata,alphacolor))
					else
						SETPIXELCOLOR(0,LOOKUP(col))
				}
				INCREMENT_DST(-HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata >= end + 4*HMODULO)
			{
				UINT32 col4;

				INCREMENT_DST(-4*HMODULO);
				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0))
					{
						if (((col4>>SHIFT0) & 0xff) == alphapen)
							SETPIXELCOLOR(4*HMODULO,alpha_blend(dstdata[4*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(4*HMODULO,LOOKUP((col4>>SHIFT0) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT1))
					{
						if (((col4>>SHIFT1) & 0xff) == alphapen)
							SETPIXELCOLOR(3*HMODULO,alpha_blend(dstdata[3*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(3*HMODULO,LOOKUP((col4>>SHIFT1) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT2))
					{
						if (((col4>>SHIFT2) & 0xff) == alphapen)
							SETPIXELCOLOR(2*HMODULO,alpha_blend(dstdata[2*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(2*HMODULO,LOOKUP((col4>>SHIFT2) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT3))
					{
						if (((col4>>SHIFT3) & 0xff) == alphapen)
							SETPIXELCOLOR(1*HMODULO,alpha_blend(dstdata[1*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(1*HMODULO,LOOKUP((col4>>SHIFT3) & 0xff))
					}
				}
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (col == alphapen)
						SETPIXELCOLOR(0,alpha_blend(*dstdata, alphacolor))
					else
						SETPIXELCOLOR(0,LOOKUP(col))
				}
				INCREMENT_DST(-HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO);
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;
		UINT32 alphacolor = LOOKUP(alphapen);

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata < end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (col == alphapen)
						SETPIXELCOLOR(0,alpha_blend(*dstdata, alphacolor))
					else
						SETPIXELCOLOR(0,LOOKUP(col))
				}
				INCREMENT_DST(HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata <= end - 4*HMODULO)
			{
				UINT32 col4;

				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0))
					{
						if (((col4>>SHIFT0) & 0xff) == alphapen)
							SETPIXELCOLOR(0*HMODULO,alpha_blend(dstdata[0*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(0*HMODULO,LOOKUP((col4>>SHIFT0) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT1))
					{
						if (((col4>>SHIFT1) & 0xff) == alphapen)
							SETPIXELCOLOR(1*HMODULO,alpha_blend(dstdata[1*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(1*HMODULO,LOOKUP((col4>>SHIFT1) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT2))
					{
						if (((col4>>SHIFT2) & 0xff) == alphapen)
							SETPIXELCOLOR(2*HMODULO,alpha_blend(dstdata[2*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(2*HMODULO,LOOKUP((col4>>SHIFT2) & 0xff))
					}
					if (xod4 & (0xff<<SHIFT3))
					{
						if (((col4>>SHIFT3) & 0xff) == alphapen)
							SETPIXELCOLOR(3*HMODULO,alpha_blend(dstdata[3*HMODULO], alphacolor))
						else
							SETPIXELCOLOR(3*HMODULO,LOOKUP((col4>>SHIFT3) & 0xff))
					}
				}
				INCREMENT_DST(4*HMODULO);
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (col == alphapen)
						SETPIXELCOLOR(0,alpha_blend(*dstdata, alphacolor))
					else
						SETPIXELCOLOR(0,LOOKUP(col))
				}
				INCREMENT_DST(HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO);
			dstheight--;
		}
	}
})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_alpha,(COMMON_ARGS,
		COLOR_ARG,int transpen),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata > end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,alpha_blend(*dstdata, LOOKUP(col)));
				INCREMENT_DST(-HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata >= end + 4*HMODULO)
			{
				UINT32 col4;

				INCREMENT_DST(-4*HMODULO);
				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(4*HMODULO,alpha_blend(dstdata[4*HMODULO], LOOKUP((col4>>SHIFT0) & 0xff)));
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(3*HMODULO,alpha_blend(dstdata[3*HMODULO], LOOKUP((col4>>SHIFT1) & 0xff)));
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,alpha_blend(dstdata[2*HMODULO], LOOKUP((col4>>SHIFT2) & 0xff)));
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(1*HMODULO,alpha_blend(dstdata[1*HMODULO], LOOKUP((col4>>SHIFT3) & 0xff)));
				}
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata > end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,alpha_blend(*dstdata, LOOKUP(col)));
				INCREMENT_DST(-HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO);
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;
		int trans4;
		UINT32 *sd4;

		trans4 = transpen * 0x01010101;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (((long)srcdata & 3) && dstdata < end)	/* longword align */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,alpha_blend(*dstdata, LOOKUP(col)));
				INCREMENT_DST(HMODULO);
			}
			sd4 = (UINT32 *)srcdata;
			while (dstdata <= end - 4*HMODULO)
			{
				UINT32 col4;

				if ((col4 = *(sd4++)) != trans4)
				{
					UINT32 xod4;

					xod4 = col4 ^ trans4;
					if (xod4 & (0xff<<SHIFT0)) SETPIXELCOLOR(0*HMODULO,alpha_blend(dstdata[0*HMODULO], LOOKUP((col4>>SHIFT0) & 0xff)));
					if (xod4 & (0xff<<SHIFT1)) SETPIXELCOLOR(1*HMODULO,alpha_blend(dstdata[1*HMODULO], LOOKUP((col4>>SHIFT1) & 0xff)));
					if (xod4 & (0xff<<SHIFT2)) SETPIXELCOLOR(2*HMODULO,alpha_blend(dstdata[2*HMODULO], LOOKUP((col4>>SHIFT2) & 0xff)));
					if (xod4 & (0xff<<SHIFT3)) SETPIXELCOLOR(3*HMODULO,alpha_blend(dstdata[3*HMODULO], LOOKUP((col4>>SHIFT3) & 0xff)));
				}
				INCREMENT_DST(4*HMODULO);
			}
			srcdata = (UINT8 *)sd4;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen) SETPIXELCOLOR(0,alpha_blend(*dstdata, LOOKUP(col)));
				INCREMENT_DST(HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO);
			dstheight--;
		}
	}
})

/* pjp 02/06/02 */
DECLARE_SWAP_RAW_PRI(blockmove_8toN_alpharange,(COMMON_ARGS,
		COLOR_ARG,int transpen),
{
	ADJUST_8

	if (flipx)
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata - dstwidth*HMODULO;
			while (dstdata > end) /* Note that I'm missing the optimisations present in the other alpha functions */
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (gfx_alpharange_table[col] == 0xff)
						SETPIXELCOLOR(0,LOOKUP(col))
					else
						SETPIXELCOLOR(0,alpha_blend_r(*dstdata,LOOKUP(col),gfx_alpharange_table[col]))
				}
				INCREMENT_DST(-HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO + dstwidth*HMODULO);
			dstheight--;
		}
	}
	else
	{
		DATA_TYPE *end;

		while (dstheight)
		{
			end = dstdata + dstwidth*HMODULO;
			while (dstdata < end)
			{
				int col;

				col = *(srcdata++);
				if (col != transpen)
				{
					if (gfx_alpharange_table[col] == 0xff)
						SETPIXELCOLOR(0,LOOKUP(col))
					else
						SETPIXELCOLOR(0,alpha_blend_r(*dstdata,LOOKUP(col),gfx_alpharange_table[col]))
				}
				INCREMENT_DST(HMODULO);
			}

			srcdata += srcmodulo;
			INCREMENT_DST(ydir*VMODULO - dstwidth*HMODULO);
			dstheight--;
		}
	}
})

#else

DECLARE_SWAP_RAW_PRI(blockmove_8toN_alphaone,(COMMON_ARGS,
		COLOR_ARG,int transpen, int alphapen),{})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_alpha,(COMMON_ARGS,
		COLOR_ARG,int transpen),{})

DECLARE_SWAP_RAW_PRI(blockmove_8toN_alpharange,(COMMON_ARGS,
		COLOR_ARG,int transpen),{})

#endif

DECLARE(blockmove_NtoN_opaque_noremap,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo),
{
	while (srcheight)
	{
		memcpy(dstdata,srcdata,srcwidth * sizeof(DATA_TYPE));
		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_opaque_noremap_flipx,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo),
{
	DATA_TYPE *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			srcdata -= 8;
			dstdata[0] = srcdata[8];
			dstdata[1] = srcdata[7];
			dstdata[2] = srcdata[6];
			dstdata[3] = srcdata[5];
			dstdata[4] = srcdata[4];
			dstdata[5] = srcdata[3];
			dstdata[6] = srcdata[2];
			dstdata[7] = srcdata[1];
			dstdata += 8;
		}
		while (dstdata < end)
			*(dstdata++) = *(srcdata--);

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_opaque_remap,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		const pen_t *paldata),
{
	DATA_TYPE *end;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			dstdata[0] = paldata[srcdata[0]];
			dstdata[1] = paldata[srcdata[1]];
			dstdata[2] = paldata[srcdata[2]];
			dstdata[3] = paldata[srcdata[3]];
			dstdata[4] = paldata[srcdata[4]];
			dstdata[5] = paldata[srcdata[5]];
			dstdata[6] = paldata[srcdata[6]];
			dstdata[7] = paldata[srcdata[7]];
			dstdata += 8;
			srcdata += 8;
		}
		while (dstdata < end)
			*(dstdata++) = paldata[*(srcdata++)];

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_opaque_remap_flipx,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		const pen_t *paldata),
{
	DATA_TYPE *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			srcdata -= 8;
			dstdata[0] = paldata[srcdata[8]];
			dstdata[1] = paldata[srcdata[7]];
			dstdata[2] = paldata[srcdata[6]];
			dstdata[3] = paldata[srcdata[5]];
			dstdata[4] = paldata[srcdata[4]];
			dstdata[5] = paldata[srcdata[3]];
			dstdata[6] = paldata[srcdata[2]];
			dstdata[7] = paldata[srcdata[1]];
			dstdata += 8;
		}
		while (dstdata < end)
			*(dstdata++) = paldata[*(srcdata--)];

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})


DECLARE(blockmove_NtoN_blend_noremap,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		int srcshift),
{
	DATA_TYPE *end;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			dstdata[0] |= srcdata[0] << srcshift;
			dstdata[1] |= srcdata[1] << srcshift;
			dstdata[2] |= srcdata[2] << srcshift;
			dstdata[3] |= srcdata[3] << srcshift;
			dstdata[4] |= srcdata[4] << srcshift;
			dstdata[5] |= srcdata[5] << srcshift;
			dstdata[6] |= srcdata[6] << srcshift;
			dstdata[7] |= srcdata[7] << srcshift;
			dstdata += 8;
			srcdata += 8;
		}
		while (dstdata < end)
			*(dstdata++) |= *(srcdata++) << srcshift;

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_blend_noremap_flipx,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		int srcshift),
{
	DATA_TYPE *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			srcdata -= 8;
			dstdata[0] |= srcdata[8] << srcshift;
			dstdata[1] |= srcdata[7] << srcshift;
			dstdata[2] |= srcdata[6] << srcshift;
			dstdata[3] |= srcdata[5] << srcshift;
			dstdata[4] |= srcdata[4] << srcshift;
			dstdata[5] |= srcdata[3] << srcshift;
			dstdata[6] |= srcdata[2] << srcshift;
			dstdata[7] |= srcdata[1] << srcshift;
			dstdata += 8;
		}
		while (dstdata < end)
			*(dstdata++) |= *(srcdata--) << srcshift;

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_blend_remap,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		const pen_t *paldata,int srcshift),
{
	DATA_TYPE *end;

	srcmodulo -= srcwidth;
	dstmodulo -= srcwidth;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			dstdata[0] = paldata[dstdata[0] | (srcdata[0] << srcshift)];
			dstdata[1] = paldata[dstdata[1] | (srcdata[1] << srcshift)];
			dstdata[2] = paldata[dstdata[2] | (srcdata[2] << srcshift)];
			dstdata[3] = paldata[dstdata[3] | (srcdata[3] << srcshift)];
			dstdata[4] = paldata[dstdata[4] | (srcdata[4] << srcshift)];
			dstdata[5] = paldata[dstdata[5] | (srcdata[5] << srcshift)];
			dstdata[6] = paldata[dstdata[6] | (srcdata[6] << srcshift)];
			dstdata[7] = paldata[dstdata[7] | (srcdata[7] << srcshift)];
			dstdata += 8;
			srcdata += 8;
		}
		while (dstdata < end)
		{
			*dstdata = paldata[*dstdata | (*(srcdata++) << srcshift)];
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})

DECLARE(blockmove_NtoN_blend_remap_flipx,(
		const DATA_TYPE *srcdata,int srcwidth,int srcheight,int srcmodulo,
		DATA_TYPE *dstdata,int dstmodulo,
		const pen_t *paldata,int srcshift),
{
	DATA_TYPE *end;

	srcmodulo += srcwidth;
	dstmodulo -= srcwidth;
	//srcdata += srcwidth-1;

	while (srcheight)
	{
		end = dstdata + srcwidth;
		while (dstdata <= end - 8)
		{
			srcdata -= 8;
			dstdata[0] = paldata[dstdata[0] | (srcdata[8] << srcshift)];
			dstdata[1] = paldata[dstdata[1] | (srcdata[7] << srcshift)];
			dstdata[2] = paldata[dstdata[2] | (srcdata[6] << srcshift)];
			dstdata[3] = paldata[dstdata[3] | (srcdata[5] << srcshift)];
			dstdata[4] = paldata[dstdata[4] | (srcdata[4] << srcshift)];
			dstdata[5] = paldata[dstdata[5] | (srcdata[3] << srcshift)];
			dstdata[6] = paldata[dstdata[6] | (srcdata[2] << srcshift)];
			dstdata[7] = paldata[dstdata[7] | (srcdata[1] << srcshift)];
			dstdata += 8;
		}
		while (dstdata < end)
		{
			*dstdata = paldata[*dstdata | (*(srcdata--) << srcshift)];
			dstdata++;
		}

		srcdata += srcmodulo;
		dstdata += dstmodulo;
		srcheight--;
	}
})





DECLARE(drawgfx_core,(
		struct mame_bitmap *dest,const struct GfxElement *gfx,
		unsigned int code,unsigned int color,int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color,
		struct mame_bitmap *pri_buffer,UINT32 pri_mask),
{
	int ox;
	int oy;
	int ex;
	int ey;


	/* check bounds */
	ox = sx;
	oy = sy;

	ex = sx + gfx->width-1;
	if (sx < 0) sx = 0;
	if (clip && sx < clip->min_x) sx = clip->min_x;
	if (ex >= dest->width) ex = dest->width-1;
	if (clip && ex > clip->max_x) ex = clip->max_x;
	if (sx > ex) return;

	ey = sy + gfx->height-1;
	if (sy < 0) sy = 0;
	if (clip && sy < clip->min_y) sy = clip->min_y;
	if (ey >= dest->height) ey = dest->height-1;
	if (clip && ey > clip->max_y) ey = clip->max_y;
	if (sy > ey) return;

	{
		UINT8 *sd = gfx->gfxdata + code * gfx->char_modulo; 	/* source data */
		int sw = gfx->width;									/* source width */
		int sh = gfx->height;									/* source height */
		int sm = gfx->line_modulo;								/* source modulo */
		int ls = sx-ox; 										/* left skip */
		int ts = sy-oy; 										/* top skip */
		DATA_TYPE *dd = ((DATA_TYPE *)dest->line[sy]) + sx; 	/* dest data */
		int dw = ex-sx+1;										/* dest width */
		int dh = ey-sy+1;										/* dest height */
		int dm = ((DATA_TYPE *)dest->line[1])-((DATA_TYPE *)dest->line[0]); /* dest modulo */
		const pen_t *paldata = &gfx->colortable[gfx->color_granularity * color];
		UINT8 *pribuf = (pri_buffer) ? ((UINT8 *)pri_buffer->line[sy]) + sx : NULL;

		/* optimizations for 1:1 mapping */
		if (Machine->drv->color_table_len == 0 &&
			dest->depth == 16 &&
			paldata >= Machine->remapped_colortable &&
			paldata < Machine->remapped_colortable + Machine->drv->total_colors)
		{
			switch (transparency)
			{
				case TRANSPARENCY_NONE:
					transparency = TRANSPARENCY_NONE_RAW;
					color = paldata - Machine->remapped_colortable;
					break;
				case TRANSPARENCY_PEN:
					transparency = TRANSPARENCY_PEN_RAW;
					color = paldata - Machine->remapped_colortable;
					break;
				case TRANSPARENCY_PENS:
					transparency = TRANSPARENCY_PENS_RAW;
					color = paldata - Machine->remapped_colortable;
					break;
				case TRANSPARENCY_PEN_TABLE:
					transparency = TRANSPARENCY_PEN_TABLE_RAW;
					color = paldata - Machine->remapped_colortable;
					break;
			}
		}

		switch (transparency)
		{
			case TRANSPARENCY_NONE:
				if (gfx->flags & GFX_PACKED)
				{
					if (pribuf)
						BLOCKMOVEPRI(4toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask));
					else
						BLOCKMOVELU(4toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata));
				}
				else
				{
					if (pribuf)
						BLOCKMOVEPRI(8toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask));
					else
						BLOCKMOVELU(8toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata));
				}
				break;

			case TRANSPARENCY_NONE_RAW:
				if (gfx->flags & GFX_PACKED)
				{
					if (pribuf)
						BLOCKMOVERAWPRI(4toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask));
					else
						BLOCKMOVERAW(4toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color));
				}
				else
				{
					if (pribuf)
						BLOCKMOVERAWPRI(8toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask));
					else
						BLOCKMOVERAW(8toN_opaque,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color));
				}
				break;

			case TRANSPARENCY_PEN:
				if (gfx->flags & GFX_PACKED)
				{
					if (pribuf)
						BLOCKMOVEPRI(4toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
					else
						BLOCKMOVELU(4toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				}
				else
				{
					if (pribuf)
						BLOCKMOVEPRI(8toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
					else
						BLOCKMOVELU(8toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				}
				break;

			case TRANSPARENCY_PEN_RAW:
				if (gfx->flags & GFX_PACKED)
				{
					if (pribuf)
						BLOCKMOVERAWPRI(4toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask,transparent_color));
					else
						BLOCKMOVERAW(4toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,transparent_color));
				}
				else
				{
					if (pribuf)
						BLOCKMOVERAWPRI(8toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask,transparent_color));
					else
						BLOCKMOVERAW(8toN_transpen,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,transparent_color));
				}
				break;

			case TRANSPARENCY_PENS:
				if (pribuf)
					BLOCKMOVEPRI(8toN_transmask,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVELU(8toN_transmask,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				break;

			case TRANSPARENCY_PENS_RAW:
				if (pribuf)
					BLOCKMOVERAWPRI(8toN_transmask,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVERAW(8toN_transmask,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,transparent_color));
				break;

			case TRANSPARENCY_COLOR:
				if (gfx->flags & GFX_PACKED)
				{
					if (pribuf)
						BLOCKMOVEPRI(4toN_transcolor,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,Machine->game_colortable + (paldata - Machine->remapped_colortable),transparent_color));
					else
						BLOCKMOVELU(4toN_transcolor,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,Machine->game_colortable + (paldata - Machine->remapped_colortable),transparent_color));
				}
				else
				{
					if (pribuf)
						BLOCKMOVEPRI(8toN_transcolor,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,Machine->game_colortable + (paldata - Machine->remapped_colortable),transparent_color));
					else
						BLOCKMOVELU(8toN_transcolor,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,Machine->game_colortable + (paldata - Machine->remapped_colortable),transparent_color));
				}
				break;

			case TRANSPARENCY_PEN_TABLE:
				if (pribuf)
					BLOCKMOVEPRI(8toN_pen_table,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVELU(8toN_pen_table,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				break;

			case TRANSPARENCY_PEN_TABLE_RAW:
				if (pribuf)
					BLOCKMOVERAWPRI(8toN_pen_table,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVERAW(8toN_pen_table,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,transparent_color));
				break;

			case TRANSPARENCY_BLEND_RAW:
				if (pribuf)
					BLOCKMOVERAWPRI(8toN_transblend,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVERAW(8toN_transblend,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,color,transparent_color));
				break;

			case TRANSPARENCY_ALPHAONE:
				if (pribuf)
					BLOCKMOVEPRI(8toN_alphaone,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color & 0xff, (transparent_color>>8) & 0xff));
				else
					BLOCKMOVELU(8toN_alphaone,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color & 0xff, (transparent_color>>8) & 0xff));
				break;

			case TRANSPARENCY_ALPHA:
				if (pribuf)
					BLOCKMOVEPRI(8toN_alpha,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVELU(8toN_alpha,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				break;

			case TRANSPARENCY_ALPHARANGE:
				if (pribuf)
					BLOCKMOVEPRI(8toN_alpharange,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,pribuf,pri_mask,transparent_color));
				else
					BLOCKMOVELU(8toN_alpharange,(sd,sw,sh,sm,ls,ts,flipx,flipy,dd,dw,dh,dm,paldata,transparent_color));
				break;

			default:
				if (pribuf)
					usrintf_showmessage("pdrawgfx pen mode not supported");
				else
					usrintf_showmessage("drawgfx pen mode not supported");
				break;
		}
	}
})

DECLARE(copybitmap_core,(
		struct mame_bitmap *dest,struct mame_bitmap *src,
		int flipx,int flipy,int sx,int sy,
		const struct rectangle *clip,int transparency,int transparent_color),
{
	int ox;
	int oy;
	int ex;
	int ey;


	/* check bounds */
	ox = sx;
	oy = sy;

	ex = sx + src->width-1;
	if (sx < 0) sx = 0;
	if (clip && sx < clip->min_x) sx = clip->min_x;
	if (ex >= dest->width) ex = dest->width-1;
	if (clip && ex > clip->max_x) ex = clip->max_x;
	if (sx > ex) return;

	ey = sy + src->height-1;
	if (sy < 0) sy = 0;
	if (clip && sy < clip->min_y) sy = clip->min_y;
	if (ey >= dest->height) ey = dest->height-1;
	if (clip && ey > clip->max_y) ey = clip->max_y;
	if (sy > ey) return;

	{
		DATA_TYPE *sd = ((DATA_TYPE *)src->line[0]);							/* source data */
		int sw = ex-sx+1;														/* source width */
		int sh = ey-sy+1;														/* source height */
		int sm = ((DATA_TYPE *)src->line[1])-((DATA_TYPE *)src->line[0]);		/* source modulo */
		DATA_TYPE *dd = ((DATA_TYPE *)dest->line[sy]) + sx; 					/* dest data */
		int dm = ((DATA_TYPE *)dest->line[1])-((DATA_TYPE *)dest->line[0]); 	/* dest modulo */

		if (flipx)
		{
			//if ((sx-ox) == 0) sd += gfx->width - sw;
			sd += src->width -1 -(sx-ox);
		}
		else
			sd += (sx-ox);

		if (flipy)
		{
			//if ((sy-oy) == 0) sd += sm * (gfx->height - sh);
			//dd += dm * (sh - 1);
			//dm = -dm;
			sd += sm * (src->height -1 -(sy-oy));
			sm = -sm;
		}
		else
			sd += sm * (sy-oy);

		switch (transparency)
		{
			case TRANSPARENCY_NONE:
				BLOCKMOVE(NtoN_opaque_remap,flipx,(sd,sw,sh,sm,dd,dm,Machine->pens));
				break;

			case TRANSPARENCY_NONE_RAW:
				BLOCKMOVE(NtoN_opaque_noremap,flipx,(sd,sw,sh,sm,dd,dm));
				break;

			case TRANSPARENCY_PEN_RAW:
				BLOCKMOVE(NtoN_transpen_noremap,flipx,(sd,sw,sh,sm,dd,dm,transparent_color));
				break;

			case TRANSPARENCY_BLEND:
				BLOCKMOVE(NtoN_blend_remap,flipx,(sd,sw,sh,sm,dd,dm,Machine->pens,transparent_color));
				break;

			case TRANSPARENCY_BLEND_RAW:
				BLOCKMOVE(NtoN_blend_noremap,flipx,(sd,sw,sh,sm,dd,dm,transparent_color));
				break;

			default:
				usrintf_showmessage("copybitmap pen mode not supported");
				break;
		}
	}
})

#if (0==PSP_FORCE_NO_ROZ)
DECLARE(copyrozbitmap_core,(struct mame_bitmap *bitmap,struct mame_bitmap *srcbitmap,
		UINT32 startx,UINT32 starty,int incxx,int incxy,int incyx,int incyy,int wraparound,
		const struct rectangle *clip,int transparency,int transparent_color,UINT32 priority),
{
	UINT32 cx;
	UINT32 cy;
	int x;
	int sx;
	int sy;
	int ex;
	int ey;
	const int xmask = srcbitmap->width-1;
	const int ymask = srcbitmap->height-1;
	const int widthshifted = srcbitmap->width << 16;
	const int heightshifted = srcbitmap->height << 16;
	DATA_TYPE *dest;

	if (clip)
	{
		startx += clip->min_x * incxx + clip->min_y * incyx;
		starty += clip->min_x * incxy + clip->min_y * incyy;

		sx = clip->min_x;
		sy = clip->min_y;
		ex = clip->max_x;
		ey = clip->max_y;
	}
	else
	{
		sx = 0;
		sy = 0;
		ex = bitmap->width-1;
		ey = bitmap->height-1;
	}


	if (incxy == 0 && incyx == 0 && !wraparound)
	{
		/* optimized loop for the not rotated case */

		if (incxx == 0x10000)
		{
			/* optimized loop for the not zoomed case */

			/* startx is unsigned */
			startx = ((INT32)startx) >> 16;

			if (startx >= srcbitmap->width)
			{
				sx += -startx;
				startx = 0;
			}

			if (sx <= ex)
			{
				while (sy <= ey)
				{
					if (starty < heightshifted)
					{
						x = sx;
						cx = startx;
						cy = starty >> 16;
						dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
						if (priority)
						{
							UINT8 *pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
							DATA_TYPE *src = (DATA_TYPE *)srcbitmap->line[cy];

							while (x <= ex && cx < srcbitmap->width)
							{
								int c = src[cx];

								if (c != transparent_color)
								{
									*dest = c;
									*pri |= priority;
								}

								cx++;
								x++;
								dest++;
								pri++;
							}
						}
						else
						{
							DATA_TYPE *src = (DATA_TYPE *)srcbitmap->line[cy];

							while (x <= ex && cx < srcbitmap->width)
							{
								int c = src[cx];

								if (c != transparent_color)
									*dest = c;

								cx++;
								x++;
								dest++;
							}
						}
					}
					starty += incyy;
					sy++;
				}
			}
		}
		else
		{
			while (startx >= widthshifted && sx <= ex)
			{
				startx += incxx;
				sx++;
			}

			if (sx <= ex)
			{
				while (sy <= ey)
				{
					if (starty < heightshifted)
					{
						x = sx;
						cx = startx;
						cy = starty >> 16;
						dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
						if (priority)
						{
							UINT8 *pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;
							DATA_TYPE *src = (DATA_TYPE *)srcbitmap->line[cy];

							while (x <= ex && cx < widthshifted)
							{
								int c = src[cx >> 16];

								if (c != transparent_color)
								{
									*dest = c;
									*pri |= priority;
								}

								cx += incxx;
								x++;
								dest++;
								pri++;
							}
						}
						else
						{
							DATA_TYPE *src = (DATA_TYPE *)srcbitmap->line[cy];

							while (x <= ex && cx < widthshifted)
							{
								int c = src[cx >> 16];

								if (c != transparent_color)
									*dest = c;

								cx += incxx;
								x++;
								dest++;
							}
						}
					}
					starty += incyy;
					sy++;
				}
			}
		}
	}
	else
	{
		if (wraparound)
		{
			/* plot with wraparound */
			while (sy <= ey)
			{
				x = sx;
				cx = startx;
				cy = starty;
				dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
				if (priority)
				{
					UINT8 *pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;

					while (x <= ex)
					{
						int c = ((DATA_TYPE *)srcbitmap->line[(cy >> 16) & ymask])[(cx >> 16) & xmask];

						if (c != transparent_color)
						{
							*dest = c;
							*pri |= priority;
						}

						cx += incxx;
						cy += incxy;
						x++;
						dest++;
						pri++;
					}
				}
				else
				{
					while (x <= ex)
					{
						int c = ((DATA_TYPE *)srcbitmap->line[(cy >> 16) & ymask])[(cx >> 16) & xmask];

						if (c != transparent_color)
							*dest = c;

						cx += incxx;
						cy += incxy;
						x++;
						dest++;
					}
				}
				startx += incyx;
				starty += incyy;
				sy++;
			}
		}
		else
		{
			while (sy <= ey)
			{
				x = sx;
				cx = startx;
				cy = starty;
				dest = ((DATA_TYPE *)bitmap->line[sy]) + sx;
				if (priority)
				{
					UINT8 *pri = ((UINT8 *)priority_bitmap->line[sy]) + sx;

					while (x <= ex)
					{
						if (cx < widthshifted && cy < heightshifted)
						{
							int c = ((DATA_TYPE *)srcbitmap->line[cy >> 16])[cx >> 16];

							if (c != transparent_color)
							{
								*dest = c;
								*pri |= priority;
							}
						}

						cx += incxx;
						cy += incxy;
						x++;
						dest++;
						pri++;
					}
				}
				else
				{
					while (x <= ex)
					{
						if (cx < widthshifted && cy < heightshifted)
						{
							int c = ((DATA_TYPE *)srcbitmap->line[cy >> 16])[cx >> 16];

							if (c != transparent_color)
								*dest = c;
						}

						cx += incxx;
						cy += incxy;
						x++;
						dest++;
					}
				}
				startx += incyx;
				starty += incyy;
				sy++;
			}
		}
	}
})
#endif //(0==PSP_FORCE_NO_ROZ)


DECLAREG(draw_scanline, (
		struct mame_bitmap *bitmap,int x,int y,int length,
		const DATA_TYPE *src,pen_t *pens,int transparent_pen),
{
	/* 8bpp destination */
	if (bitmap->depth == 8)
	{
		int dy = bitmap->rowpixels;
		UINT8 *dst = (UINT8 *)bitmap->base + y * dy + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = pens[*src++];
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = pens[spixel];
					dst += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = *src++;
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = spixel;
					dst += xadv;
				}
		}
	}

	/* 16bpp destination */
	else if(bitmap->depth == 15 || bitmap->depth == 16)
	{
		int dy = bitmap->rowpixels;
		UINT16 *dst = (UINT16 *)bitmap->base + y * dy + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = pens[*src++];
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = pens[spixel];
					dst += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = *src++;
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = spixel;
					dst += xadv;
				}
		}
	}
/*#if (0==PSP_FORCE16)*/
	/* 32bpp destination */
	else
	{
		int dy = bitmap->rowpixels;
		UINT32 *dst = (UINT32 *)bitmap->base + y * dy + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = pens[*src++];
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = pens[spixel];
					dst += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dst = *src++;
					dst += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
						*dst = spixel;
					dst += xadv;
				}
		}
	}
/*#endif //(PSP_FORCE16)*/
})

DECLAREG(pdraw_scanline, (
		struct mame_bitmap *bitmap,int x,int y,int length,
		const DATA_TYPE *src,pen_t *pens,int transparent_pen,int pri),
{
	/* 8bpp destination */
	if (bitmap->depth == 8)
	{
		int dy = bitmap->rowpixels;
		int dyp = priority_bitmap->rowpixels;
		UINT8 *dsti = (UINT8 *)bitmap->base + y * dy + x;
		UINT8 *dstp = (UINT8 *)priority_bitmap->base + y * dyp + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = pens[*src++];
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = pens[spixel];
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = *src++;
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = spixel;
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}
	}

	/* 16bpp destination */
	else if(bitmap->depth == 15 || bitmap->depth == 16)
	{
		int dy = bitmap->rowpixels;
		int dyp = priority_bitmap->rowpixels;
		UINT16 *dsti = (UINT16 *)bitmap->base + y * dy + x;
		UINT8 *dstp = (UINT8 *)priority_bitmap->base + y * dyp + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = pens[*src++];
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = pens[spixel];
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = *src++;
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = spixel;
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}
	}
/*#if (0==PSP_FORCE16)*/
	/* 32bpp destination */
	else
	{
		int dy = bitmap->rowpixels;
		int dyp = priority_bitmap->rowpixels;
		UINT32 *dsti = (UINT32 *)bitmap->base + y * dy + x;
		UINT8 *dstp = (UINT8 *)priority_bitmap->base + y * dyp + x;
		int xadv = 1;

		/* with pen lookups */
		if (pens)
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = pens[*src++];
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = pens[spixel];
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}

		/* without pen lookups */
		else
		{
			if (transparent_pen == -1)
				while (length--)
				{
					*dsti = *src++;
					*dstp = pri;
					dsti += xadv;
					dstp += xadv;
				}
			else
				while (length--)
				{
					UINT32 spixel = *src++;
					if (spixel != transparent_pen)
					{
						*dsti = spixel;
						*dstp = pri;
					}
					dsti += xadv;
					dstp += xadv;
				}
		}
	}
/*#endif //(PSP_FORCE16)*/
}
)

DECLAREG(extract_scanline, (
		struct mame_bitmap *bitmap,int x,int y,int length,
		DATA_TYPE *dst),
{
	/* 8bpp destination */
	if (bitmap->depth == 8)
	{
		int dy = bitmap->rowpixels;
		UINT8 *src = (UINT8 *)bitmap->base + y * dy + x;
		int xadv = 1;

		while (length--)
		{
			*dst++ = *src;
			src += xadv;
		}
	}

	/* 16bpp destination */
	else if(bitmap->depth == 15 || bitmap->depth == 16)
	{
		int dy = bitmap->rowpixels;
		UINT16 *src = (UINT16 *)bitmap->base + y * dy + x;
		int xadv = 1;

		while (length--)
		{
			*dst++ = *src;
			src += xadv;
		}
	}
/*#if (0==PSP_FORCE16)*/
	/* 32bpp destination */
	else
	{
		int dy = bitmap->rowpixels;
		UINT32 *src = (UINT32 *)bitmap->base + y * dy + x;
		int xadv = 1;

		while (length--)
		{
			*dst++ = *src;
			src += xadv;
		}
	}
/*#endif //(PSP_FORCE16)*/
})
