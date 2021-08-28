#include "m68kcpu.h"

/* ======================================================================== */
/* ==== AUTO GENERATED ===== INSTRUCTION HANDLERS ========================= */
/* ======================================================================== */


static void m68k_op_1010(void)
{
    m68ki_exception_1010();
}


static void m68k_op_1111(void)
{
    m68ki_exception_1111();
}


static void m68k_op_abcd_8_rr(void)
{
    uint* r_dst = &DX;
    uint src = DY;
    uint dst = *r_dst;
    uint res = LOW_NIBBLE(src) + LOW_NIBBLE(dst) + XFLAG_AS_1();

    FLAG_V = ~res; /* Undefined V behavior */

    if(res > 9)
        res += 6;
    res += HIGH_NIBBLE(src) + HIGH_NIBBLE(dst);
    FLAG_X = FLAG_C = (res > 0x99) << 8;
    if(FLAG_C)
        res -= 0xa0;

    FLAG_V &= res; /* Undefined V behavior part II */
    FLAG_N = NFLAG_8(res); /* Undefined N behavior */

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;
}


static void m68k_op_abcd_8_mm_ax7(void)
{
    uint src = OPER_AY_PD_8();
    uint ea  = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = LOW_NIBBLE(src) + LOW_NIBBLE(dst) + XFLAG_AS_1();

    FLAG_V = ~res; /* Undefined V behavior */

    if(res > 9)
        res += 6;
    res += HIGH_NIBBLE(src) + HIGH_NIBBLE(dst);
    FLAG_X = FLAG_C = (res > 0x99) << 8;
    if(FLAG_C)
        res -= 0xa0;

    FLAG_V &= res; /* Undefined V behavior part II */
    FLAG_N = NFLAG_8(res); /* Undefined N behavior */

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_abcd_8_mm_ay7(void)
{
    uint src = OPER_A7_PD_8();
    uint ea  = EA_AX_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = LOW_NIBBLE(src) + LOW_NIBBLE(dst) + XFLAG_AS_1();

    FLAG_V = ~res; /* Undefined V behavior */

    if(res > 9)
        res += 6;
    res += HIGH_NIBBLE(src) + HIGH_NIBBLE(dst);
    FLAG_X = FLAG_C = (res > 0x99) << 8;
    if(FLAG_C)
        res -= 0xa0;

    FLAG_V &= res; /* Undefined V behavior part II */
    FLAG_N = NFLAG_8(res); /* Undefined N behavior */

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_abcd_8_mm_axy7(void)
{
    uint src = OPER_A7_PD_8();
    uint ea  = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = LOW_NIBBLE(src) + LOW_NIBBLE(dst) + XFLAG_AS_1();

    FLAG_V = ~res; /* Undefined V behavior */

    if(res > 9)
        res += 6;
    res += HIGH_NIBBLE(src) + HIGH_NIBBLE(dst);
    FLAG_X = FLAG_C = (res > 0x99) << 8;
    if(FLAG_C)
        res -= 0xa0;

    FLAG_V &= res; /* Undefined V behavior part II */
    FLAG_N = NFLAG_8(res); /* Undefined N behavior */

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_abcd_8_mm(void)
{
    uint src = OPER_AY_PD_8();
    uint ea  = EA_AX_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = LOW_NIBBLE(src) + LOW_NIBBLE(dst) + XFLAG_AS_1();

    FLAG_V = ~res; /* Undefined V behavior */

    if(res > 9)
        res += 6;
    res += HIGH_NIBBLE(src) + HIGH_NIBBLE(dst);
    FLAG_X = FLAG_C = (res > 0x99) << 8;
    if(FLAG_C)
        res -= 0xa0;

    FLAG_V &= res; /* Undefined V behavior part II */
    FLAG_N = NFLAG_8(res); /* Undefined N behavior */

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_add_8_er_d(void)
{
    uint* r_dst = &DX;
    uint src = MASK_OUT_ABOVE_8(DY);
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_ai(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_AI_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PI_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pi7(void)
{
    uint* r_dst = &DX;
    uint src = OPER_A7_PI_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pd(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PD_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pd7(void)
{
    uint* r_dst = &DX;
    uint src = OPER_A7_PD_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_di(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_DI_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_ix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_IX_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_aw(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AW_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_al(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AL_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pcdi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCDI_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_pcix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCIX_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_8_er_i(void)
{
    uint* r_dst = &DX;
    uint src = OPER_I_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_d(void)
{
    uint* r_dst = &DX;
    uint src = MASK_OUT_ABOVE_16(DY);
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_a(void)
{
    uint* r_dst = &DX;
    uint src = MASK_OUT_ABOVE_16(AY);
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_ai(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_AI_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_pi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PI_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_pd(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PD_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_di(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_DI_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_ix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_IX_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_aw(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AW_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_al(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AL_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_pcdi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCDI_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_pcix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCIX_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_16_er_i(void)
{
    uint* r_dst = &DX;
    uint src = OPER_I_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_add_32_er_d(void)
{
    uint* r_dst = &DX;
    uint src = DY;
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_a(void)
{
    uint* r_dst = &DX;
    uint src = AY;
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_ai(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_AI_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_pi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PI_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_pd(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_PD_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_di(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_DI_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_ix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AY_IX_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_aw(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AW_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_al(void)
{
    uint* r_dst = &DX;
    uint src = OPER_AL_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_pcdi(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCDI_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_pcix(void)
{
    uint* r_dst = &DX;
    uint src = OPER_PCIX_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_32_er_i(void)
{
    uint* r_dst = &DX;
    uint src = OPER_I_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_add_8_re_ai(void)
{
    uint ea = EA_AY_AI_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_pi(void)
{
    uint ea = EA_AY_PI_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_pi7(void)
{
    uint ea = EA_A7_PI_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_pd(void)
{
    uint ea = EA_AY_PD_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_pd7(void)
{
    uint ea = EA_A7_PD_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_di(void)
{
    uint ea = EA_AY_DI_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_ix(void)
{
    uint ea = EA_AY_IX_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_aw(void)
{
    uint ea = EA_AW_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_8_re_al(void)
{
    uint ea = EA_AL_8();
    uint src = MASK_OUT_ABOVE_8(DX);
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_add_16_re_ai(void)
{
    uint ea = EA_AY_AI_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_pi(void)
{
    uint ea = EA_AY_PI_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_pd(void)
{
    uint ea = EA_AY_PD_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_di(void)
{
    uint ea = EA_AY_DI_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_ix(void)
{
    uint ea = EA_AY_IX_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_aw(void)
{
    uint ea = EA_AW_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_16_re_al(void)
{
    uint ea = EA_AL_16();
    uint src = MASK_OUT_ABOVE_16(DX);
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_add_32_re_ai(void)
{
    uint ea = EA_AY_AI_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_pi(void)
{
    uint ea = EA_AY_PI_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_pd(void)
{
    uint ea = EA_AY_PD_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_di(void)
{
    uint ea = EA_AY_DI_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_ix(void)
{
    uint ea = EA_AY_IX_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_aw(void)
{
    uint ea = EA_AW_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_add_32_re_al(void)
{
    uint ea = EA_AL_32();
    uint src = DX;
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_adda_16_d(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(DY));
}


static void m68k_op_adda_16_a(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(AY));
}


static void m68k_op_adda_16_ai(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AY_AI_16()));
}


static void m68k_op_adda_16_pi(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AY_PI_16()));
}


static void m68k_op_adda_16_pd(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AY_PD_16()));
}


static void m68k_op_adda_16_di(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AY_DI_16()));
}


static void m68k_op_adda_16_ix(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AY_IX_16()));
}


static void m68k_op_adda_16_aw(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AW_16()));
}


static void m68k_op_adda_16_al(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_AL_16()));
}


static void m68k_op_adda_16_pcdi(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_PCDI_16()));
}


static void m68k_op_adda_16_pcix(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_PCIX_16()));
}


static void m68k_op_adda_16_i(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + MAKE_INT_16(OPER_I_16()));
}


static void m68k_op_adda_32_d(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + DY);
}


static void m68k_op_adda_32_a(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + AY);
}


static void m68k_op_adda_32_ai(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AY_AI_32());
}


static void m68k_op_adda_32_pi(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AY_PI_32());
}


static void m68k_op_adda_32_pd(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AY_PD_32());
}


static void m68k_op_adda_32_di(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AY_DI_32());
}


static void m68k_op_adda_32_ix(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AY_IX_32());
}


static void m68k_op_adda_32_aw(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AW_32());
}


static void m68k_op_adda_32_al(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_AL_32());
}


static void m68k_op_adda_32_pcdi(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_PCDI_32());
}


static void m68k_op_adda_32_pcix(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_PCIX_32());
}


static void m68k_op_adda_32_i(void)
{
    uint* r_dst = &AX;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + OPER_I_32());
}


static void m68k_op_addi_8_d(void)
{
    uint* r_dst = &DY;
    uint src = OPER_I_8();
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_addi_8_ai(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_AI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_pi(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_PI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_pi7(void)
{
    uint src = OPER_I_8();
    uint ea = EA_A7_PI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_pd(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_pd7(void)
{
    uint src = OPER_I_8();
    uint ea = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_di(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_DI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_ix(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_IX_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_aw(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AW_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_8_al(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AL_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addi_16_d(void)
{
    uint* r_dst = &DY;
    uint src = OPER_I_16();
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_addi_16_ai(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_AI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_pi(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_PI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_pd(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_PD_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_di(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_DI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_ix(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_IX_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_aw(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AW_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_16_al(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AL_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addi_32_d(void)
{
    uint* r_dst = &DY;
    uint src = OPER_I_32();
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_addi_32_ai(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_AI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_pi(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_PI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_pd(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_PD_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_di(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_DI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_ix(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_IX_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_aw(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AW_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addi_32_al(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AL_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_8_d(void)
{
    uint* r_dst = &DY;
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | FLAG_Z;
}


static void m68k_op_addq_8_ai(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_AI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_pi(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_pi7(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_A7_PI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_pd(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_pd7(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_di(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_DI_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_ix(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_IX_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_aw(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AW_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_8_al(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AL_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_addq_16_d(void)
{
    uint* r_dst = &DY;
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | FLAG_Z;
}


static void m68k_op_addq_16_a(void)
{
    uint* r_dst = &AY;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + (((REG_IR >> 9) - 1) & 7) + 1);
}


static void m68k_op_addq_16_ai(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_AI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_pi(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_pd(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PD_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_di(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_DI_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_ix(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_IX_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_aw(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AW_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_16_al(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AL_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst;

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_addq_32_d(void)
{
    uint* r_dst = &DY;
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint dst = *r_dst;
    uint res = src + dst;

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    *r_dst = FLAG_Z;
}


static void m68k_op_addq_32_a(void)
{
    uint* r_dst = &AY;

    *r_dst = MASK_OUT_ABOVE_32(*r_dst + (((REG_IR >> 9) - 1) & 7) + 1);
}


static void m68k_op_addq_32_ai(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_AI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_pi(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_pd(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_PD_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_di(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_DI_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_ix(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AY_IX_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_aw(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AW_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addq_32_al(void)
{
    uint src = (((REG_IR >> 9) - 1) & 7) + 1;
    uint ea = EA_AL_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst;


    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);

    m68ki_write_32(ea, FLAG_Z);
}


static void m68k_op_addx_8_rr(void)
{
    uint* r_dst = &DX;
    uint src = MASK_OUT_ABOVE_8(DY);
    uint dst = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;
}


static void m68k_op_addx_16_rr(void)
{
    uint* r_dst = &DX;
    uint src = MASK_OUT_ABOVE_16(DY);
    uint dst = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);

    res = MASK_OUT_ABOVE_16(res);
    FLAG_Z |= res;

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | res;
}


static void m68k_op_addx_32_rr(void)
{
    uint* r_dst = &DX;
    uint src = DY;
    uint dst = *r_dst;
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);

    res = MASK_OUT_ABOVE_32(res);
    FLAG_Z |= res;

    *r_dst = res;
}


static void m68k_op_addx_8_mm_ax7(void)
{
    uint src = OPER_AY_PD_8();
    uint ea  = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_addx_8_mm_ay7(void)
{
    uint src = OPER_A7_PD_8();
    uint ea  = EA_AX_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_addx_8_mm_axy7(void)
{
    uint src = OPER_A7_PD_8();
    uint ea  = EA_A7_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_addx_8_mm(void)
{
    uint src = OPER_AY_PD_8();
    uint ea  = EA_AX_PD_8();
    uint dst = m68ki_read_8(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_8(res);
    FLAG_V = VFLAG_ADD_8(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_8(res);

    res = MASK_OUT_ABOVE_8(res);
    FLAG_Z |= res;

    m68ki_write_8(ea, res);
}


static void m68k_op_addx_16_mm(void)
{
    uint src = OPER_AY_PD_16();
    uint ea  = EA_AX_PD_16();
    uint dst = m68ki_read_16(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_16(res);
    FLAG_V = VFLAG_ADD_16(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_16(res);

    res = MASK_OUT_ABOVE_16(res);
    FLAG_Z |= res;

    m68ki_write_16(ea, res);
}


static void m68k_op_addx_32_mm(void)
{
    uint src = OPER_AY_PD_32();
    uint ea  = EA_AX_PD_32();
    uint dst = m68ki_read_32(ea);
    uint res = src + dst + XFLAG_AS_1();

    FLAG_N = NFLAG_32(res);
    FLAG_V = VFLAG_ADD_32(src, dst, res);
    FLAG_X = FLAG_C = CFLAG_ADD_32(src, dst, res);

    res = MASK_OUT_ABOVE_32(res);
    FLAG_Z |= res;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_8_er_d(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (DY | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_ai(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AY_AI_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pi(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AY_PI_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pi7(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_A7_PI_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pd(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AY_PD_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pd7(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_A7_PD_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_di(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AY_DI_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_ix(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AY_IX_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_aw(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AW_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_al(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_AL_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pcdi(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_PCDI_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_pcix(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_PCIX_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_er_i(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DX &= (OPER_I_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_d(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (DY | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_ai(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AY_AI_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_pi(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AY_PI_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_pd(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AY_PD_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_di(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AY_DI_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_ix(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AY_IX_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_aw(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AW_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_al(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_AL_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_pcdi(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_PCDI_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_pcix(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_PCIX_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_16_er_i(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DX &= (OPER_I_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_d(void)
{
    FLAG_Z = DX &= DY;

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_ai(void)
{
    FLAG_Z = DX &= OPER_AY_AI_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_pi(void)
{
    FLAG_Z = DX &= OPER_AY_PI_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_pd(void)
{
    FLAG_Z = DX &= OPER_AY_PD_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_di(void)
{
    FLAG_Z = DX &= OPER_AY_DI_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_ix(void)
{
    FLAG_Z = DX &= OPER_AY_IX_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_aw(void)
{
    FLAG_Z = DX &= OPER_AW_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_al(void)
{
    FLAG_Z = DX &= OPER_AL_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_pcdi(void)
{
    FLAG_Z = DX &= OPER_PCDI_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_pcix(void)
{
    FLAG_Z = DX &= OPER_PCIX_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_32_er_i(void)
{
    FLAG_Z = DX &= OPER_I_32();

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_and_8_re_ai(void)
{
    uint ea = EA_AY_AI_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_pi(void)
{
    uint ea = EA_AY_PI_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_pi7(void)
{
    uint ea = EA_A7_PI_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_pd(void)
{
    uint ea = EA_AY_PD_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_pd7(void)
{
    uint ea = EA_A7_PD_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_di(void)
{
    uint ea = EA_AY_DI_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_ix(void)
{
    uint ea = EA_AY_IX_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_aw(void)
{
    uint ea = EA_AW_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_8_re_al(void)
{
    uint ea = EA_AL_8();
    uint res = DX & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_8(res);

    m68ki_write_8(ea, FLAG_Z);
}


static void m68k_op_and_16_re_ai(void)
{
    uint ea = EA_AY_AI_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_pi(void)
{
    uint ea = EA_AY_PI_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_pd(void)
{
    uint ea = EA_AY_PD_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_di(void)
{
    uint ea = EA_AY_DI_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_ix(void)
{
    uint ea = EA_AY_IX_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_aw(void)
{
    uint ea = EA_AW_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_16_re_al(void)
{
    uint ea = EA_AL_16();
    uint res = DX & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_Z = MASK_OUT_ABOVE_16(res);

    m68ki_write_16(ea, FLAG_Z);
}


static void m68k_op_and_32_re_ai(void)
{
    uint ea = EA_AY_AI_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_pi(void)
{
    uint ea = EA_AY_PI_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_pd(void)
{
    uint ea = EA_AY_PD_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_di(void)
{
    uint ea = EA_AY_DI_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_ix(void)
{
    uint ea = EA_AY_IX_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_aw(void)
{
    uint ea = EA_AW_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_and_32_re_al(void)
{
    uint ea = EA_AL_32();
    uint res = DX & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_8_d(void)
{
    FLAG_Z = MASK_OUT_ABOVE_8(DY &= (OPER_I_8() | 0xffffff00));

    FLAG_N = NFLAG_8(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_andi_8_ai(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_AI_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_pi(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_PI_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_pi7(void)
{
    uint src = OPER_I_8();
    uint ea = EA_A7_PI_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_pd(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_PD_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_pd7(void)
{
    uint src = OPER_I_8();
    uint ea = EA_A7_PD_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_di(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_DI_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_ix(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AY_IX_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_aw(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AW_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_8_al(void)
{
    uint src = OPER_I_8();
    uint ea = EA_AL_8();
    uint res = src & m68ki_read_8(ea);

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_8(ea, res);
}


static void m68k_op_andi_16_d(void)
{
    FLAG_Z = MASK_OUT_ABOVE_16(DY &= (OPER_I_16() | 0xffff0000));

    FLAG_N = NFLAG_16(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_andi_16_ai(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_AI_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_pi(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_PI_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_pd(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_PD_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_di(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_DI_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_ix(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AY_IX_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_aw(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AW_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_16_al(void)
{
    uint src = OPER_I_16();
    uint ea = EA_AL_16();
    uint res = src & m68ki_read_16(ea);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_16(ea, res);
}


static void m68k_op_andi_32_d(void)
{
    FLAG_Z = DY &= (OPER_I_32());

    FLAG_N = NFLAG_32(FLAG_Z);
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_andi_32_ai(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_AI_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_pi(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_PI_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_pd(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_PD_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_di(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_DI_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_ix(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AY_IX_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_aw(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AW_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_32_al(void)
{
    uint src = OPER_I_32();
    uint ea = EA_AL_32();
    uint res = src & m68ki_read_32(ea);

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_C = CFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;

    m68ki_write_32(ea, res);
}


static void m68k_op_andi_16_toc(void)
{
    m68ki_set_ccr(m68ki_get_ccr() & OPER_I_16());
}


static void m68k_op_andi_16_tos(void)
{
    if(FLAG_S)
    {
        uint src = OPER_I_16();
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_set_sr(m68ki_get_sr() & src);
        return;
    }
    m68ki_exception_privilege_violation();
}


static void m68k_op_asr_8_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src >> shift;

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    if(GET_MSB_8(src))
        res |= m68ki_shift_8_table[shift];

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_X = FLAG_C = src << (9-shift);
}


static void m68k_op_asr_16_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src >> shift;

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    if(GET_MSB_16(src))
        res |= m68ki_shift_16_table[shift];

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | res;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_X = FLAG_C = src << (9-shift);
}


static void m68k_op_asr_32_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = *r_dst;
    uint res = src >> shift;

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    if(GET_MSB_32(src))
        res |= m68ki_shift_32_table[shift];

    *r_dst = res;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_X = FLAG_C = src << (9-shift);
}


static void m68k_op_asr_8_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = MASK_OUT_ABOVE_8(*r_dst);
    uint res = src >> shift;

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 8)
        {
            if(GET_MSB_8(src))
                res |= m68ki_shift_8_table[shift];

            *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;

            FLAG_X = FLAG_C = src << (9-shift);
            FLAG_N = NFLAG_8(res);
            FLAG_Z = res;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        if(GET_MSB_8(src))
        {
            *r_dst |= 0xff;
            FLAG_C = CFLAG_SET;
            FLAG_X = XFLAG_SET;
            FLAG_N = NFLAG_SET;
            FLAG_Z = ZFLAG_CLEAR;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        *r_dst &= 0xffffff00;
        FLAG_C = CFLAG_CLEAR;
        FLAG_X = XFLAG_CLEAR;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = VFLAG_CLEAR;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_8(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asr_16_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = MASK_OUT_ABOVE_16(*r_dst);
    uint res = src >> shift;

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 16)
        {
            if(GET_MSB_16(src))
                res |= m68ki_shift_16_table[shift];

            *r_dst = MASK_OUT_BELOW_16(*r_dst) | res;

            FLAG_C = FLAG_X = (src >> (shift - 1))<<8;
            FLAG_N = NFLAG_16(res);
            FLAG_Z = res;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        if(GET_MSB_16(src))
        {
            *r_dst |= 0xffff;
            FLAG_C = CFLAG_SET;
            FLAG_X = XFLAG_SET;
            FLAG_N = NFLAG_SET;
            FLAG_Z = ZFLAG_CLEAR;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        *r_dst &= 0xffff0000;
        FLAG_C = CFLAG_CLEAR;
        FLAG_X = XFLAG_CLEAR;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = VFLAG_CLEAR;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_16(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asr_32_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = *r_dst;
    uint res = src >> shift;

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 32)
        {
            if(GET_MSB_32(src))
                res |= m68ki_shift_32_table[shift];

            *r_dst = res;

            FLAG_C = FLAG_X = (src >> (shift - 1))<<8;
            FLAG_N = NFLAG_32(res);
            FLAG_Z = res;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        if(GET_MSB_32(src))
        {
            *r_dst = 0xffffffff;
            FLAG_C = CFLAG_SET;
            FLAG_X = XFLAG_SET;
            FLAG_N = NFLAG_SET;
            FLAG_Z = ZFLAG_CLEAR;
            FLAG_V = VFLAG_CLEAR;
            return;
        }

        *r_dst = 0;
        FLAG_C = CFLAG_CLEAR;
        FLAG_X = XFLAG_CLEAR;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = VFLAG_CLEAR;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_32(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asr_16_ai(void)
{
    uint ea = EA_AY_AI_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_pi(void)
{
    uint ea = EA_AY_PI_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_pd(void)
{
    uint ea = EA_AY_PD_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_di(void)
{
    uint ea = EA_AY_DI_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_ix(void)
{
    uint ea = EA_AY_IX_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_aw(void)
{
    uint ea = EA_AW_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asr_16_al(void)
{
    uint ea = EA_AL_16();
    uint src = m68ki_read_16(ea);
    uint res = src >> 1;

    if(GET_MSB_16(src))
        res |= 0x8000;

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = FLAG_X = src << 8;
}


static void m68k_op_asl_8_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = MASK_OUT_ABOVE_8(*r_dst);
    uint res = MASK_OUT_ABOVE_8(src << shift);

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;

    FLAG_X = FLAG_C = src << shift;
    FLAG_N = NFLAG_8(res);
    FLAG_Z = res;
    src &= m68ki_shift_8_table[shift + 1];
    FLAG_V = (!(src == 0 || (src == m68ki_shift_8_table[shift + 1] && shift < 8)))<<7;
}


static void m68k_op_asl_16_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = MASK_OUT_ABOVE_16(*r_dst);
    uint res = MASK_OUT_ABOVE_16(src << shift);

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    *r_dst = MASK_OUT_BELOW_16(*r_dst) | res;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> (8-shift);
    src &= m68ki_shift_16_table[shift + 1];
    FLAG_V = (!(src == 0 || src == m68ki_shift_16_table[shift + 1]))<<7;
}


static void m68k_op_asl_32_s(void)
{
    uint* r_dst = &DY;
    uint shift = (((REG_IR >> 9) - 1) & 7) + 1;
    uint src = *r_dst;
    uint res = MASK_OUT_ABOVE_32(src << shift);

    if(shift != 0)
        USE_CYCLES(shift<<CYC_SHIFT);

    *r_dst = res;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> (24-shift);
    src &= m68ki_shift_32_table[shift + 1];
    FLAG_V = (!(src == 0 || src == m68ki_shift_32_table[shift + 1]))<<7;
}


static void m68k_op_asl_8_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = MASK_OUT_ABOVE_8(*r_dst);
    uint res = MASK_OUT_ABOVE_8(src << shift);

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 8)
        {
            *r_dst = MASK_OUT_BELOW_8(*r_dst) | res;
            FLAG_X = FLAG_C = src << shift;
            FLAG_N = NFLAG_8(res);
            FLAG_Z = res;
            src &= m68ki_shift_8_table[shift + 1];
            FLAG_V = (!(src == 0 || src == m68ki_shift_8_table[shift + 1]))<<7;
            return;
        }

        *r_dst &= 0xffffff00;
        FLAG_X = FLAG_C = ((shift == 8 ? src & 1 : 0))<<8;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = (!(src == 0))<<7;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_8(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asl_16_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = MASK_OUT_ABOVE_16(*r_dst);
    uint res = MASK_OUT_ABOVE_16(src << shift);

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 16)
        {
            *r_dst = MASK_OUT_BELOW_16(*r_dst) | res;
            FLAG_X = FLAG_C = (src << shift) >> 8;
            FLAG_N = NFLAG_16(res);
            FLAG_Z = res;
            src &= m68ki_shift_16_table[shift + 1];
            FLAG_V = (!(src == 0 || src == m68ki_shift_16_table[shift + 1]))<<7;
            return;
        }

        *r_dst &= 0xffff0000;
        FLAG_X = FLAG_C = ((shift == 16 ? src & 1 : 0))<<8;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = (!(src == 0))<<7;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_16(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asl_32_r(void)
{
    uint* r_dst = &DY;
    uint shift = DX & 0x3f;
    uint src = *r_dst;
    uint res = MASK_OUT_ABOVE_32(src << shift);

    if(shift != 0)
    {
        USE_CYCLES(shift<<CYC_SHIFT);

        if(shift < 32)
        {
            *r_dst = res;
            FLAG_X = FLAG_C = (src >> (32 - shift)) << 8;
            FLAG_N = NFLAG_32(res);
            FLAG_Z = res;
            src &= m68ki_shift_32_table[shift + 1];
            FLAG_V = (!(src == 0 || src == m68ki_shift_32_table[shift + 1]))<<7;
            return;
        }

        *r_dst = 0;
        FLAG_X = FLAG_C = ((shift == 32 ? src & 1 : 0))<<8;
        FLAG_N = NFLAG_CLEAR;
        FLAG_Z = ZFLAG_SET;
        FLAG_V = (!(src == 0))<<7;
        return;
    }

    FLAG_C = CFLAG_CLEAR;
    FLAG_N = NFLAG_32(src);
    FLAG_Z = src;
    FLAG_V = VFLAG_CLEAR;
}


static void m68k_op_asl_16_ai(void)
{
    uint ea = EA_AY_AI_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_pi(void)
{
    uint ea = EA_AY_PI_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_pd(void)
{
    uint ea = EA_AY_PD_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_di(void)
{
    uint ea = EA_AY_DI_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_ix(void)
{
    uint ea = EA_AY_IX_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_aw(void)
{
    uint ea = EA_AW_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_asl_16_al(void)
{
    uint ea = EA_AL_16();
    uint src = m68ki_read_16(ea);
    uint res = MASK_OUT_ABOVE_16(src << 1);

    m68ki_write_16(ea, res);

    FLAG_N = NFLAG_16(res);
    FLAG_Z = res;
    FLAG_X = FLAG_C = src >> 7;
    src &= 0xc000;
    FLAG_V = (!(src == 0 || src == 0xc000))<<7;
}


static void m68k_op_bhi_8(void)
{
    if(COND_HI())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bls_8(void)
{
    if(COND_LS())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bcc_8(void)
{
    if(COND_CC())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bcs_8(void)
{
    if(COND_CS())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bne_8(void)
{
    if(COND_NE())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_beq_8(void)
{
    if(COND_EQ())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bvc_8(void)
{
    if(COND_VC())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bvs_8(void)
{
    if(COND_VS())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bpl_8(void)
{
    if(COND_PL())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bmi_8(void)
{
    if(COND_MI())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bge_8(void)
{
    if(COND_GE())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_blt_8(void)
{
    if(COND_LT())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bgt_8(void)
{
    if(COND_GT())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_ble_8(void)
{
    if(COND_LE())
    {
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
        return;
    }
    USE_CYCLES(CYC_BCC_NOTAKE_B);
}


static void m68k_op_bhi_16(void)
{
    if(COND_HI())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bls_16(void)
{
    if(COND_LS())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bcc_16(void)
{
    if(COND_CC())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bcs_16(void)
{
    if(COND_CS())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bne_16(void)
{
    if(COND_NE())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_beq_16(void)
{
    if(COND_EQ())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bvc_16(void)
{
    if(COND_VC())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bvs_16(void)
{
    if(COND_VS())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bpl_16(void)
{
    if(COND_PL())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bmi_16(void)
{
    if(COND_MI())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bge_16(void)
{
    if(COND_GE())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_blt_16(void)
{
    if(COND_LT())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bgt_16(void)
{
    if(COND_GT())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_ble_16(void)
{
    if(COND_LE())
    {
        uint offset = OPER_I_16();
        REG_PC -= 2;
        m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
        m68ki_branch_16(offset);
        return;
    }
    REG_PC += 2;
    USE_CYCLES(CYC_BCC_NOTAKE_W);
}


static void m68k_op_bchg_32_r_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (DX & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst ^= mask;
}


static void m68k_op_bchg_8_r_ai(void)
{
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_pi(void)
{
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_pi7(void)
{
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_pd(void)
{
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_pd7(void)
{
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_di(void)
{
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_ix(void)
{
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_aw(void)
{
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_r_al(void)
{
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_32_s_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (OPER_I_8() & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst ^= mask;
}


static void m68k_op_bchg_8_s_ai(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_pi(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_pi7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_pd(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_pd7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_di(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_ix(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_aw(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bchg_8_s_al(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src ^ mask);
}


static void m68k_op_bclr_32_r_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (DX & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst &= ~mask;
}


static void m68k_op_bclr_8_r_ai(void)
{
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_pi(void)
{
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_pi7(void)
{
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_pd(void)
{
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_pd7(void)
{
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_di(void)
{
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_ix(void)
{
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_aw(void)
{
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_r_al(void)
{
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_32_s_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (OPER_I_8() & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst &= ~mask;
}


static void m68k_op_bclr_8_s_ai(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_pi(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_pi7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_pd(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_pd7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_di(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_ix(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_aw(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bclr_8_s_al(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src & ~mask);
}


static void m68k_op_bra_8(void)
{
    m68ki_trace_t0();                  /* auto-disable (see m68kcpu.h) */
    m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
    if(REG_PC == REG_PPC)
        USE_ALL_CYCLES();
}


static void m68k_op_bra_16(void)
{
    uint offset = OPER_I_16();
    REG_PC -= 2;
    m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
    m68ki_branch_16(offset);
    if(REG_PC == REG_PPC)
        USE_ALL_CYCLES();
}


static void m68k_op_bset_32_r_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (DX & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst |= mask;
}


static void m68k_op_bset_8_r_ai(void)
{
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_pi(void)
{
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_pi7(void)
{
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_pd(void)
{
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_pd7(void)
{
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_di(void)
{
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_ix(void)
{
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_aw(void)
{
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_r_al(void)
{
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);
    uint mask = 1 << (DX & 7);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_32_s_d(void)
{
    uint* r_dst = &DY;
    uint mask = 1 << (OPER_I_8() & 0x1f);

    FLAG_Z = *r_dst & mask;
    *r_dst |= mask;
}


static void m68k_op_bset_8_s_ai(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_AI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_pi(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_pi7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_pd(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_pd7(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_A7_PD_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_di(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_DI_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_ix(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AY_IX_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_aw(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AW_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bset_8_s_al(void)
{
    uint mask = 1 << (OPER_I_8() & 7);
    uint ea = EA_AL_8();
    uint src = m68ki_read_8(ea);

    FLAG_Z = src & mask;
    m68ki_write_8(ea, src | mask);
}


static void m68k_op_bsr_8(void)
{
    m68ki_trace_t0();                  /* auto-disable (see m68kcpu.h) */
    m68ki_push_32(REG_PC);
    m68ki_branch_8(MASK_OUT_ABOVE_8(REG_IR));
}


static void m68k_op_bsr_16(void)
{
    uint offset = OPER_I_16();
    m68ki_trace_t0();              /* auto-disable (see m68kcpu.h) */
    m68ki_push_32(REG_PC);
    REG_PC -= 2;
    m68ki_branch_16(offset);
}


static void m68k_op_btst_32_r_d(void)
{
    FLAG_Z = DY & (1 << (DX & 0x1f));
}


static void m68k_op_btst_8_r_ai(void)
{
    FLAG_Z = OPER_AY_AI_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pi(void)
{
    FLAG_Z = OPER_AY_PI_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pi7(void)
{
    FLAG_Z = OPER_A7_PI_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pd(void)
{
    FLAG_Z = OPER_AY_PD_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pd7(void)
{
    FLAG_Z = OPER_A7_PD_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_di(void)
{
    FLAG_Z = OPER_AY_DI_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_ix(void)
{
    FLAG_Z = OPER_AY_IX_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_aw(void)
{
    FLAG_Z = OPER_AW_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_al(void)
{
    FLAG_Z = OPER_AL_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pcdi(void)
{
    FLAG_Z = OPER_PCDI_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_pcix(void)
{
    FLAG_Z = OPER_PCIX_8() & (1 << (DX & 7));
}


static void m68k_op_btst_8_r_i(void)
{
    FLAG_Z = OPER_I_8() & (1 << (DX & 7));
}


static void m68k_op_btst_32_s_d(void)
{
    FLAG_Z = DY & (1 << (OPER_I_8() & 0x1f));
}


static void m68k_op_btst_8_s_ai(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AY_AI_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pi(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AY_PI_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pi7(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_A7_PI_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pd(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AY_PD_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pd7(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_A7_PD_8() & (1 << bit);
}


static void m68k_op_btst_8_s_di(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AY_DI_8() & (1 << bit);
}


static void m68k_op_btst_8_s_ix(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AY_IX_8() & (1 << bit);
}


static void m68k_op_btst_8_s_aw(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AW_8() & (1 << bit);
}


static void m68k_op_btst_8_s_al(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_AL_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pcdi(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_PCDI_8() & (1 << bit);
}


static void m68k_op_btst_8_s_pcix(void)
{
    uint bit = OPER_I_8() & 7;

    FLAG_Z = OPER_PCIX_8() & (1 << bit);
}


static void m68k_op_chk_16_d(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(DY);

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_ai(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AY_AI_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_pi(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AY_PI_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_pd(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AY_PD_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_di(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AY_DI_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_ix(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AY_IX_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_aw(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AW_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_al(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_AL_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_pcdi(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_PCDI_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_pcix(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_PCIX_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_chk_16_i(void)
{
    sint src = MAKE_INT_16(DX);
    sint bound = MAKE_INT_16(OPER_I_16());

    FLAG_Z = ZFLAG_16(src); /* Undocumented */
    FLAG_V = VFLAG_CLEAR;   /* Undocumented */
    FLAG_C = CFLAG_CLEAR;   /* Undocumented */

    if(src >= 0 && src <= bound)
    {
        return;
    }
    FLAG_N = (src < 0)<<7;
    m68ki_exception_trap(EXCEPTION_CHK);
}


static void m68k_op_clr_8_d(void)
{
    DY &= 0xffffff00;

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_ai(void)
{
    m68ki_write_8(EA_AY_AI_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_pi(void)
{
    m68ki_write_8(EA_AY_PI_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_pi7(void)
{
    m68ki_write_8(EA_A7_PI_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_pd(void)
{
    m68ki_write_8(EA_AY_PD_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_pd7(void)
{
    m68ki_write_8(EA_A7_PD_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_di(void)
{
    m68ki_write_8(EA_AY_DI_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_ix(void)
{
    m68ki_write_8(EA_AY_IX_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_aw(void)
{
    m68ki_write_8(EA_AW_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_8_al(void)
{
    m68ki_write_8(EA_AL_8(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_d(void)
{
    DY &= 0xffff0000;

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_ai(void)
{
    m68ki_write_16(EA_AY_AI_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_pi(void)
{
    m68ki_write_16(EA_AY_PI_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_pd(void)
{
    m68ki_write_16(EA_AY_PD_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_di(void)
{
    m68ki_write_16(EA_AY_DI_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_ix(void)
{
    m68ki_write_16(EA_AY_IX_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_aw(void)
{
    m68ki_write_16(EA_AW_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_16_al(void)
{
    m68ki_write_16(EA_AL_16(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_d(void)
{
    DY = 0;

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_ai(void)
{
    m68ki_write_32(EA_AY_AI_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_pi(void)
{
    m68ki_write_32(EA_AY_PI_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_pd(void)
{
    m68ki_write_32(EA_AY_PD_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_di(void)
{
    m68ki_write_32(EA_AY_DI_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_ix(void)
{
    m68ki_write_32(EA_AY_IX_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_aw(void)
{
    m68ki_write_32(EA_AW_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_clr_32_al(void)
{
    m68ki_write_32(EA_AL_32(), 0);

    FLAG_N = NFLAG_CLEAR;
    FLAG_V = VFLAG_CLEAR;
    FLAG_C = CFLAG_CLEAR;
    FLAG_Z = ZFLAG_SET;
}


static void m68k_op_cmp_8_d(void)
{
    uint src = MASK_OUT_ABOVE_8(DY);
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_ai(void)
{
    uint src = OPER_AY_AI_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pi(void)
{
    uint src = OPER_AY_PI_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pi7(void)
{
    uint src = OPER_A7_PI_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pd(void)
{
    uint src = OPER_AY_PD_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pd7(void)
{
    uint src = OPER_A7_PD_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_di(void)
{
    uint src = OPER_AY_DI_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_ix(void)
{
    uint src = OPER_AY_IX_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_aw(void)
{
    uint src = OPER_AW_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_al(void)
{
    uint src = OPER_AL_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pcdi(void)
{
    uint src = OPER_PCDI_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_pcix(void)
{
    uint src = OPER_PCIX_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_8_i(void)
{
    uint src = OPER_I_8();
    uint dst = MASK_OUT_ABOVE_8(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmp_16_d(void)
{
    uint src = MASK_OUT_ABOVE_16(DY);
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_a(void)
{
    uint src = MASK_OUT_ABOVE_16(AY);
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_ai(void)
{
    uint src = OPER_AY_AI_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_pi(void)
{
    uint src = OPER_AY_PI_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_pd(void)
{
    uint src = OPER_AY_PD_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_di(void)
{
    uint src = OPER_AY_DI_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_ix(void)
{
    uint src = OPER_AY_IX_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_aw(void)
{
    uint src = OPER_AW_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_al(void)
{
    uint src = OPER_AL_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_pcdi(void)
{
    uint src = OPER_PCDI_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_pcix(void)
{
    uint src = OPER_PCIX_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_16_i(void)
{
    uint src = OPER_I_16();
    uint dst = MASK_OUT_ABOVE_16(DX);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmp_32_d(void)
{
    uint src = DY;
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_a(void)
{
    uint src = AY;
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_ai(void)
{
    uint src = OPER_AY_AI_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_pi(void)
{
    uint src = OPER_AY_PI_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_pd(void)
{
    uint src = OPER_AY_PD_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_di(void)
{
    uint src = OPER_AY_DI_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_ix(void)
{
    uint src = OPER_AY_IX_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_aw(void)
{
    uint src = OPER_AW_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_al(void)
{
    uint src = OPER_AL_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_pcdi(void)
{
    uint src = OPER_PCDI_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_pcix(void)
{
    uint src = OPER_PCIX_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmp_32_i(void)
{
    uint src = OPER_I_32();
    uint dst = DX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_d(void)
{
    uint src = MAKE_INT_16(DY);
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_a(void)
{
    uint src = MAKE_INT_16(AY);
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_ai(void)
{
    uint src = MAKE_INT_16(OPER_AY_AI_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_pi(void)
{
    uint src = MAKE_INT_16(OPER_AY_PI_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_pd(void)
{
    uint src = MAKE_INT_16(OPER_AY_PD_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_di(void)
{
    uint src = MAKE_INT_16(OPER_AY_DI_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_ix(void)
{
    uint src = MAKE_INT_16(OPER_AY_IX_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_aw(void)
{
    uint src = MAKE_INT_16(OPER_AW_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_al(void)
{
    uint src = MAKE_INT_16(OPER_AL_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_pcdi(void)
{
    uint src = MAKE_INT_16(OPER_PCDI_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_pcix(void)
{
    uint src = MAKE_INT_16(OPER_PCIX_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_16_i(void)
{
    uint src = MAKE_INT_16(OPER_I_16());
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_d(void)
{
    uint src = DY;
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_a(void)
{
    uint src = AY;
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_ai(void)
{
    uint src = OPER_AY_AI_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_pi(void)
{
    uint src = OPER_AY_PI_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_pd(void)
{
    uint src = OPER_AY_PD_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_di(void)
{
    uint src = OPER_AY_DI_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_ix(void)
{
    uint src = OPER_AY_IX_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_aw(void)
{
    uint src = OPER_AW_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_al(void)
{
    uint src = OPER_AL_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_pcdi(void)
{
    uint src = OPER_PCDI_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_pcix(void)
{
    uint src = OPER_PCIX_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpa_32_i(void)
{
    uint src = OPER_I_32();
    uint dst = AX;
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_8_d(void)
{
    uint src = OPER_I_8();
    uint dst = MASK_OUT_ABOVE_8(DY);
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_ai(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AY_AI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_pi(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AY_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_pi7(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_A7_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_pd(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AY_PD_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_pd7(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_A7_PD_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_di(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AY_DI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_ix(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AY_IX_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_aw(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AW_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_8_al(void)
{
    uint src = OPER_I_8();
    uint dst = OPER_AL_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpi_16_d(void)
{
    uint src = OPER_I_16();
    uint dst = MASK_OUT_ABOVE_16(DY);
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_ai(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AY_AI_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_pi(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AY_PI_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_pd(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AY_PD_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_di(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AY_DI_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_ix(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AY_IX_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_aw(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AW_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_16_al(void)
{
    uint src = OPER_I_16();
    uint dst = OPER_AL_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpi_32_d(void)
{
    uint src = OPER_I_32();
    uint dst = DY;
    uint res = dst - src;

    m68ki_cmpild_callback(src, REG_IR & 7);        /* auto-disable (see m68kcpu.h) */

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_ai(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AY_AI_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_pi(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AY_PI_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_pd(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AY_PD_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_di(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AY_DI_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_ix(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AY_IX_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_aw(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AW_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpi_32_al(void)
{
    uint src = OPER_I_32();
    uint dst = OPER_AL_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


static void m68k_op_cmpm_8_ax7(void)
{
    uint src = OPER_AY_PI_8();
    uint dst = OPER_A7_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpm_8_ay7(void)
{
    uint src = OPER_A7_PI_8();
    uint dst = OPER_AX_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpm_8_axy7(void)
{
    uint src = OPER_A7_PI_8();
    uint dst = OPER_A7_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpm_8(void)
{
    uint src = OPER_AY_PI_8();
    uint dst = OPER_AX_PI_8();
    uint res = dst - src;

    FLAG_N = NFLAG_8(res);
    FLAG_Z = MASK_OUT_ABOVE_8(res);
    FLAG_V = VFLAG_SUB_8(src, dst, res);
    FLAG_C = CFLAG_8(res);
}


static void m68k_op_cmpm_16(void)
{
    uint src = OPER_AY_PI_16();
    uint dst = OPER_AX_PI_16();
    uint res = dst - src;

    FLAG_N = NFLAG_16(res);
    FLAG_Z = MASK_OUT_ABOVE_16(res);
    FLAG_V = VFLAG_SUB_16(src, dst, res);
    FLAG_C = CFLAG_16(res);
}


static void m68k_op_cmpm_32(void)
{
    uint src = OPER_AY_PI_32();
    uint dst = OPER_AX_PI_32();
    uint res = dst - src;

    FLAG_N = NFLAG_32(res);
    FLAG_Z = MASK_OUT_ABOVE_32(res);
    FLAG_V = VFLAG_SUB_32(src, dst, res);
    FLAG_C = CFLAG_SUB_32(src, dst, res);
}


/* ======================================================================== */
/* ==== AUTO GENERATED ========== END OF FILE ============================= */
/* ======================================================================== */


