/*********************************************************************

	usrintrf.c

	Functions used to handle MAME's user interface.

*********************************************************************/

#include "driver.h"
//#include "info.h"
//#include "artwork.h"//#include "vidhrdw/vector.h"
#include "datafile.h"
#include <stdarg.h>
//#include <math.h>
#include "ui_text.h"
#include "state.h"



/***************************************************************************

	Externals

***************************************************************************/

/* Variables for stat menu */
extern char build_version[];

/* MARTINEZ.F 990207 Memory Card */

struct MEMCARDinterface memcard_intf;
int mcd_action;
int mcd_number;
int memcard_status;
int memcard_number;
int memcard_manager;

#if defined(__sgi) && !defined(MESS)
static int game_paused = 0; /* not zero if the game is paused */
#endif
/* MARTINEZ.F 990207 Memory Card End */


/***************************************************************************

	Local variables

***************************************************************************/

/* raw coordinates, relative to the real scrbitmap */
static struct rectangle uirawbounds;
static int uirawcharwidth, uirawcharheight;

/* rotated coordinates, easier to deal with */
static struct rectangle uirotbounds;
static int uirotwidth, uirotheight;
int uirotcharwidth, uirotcharheight;

static int setup_selected;
static int osd_selected;
static int jukebox_selected;
static int single_step;

static int showfps;
static int showfpstemp;

static int show_profiler;

UINT8 ui_dirty;

int input_port_settings_modify; 	//TMK

/***************************************************************************
	Font data
***************************************************************************/
//hankaku_font5x10
extern const unsigned char hankaku_font5x10[];

#define MAX_UIFONT_SIZE 8 /* max(width,height) */
static const struct GfxLayout uifontlayout =
{
	(6+1),8,
	256,
	1,
	{ 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8
};


#if 0
#pragma mark UTILITIES
#endif

/*-------------------------------------------------
	ui_markdirty - mark a raw rectangle dirty
-------------------------------------------------*/

INLINE void ui_markdirty(const struct rectangle *rect)
{
	//artwork_mark_ui_dirty(rect->min_x, rect->min_y, rect->max_x, rect->max_y);
	ui_dirty = 5;
}



/*-------------------------------------------------
	ui_raw2rot_rect - convert a rect from raw
	coordinates to rotated coordinates
-------------------------------------------------*/

static void ui_raw2rot_rect(struct rectangle *rect)
{
	int temp, w, h;
	/* get the effective screen size, including artwork */
	//artwork_get_screensize(&w, &h);
	w = Machine->drv->screen_width;
	h = Machine->drv->screen_height;

	/* apply X flip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_X)
	{
		temp = w - rect->min_x - 1;
		rect->min_x = w - rect->max_x - 1;
		rect->max_x = temp;
	}

	/* apply Y flip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_Y)
	{
		temp = h - rect->min_y - 1;
		rect->min_y = h - rect->max_y - 1;
		rect->max_y = temp;
	}

	/* apply X/Y swap first */
	if (Machine->ui_orientation & ORIENTATION_SWAP_XY)
	{
		temp = rect->min_x; rect->min_x = rect->min_y; rect->min_y = temp;
		temp = rect->max_x; rect->max_x = rect->max_y; rect->max_y = temp;
	}
}



/*-------------------------------------------------
	ui_rot2raw_rect - convert a rect from rotated
	coordinates to raw coordinates
-------------------------------------------------*/

static void ui_rot2raw_rect(struct rectangle *rect)
{
	int temp, w, h;
	/* get the effective screen size, including artwork */
	//artwork_get_screensize(&w, &h);
	w = Machine->drv->screen_width;
	h = Machine->drv->screen_height;

	/* apply X/Y swap first */
	if (Machine->ui_orientation & ORIENTATION_SWAP_XY)
	{
		temp = rect->min_x; rect->min_x = rect->min_y; rect->min_y = temp;
		temp = rect->max_x; rect->max_x = rect->max_y; rect->max_y = temp;
	}

	/* apply X flip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_X)
	{
		temp = w - rect->min_x - 1;
		rect->min_x = w - rect->max_x - 1;
		rect->max_x = temp;
	}

	/* apply Y flip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_Y)
	{
		temp = h - rect->min_y - 1;
		rect->min_y = h - rect->max_y - 1;
		rect->max_y = temp;
	}
}



/*-------------------------------------------------
	set_ui_visarea - called by the OSD code to
	set the UI's domain
-------------------------------------------------*/

void set_ui_visarea(int xmin, int ymin, int xmax, int ymax)
{
	/* fill in the rect */
	uirawbounds.min_x = xmin;
	uirawbounds.min_y = ymin;
	uirawbounds.max_x = xmax;
	uirawbounds.max_y = ymax;

	/* orient it */
	uirotbounds = uirawbounds;
	ui_raw2rot_rect(&uirotbounds);

	/* make some easier-to-access globals */
	uirotwidth  = uirotbounds.max_x - uirotbounds.min_x + 1;
	uirotheight = uirotbounds.max_y - uirotbounds.min_y + 1;

	/* remove me */
	Machine->uiwidth  = uirotbounds.max_x - uirotbounds.min_x + 1;
	Machine->uiheight = uirotbounds.max_y - uirotbounds.min_y + 1;
	Machine->uixmin = uirotbounds.min_x;
	Machine->uiymin = uirotbounds.min_y;

	/* rebuild the font */
	builduifont();
}



/*-------------------------------------------------
	erase_screen - erase the screen
-------------------------------------------------*/

static void erase_screen(void)
{
	fillbitmap(Machine->scrbitmap, get_black_pen(), NULL);
	schedule_full_refresh();
}



#if 0
#pragma mark -
#pragma mark FONTS & TEXT
#endif

/*-------------------------------------------------
	builduifont - build the user interface fonts
-------------------------------------------------*/

struct GfxElement *builduifont(void)
{
	struct GfxLayout layout = uifontlayout;
	struct GfxElement *font;
	int temp, i;

	/* free any existing fonts */
	if (Machine->uifont)
		freegfx(Machine->uifont);
	if (Machine->uirotfont)
		freegfx(Machine->uirotfont);

	/* first decode a straight on version for games */
	Machine->uifont = font = decodegfx(hankaku_font5x10, &layout);
	Machine->uifontwidth = layout.width;
	Machine->uifontheight = layout.height;

	/* apply swappage */
	if (Machine->ui_orientation & ORIENTATION_SWAP_XY)
	{
		for (i=0; i<2*MAX_UIFONT_SIZE; i++)
			temp = layout.xoffset[i], layout.xoffset[i] = layout.yoffset[i], layout.yoffset[i] = temp;

		temp = layout.width;
		layout.width = layout.height;
		layout.height = temp;
	}

	/* apply xflip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_X)
	{
		for (i = 0; i < layout.width/2; i++)
			temp = layout.xoffset[i], layout.xoffset[i] = layout.xoffset[layout.width - 1 - i], layout.xoffset[layout.width - 1 - i] = temp;
	}

	/* apply yflip */
	if (Machine->ui_orientation & ORIENTATION_FLIP_Y)
	{
		for (i = 0; i < layout.height/2; i++)
			temp = layout.yoffset[i], layout.yoffset[i] = layout.yoffset[layout.height - 1 - i], layout.yoffset[layout.height - 1 - i] = temp;
	}

	/* decode rotated font */
	Machine->uirotfont = decodegfx(hankaku_font5x10, &layout);

	/* set the raw and rotated character width/height */
	uirawcharwidth  = layout.width;
	uirawcharheight = layout.height;
	uirotcharwidth  = (Machine->ui_orientation & ORIENTATION_SWAP_XY) ? layout.height : layout.width;
	uirotcharheight = (Machine->ui_orientation & ORIENTATION_SWAP_XY) ? layout.width : layout.height;

	/* set up the bogus colortable */
	if (font)
	{
		static pen_t colortable[2*2];

		/* colortable will be set at run time */
		font->colortable = colortable;
		font->total_colors = 2;
		Machine->uirotfont->colortable = colortable;
		Machine->uirotfont->total_colors = 2;
	}

	return font;
}



/*-------------------------------------------------
	ui_drawchar - draw a rotated character
-------------------------------------------------*/

void ui_drawchar(int ch, int color, int sx, int sy)
{
	struct rectangle bounds;

#ifdef MESS
	extern int skip_this_frame;
	skip_this_frame = 0;
#endif /* MESS */

	/* construct a rectangle in rotated coordinates, then transform it */
	bounds.min_x = sx + uirotbounds.min_x;
	bounds.min_y = sy + uirotbounds.min_y;
	bounds.max_x = bounds.min_x + uirotcharwidth - 1;
	bounds.max_y = bounds.min_y + uirotcharheight - 1;
	ui_rot2raw_rect(&bounds);

	/* now render */
	drawgfx(Machine->scrbitmap, Machine->uirotfont, ch, color, 0, 0, bounds.min_x, bounds.min_y, &uirawbounds, TRANSPARENCY_NONE, 0);

	/* mark dirty */
	ui_markdirty(&bounds);
}



/*-------------------------------------------------
	ui_text_ex - draw a string to the screen
-------------------------------------------------*/

static void ui_text_ex(const char *buf_begin, const char *buf_end, int x, int y, int color)
{
	for ( ; buf_begin != buf_end; ++buf_begin)
	{
		ui_drawchar(*buf_begin, color, x, y);
		x += uirotcharwidth;
	}
}



/*-------------------------------------------------
	ui_text_ex - draw a string to the screen
-------------------------------------------------*/

void ui_text(const char *buf, int x, int y)
{
	ui_text_ex(buf, buf + strlen(buf), x, y, UI_COLOR_NORMAL);
}



/*-------------------------------------------------
	displaytext - display a series of text lines
-------------------------------------------------*/

void displaytext(const struct DisplayText *dt)
{
   /* loop until we run out of descriptors */
   for ( ; dt->text; dt++)
   {
	  ui_text_ex(dt->text, dt->text + strlen(dt->text), dt->x, dt->y, dt->color);
   }
}



/*-------------------------------------------------
	multiline_extract - extract one line from a
	multiline buffer; return the number of
	characters in the line; pbegin points to the
	start of the next line
-------------------------------------------------*/

static unsigned multiline_extract(const char **pbegin, const char *end, unsigned maxchars)
{
	const char *begin = *pbegin;
	unsigned numchars = 0;

	/* loop until we hit the end or max out */
	while (begin != end && numchars < maxchars)
	{
		/* if we hit an EOL, strip it and return the current count */
		if (*begin == '\n')
		{
			*pbegin = begin + 1; /* strip final space */
			return numchars;
		}

		/* if we hit a space, word wrap */
		else if (*begin == ' ')
		{
			/* find the end of this word */
			const char* word_end = begin + 1;
			while (word_end != end && *word_end != ' ' && *word_end != '\n')
				++word_end;

			/* if that pushes us past the max, truncate here */
			if (numchars + word_end - begin > maxchars)
			{
				/* if we have at least one character, strip the space */
				if (numchars)
				{
					*pbegin = begin + 1;
					return numchars;
				}

				/* otherwise, take as much as we can */
				else
				{
					*pbegin = begin + maxchars;
					return maxchars;
				}
			}

			/* advance to the end of this word */
			numchars += word_end - begin;
			begin = word_end;
		}

		/* for all other chars, just increment */
		else
		{
			++numchars;
			++begin;
		}
	}

	/* make sure we always make forward progress */
	if (begin != end && (*begin == '\n' || *begin == ' '))
		++begin;
	*pbegin = begin;
	return numchars;
}



/*-------------------------------------------------
	multiline_size - compute the output size of a
	multiline string
-------------------------------------------------*/

static void multiline_size(int *dx, int *dy, const char *begin, const char *end, unsigned maxchars)
{
	unsigned rows = 0;
	unsigned cols = 0;

	/* extract lines until the end, counting rows and tracking the max columns */
	while (begin != end)
	{
		unsigned len;
		len = multiline_extract(&begin, end, maxchars);
		if (len > cols)
			cols = len;
		++rows;
	}

	/* return the final result scaled by the char size */
	*dx = cols * uirotcharwidth;
	*dy = (rows - 1) * 3*uirotcharheight/2 + uirotcharheight;
}



/*-------------------------------------------------
	multilinebox_size - compute the output size of
	a multiline string with box
-------------------------------------------------*/

static void multilinebox_size(int *dx, int *dy, const char *begin, const char *end, unsigned maxchars)
{
	/* standard computation, plus an extra char width and height */
	multiline_size(dx, dy, begin, end, maxchars);
	*dx += uirotcharwidth;
	*dy += uirotcharheight;
}



/*-------------------------------------------------
	ui_multitext_ex - display a multiline string
-------------------------------------------------*/

static void ui_multitext_ex(const char *begin, const char *end, unsigned maxchars, int x, int y, int color)
{
	/* extract lines until the end */
	while (begin != end)
	{
		const char *line_begin = begin;
		unsigned len = multiline_extract(&begin, end, maxchars);
		ui_text_ex(line_begin, line_begin + len, x, y, color);
		y += 3*uirotcharheight/2;
	}
}



/*-------------------------------------------------
	ui_multitextbox_ex - display a multiline
	string with box
-------------------------------------------------*/

static void ui_multitextbox_ex(const char *begin, const char *end, unsigned maxchars, int x, int y, int dx, int dy, int color)
{
	/* draw the box first */
	ui_drawbox(x, y, dx, dy);

	/* indent by half a character */
	x += uirotcharwidth/2;
	y += uirotcharheight/2;

	/* draw the text */
	ui_multitext_ex(begin, end, maxchars, x, y, color);
}



/*-------------------------------------------------
	ui_drawbox - draw a black box with white border
-------------------------------------------------*/

void ui_drawbox(int leftx, int topy, int width, int height)
{
	struct rectangle bounds, tbounds;
	pen_t black, white;

	/* make a rect and clip it */
	bounds.min_x = uirotbounds.min_x + leftx;
	bounds.min_y = uirotbounds.min_y + topy;
	bounds.max_x = bounds.min_x + width - 1;
	bounds.max_y = bounds.min_y + height - 1;
	sect_rect(&bounds, &uirotbounds);

	/* pick colors from the colortable */
	black = Machine->uirotfont->colortable[0];
	white = Machine->uirotfont->colortable[1];

	/* top edge */
	tbounds = bounds;
	tbounds.max_y = tbounds.min_y;
	ui_rot2raw_rect(&tbounds);
	fillbitmap(Machine->scrbitmap, white, &tbounds);

	/* bottom edge */
	tbounds = bounds;
	tbounds.min_y = tbounds.max_y;
	ui_rot2raw_rect(&tbounds);
	fillbitmap(Machine->scrbitmap, white, &tbounds);

	/* left edge */
	tbounds = bounds;
	tbounds.max_x = tbounds.min_x;
	ui_rot2raw_rect(&tbounds);
	fillbitmap(Machine->scrbitmap, white, &tbounds);

	/* right edge */
	tbounds = bounds;
	tbounds.min_x = tbounds.max_x;
	ui_rot2raw_rect(&tbounds);
	fillbitmap(Machine->scrbitmap, white, &tbounds);

	/* fill in the middle with black */
	tbounds = bounds;
	tbounds.min_x++;
	tbounds.min_y++;
	tbounds.max_x--;
	tbounds.max_y--;
	ui_rot2raw_rect(&tbounds);
	fillbitmap(Machine->scrbitmap, black, &tbounds);

	/* mark things dirty */
	ui_rot2raw_rect(&bounds);
	ui_markdirty(&bounds);
}



/*-------------------------------------------------
	drawbar - draw a thermometer bar
-------------------------------------------------*/
//static void drawbar(int leftx, int topy, int width, int height, int percentage, int default_percentage)
//{
//	struct rectangle bounds, tbounds;
//	UINT32 black, white;
//
//	/* make a rect and orient/clip it */
//	bounds.min_x = uirotbounds.min_x + leftx;
//	bounds.min_y = uirotbounds.min_y + topy;
//	bounds.max_x = bounds.min_x + width - 1;
//	bounds.max_y = bounds.min_y + height - 1;
//	sect_rect(&bounds, &uirotbounds);
//
//	/* pick colors from the colortable */
//	black = Machine->uirotfont->colortable[0];
//	white = Machine->uirotfont->colortable[1];
//
//	/* draw the top default percentage marker */
//	tbounds = bounds;
//	tbounds.min_x += (width - 1) * default_percentage / 100;
//	tbounds.max_x = tbounds.min_x;
//	tbounds.max_y = tbounds.min_y + height / 8;
//	ui_rot2raw_rect(&tbounds);
//	fillbitmap(Machine->scrbitmap, white, &tbounds);
//
//	/* draw the bottom default percentage marker */
//	tbounds = bounds;
//	tbounds.min_x += (width - 1) * default_percentage / 100;
//	tbounds.max_x = tbounds.min_x;
//	tbounds.min_y = tbounds.max_y - height / 8;
//	ui_rot2raw_rect(&tbounds);
//	fillbitmap(Machine->scrbitmap, white, &tbounds);
//
//	/* draw the top line of the bar */
//	tbounds = bounds;
//	tbounds.min_y += height / 8;
//	tbounds.max_y = tbounds.min_y;
//	ui_rot2raw_rect(&tbounds);
//	fillbitmap(Machine->scrbitmap, white, &tbounds);
//
//	/* draw the bottom line of the bar */
//	tbounds = bounds;
//	tbounds.max_y -= height / 8;
//	tbounds.min_y = tbounds.max_y;
//	ui_rot2raw_rect(&tbounds);
//	fillbitmap(Machine->scrbitmap, white, &tbounds);
//
//	/* fill in the percentage */
//	tbounds = bounds;
//	tbounds.max_x = tbounds.min_x + (width - 1) * percentage / 100;
//	tbounds.min_y += height / 8;
//	tbounds.max_y -= height / 8;
//	ui_rot2raw_rect(&tbounds);
//	fillbitmap(Machine->scrbitmap, white, &tbounds);
//
//	/* mark things dirty */
//	ui_rot2raw_rect(&bounds);
//	ui_markdirty(&bounds);
//}



void ui_displaymenu(const char **items,const char **subitems,char *flag,int selected,int arrowize_subitem)
{
	struct DisplayText dt[256];
	int curr_dt;
	const char *lefthilight 	= ui_getstring (UI_lefthilight);
	const char *righthilight 	= ui_getstring (UI_righthilight);
	const char *uparrow 		= ui_getstring (UI_uparrow);
	const char *downarrow 		= ui_getstring (UI_downarrow);
	const char *leftarrow 		= ui_getstring (UI_leftarrow);
	const char *rightarrow 		= ui_getstring (UI_rightarrow);
	int i,count,len,maxlen,highlen;
	int leftoffs,topoffs,visible,topitem;
	int selected_long;


	i = 0;
	maxlen = 0;
	highlen = uirotwidth / uirotcharwidth;
	while (items[i])
	{
		len = 3 + strlen(items[i]);
		if (subitems && subitems[i])
			len += 2 + strlen(subitems[i]);
		if (len > maxlen && len <= highlen)
			maxlen = len;
		i++;
	}
	count = i;

	visible = uirotheight / (3 * uirotcharheight / 2) - 1;
	topitem = 0;
	if (visible > count) visible = count;
	else
	{
		topitem = selected - visible / 2;
		if (topitem < 0) topitem = 0;
		if (topitem > count - visible) topitem = count - visible;
	}

	leftoffs = (uirotwidth - maxlen * uirotcharwidth) / 2;
	topoffs = (uirotheight - (3 * visible + 1) * uirotcharheight / 2) / 2;

	/* black background */
	ui_drawbox(leftoffs,topoffs,maxlen * uirotcharwidth,(3 * visible + 1) * uirotcharheight / 2);

	selected_long = 0;
	curr_dt = 0;
	for (i = 0;i < visible;i++)
	{
		int item = i + topitem;

		if (i == 0 && item > 0)
		{
			dt[curr_dt].text = uparrow;
			dt[curr_dt].color = UI_COLOR_NORMAL;
			dt[curr_dt].x = (uirotwidth - uirotcharwidth * strlen(uparrow)) / 2;
			dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
			curr_dt++;
		}
		else if (i == visible - 1 && item < count - 1)
		{
			dt[curr_dt].text = downarrow;
			dt[curr_dt].color = UI_COLOR_NORMAL;
			dt[curr_dt].x = (uirotwidth - uirotcharwidth * strlen(downarrow)) / 2;
			dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
			curr_dt++;
		}
		else
		{
			if (subitems && subitems[item])
			{
				int sublen;
				len = strlen(items[item]);
				dt[curr_dt].text = items[item];
				dt[curr_dt].color = UI_COLOR_NORMAL;
				dt[curr_dt].x = leftoffs + 3*uirotcharwidth/2;
				dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
				curr_dt++;
				sublen = strlen(subitems[item]);
				if (sublen > maxlen-5-len)
				{
					dt[curr_dt].text = "...";
					sublen = strlen(dt[curr_dt].text);
					if (item == selected)
						selected_long = 1;
				} else {
					dt[curr_dt].text = subitems[item];
				}
				/* If this item is flagged, draw it in inverse print */
				dt[curr_dt].color = (flag && flag[item]) ? UI_COLOR_INVERSE : UI_COLOR_NORMAL;
				dt[curr_dt].x = leftoffs + uirotcharwidth * (maxlen-1-sublen) - uirotcharwidth/2;
				dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
				curr_dt++;
			}
			else
			{
				dt[curr_dt].text = items[item];
				dt[curr_dt].color = UI_COLOR_NORMAL;
				dt[curr_dt].x = (uirotwidth - uirotcharwidth * strlen(items[item])) / 2;
				dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
				curr_dt++;
			}
		}
	}

	i = selected - topitem;
	if (subitems && subitems[selected] && arrowize_subitem)
	{
		if (arrowize_subitem & 1)
		{
			int sublen;

			len = strlen(items[selected]);

			dt[curr_dt].text = leftarrow;
			dt[curr_dt].color = UI_COLOR_NORMAL;

			sublen = strlen(subitems[selected]);
			if (sublen > maxlen-5-len)
				sublen = strlen("...");

			dt[curr_dt].x = leftoffs + uirotcharwidth * (maxlen-2 - sublen) - uirotcharwidth/2 - 1;
			dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
			curr_dt++;
		}
		if (arrowize_subitem & 2)
		{
			dt[curr_dt].text = rightarrow;
			dt[curr_dt].color = UI_COLOR_NORMAL;
			dt[curr_dt].x = leftoffs + uirotcharwidth * (maxlen-1) - uirotcharwidth/2;
			dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
			curr_dt++;
		}
	}
	else
	{
		dt[curr_dt].text = righthilight;
		dt[curr_dt].color = UI_COLOR_NORMAL;
		dt[curr_dt].x = leftoffs + uirotcharwidth * (maxlen-1) - uirotcharwidth/2;
		dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
		curr_dt++;
	}
	dt[curr_dt].text = lefthilight;
	dt[curr_dt].color = UI_COLOR_NORMAL;
	dt[curr_dt].x = leftoffs + uirotcharwidth/2;
	dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
	curr_dt++;

	dt[curr_dt].text = 0;	/* terminate array */

	displaytext(dt);

	if (selected_long)
	{
		int long_dx;
		int long_dy;
		int long_x;
		int long_y;
		unsigned long_max;

		long_max = (uirotwidth / uirotcharwidth) - 2;
		multilinebox_size(&long_dx,&long_dy,subitems[selected],subitems[selected] + strlen(subitems[selected]), long_max);

		long_x = uirotwidth - long_dx;
		long_y = topoffs + (i+1) * 3*uirotcharheight/2;

		/* if too low display up */
		if (long_y + long_dy > uirotheight)
			long_y = topoffs + i * 3*uirotcharheight/2 - long_dy;

		ui_multitextbox_ex(subitems[selected],subitems[selected] + strlen(subitems[selected]), long_max, long_x,long_y,long_dx,long_dy, UI_COLOR_NORMAL);
	}
}


void ui_displaymessagewindow(const char *text)
{
	struct DisplayText dt[256];
	int curr_dt;
	char *c,*c2;
	int i,len,maxlen,lines;
	char textcopy[2048];
	int leftoffs,topoffs;
	int maxcols,maxrows;

	maxcols = (uirotwidth / uirotcharwidth) - 1;
	maxrows = (2 * uirotheight - uirotcharheight) / (3 * uirotcharheight);

	/* copy text, calculate max len, count lines, wrap long lines and crop height to fit */
	maxlen = 0;
	lines = 0;
	c = (char *)text;
	c2 = textcopy;
	while (*c)
	{
		len = 0;
		while (*c && *c != '\n')
		{
			*c2++ = *c++;
			len++;
			if (len == maxcols && *c != '\n')
			{
				/* attempt word wrap */
				char *csave = c, *c2save = c2;
				int lensave = len;

				/* back up to last space or beginning of line */
				while (*c != ' ' && *c != '\n' && c > text)
					--c, --c2, --len;

				/* if no space was found, hard wrap instead */
				if (*c != ' ')
					c = csave, c2 = c2save, len = lensave;
				else
					c++;

				*c2++ = '\n'; /* insert wrap */
				break;
			}
		}

		if (*c == '\n')
			*c2++ = *c++;

		if (len > maxlen) maxlen = len;

		lines++;
		if (lines == maxrows)
			break;
	}
	*c2 = '\0';

	maxlen += 1;

	leftoffs = (uirotwidth - uirotcharwidth * maxlen) / 2;
	if (leftoffs < 0) leftoffs = 0;
	topoffs = (uirotheight - (3 * lines + 1) * uirotcharheight / 2) / 2;

	/* black background */
	ui_drawbox(leftoffs,topoffs,maxlen * uirotcharwidth,(3 * lines + 1) * uirotcharheight / 2);

	curr_dt = 0;
	c = textcopy;
	i = 0;
	while (*c)
	{
		c2 = c;
		while (*c && *c != '\n')
			c++;

		if (*c == '\n')
		{
			*c = '\0';
			c++;
		}

		if (*c2 == '\t')	/* center text */
		{
			c2++;
			dt[curr_dt].x = (uirotwidth - uirotcharwidth * (c - c2)) / 2;
		}
		else
			dt[curr_dt].x = leftoffs + uirotcharwidth/2;

		dt[curr_dt].text = c2;
		dt[curr_dt].color = UI_COLOR_NORMAL;
		dt[curr_dt].y = topoffs + (3*i+1)*uirotcharheight/2;
		curr_dt++;

		i++;
	}

	dt[curr_dt].text = 0;	/* terminate array */

	displaytext(dt);
}



static void showcharset(void)
{
	int i;
	char buf[80];
	int mode,bank,color,firstdrawn;
	int palpage;
	int changed;
	int total_colors = 0;
	pen_t *colortable = NULL;
	int cpx=0,cpy,skip_chars=0,skip_tmap=0;
	int tilemap_xpos = 0;
	int tilemap_ypos = 0;

	mode = 0;
	bank = 0;
	color = 0;
	firstdrawn = 0;
	palpage = 0;

	changed = 1;

	do
	{
		static const struct rectangle fullrect = { 0, 10000, 0, 10000 };

		/* mark the whole thing dirty */
		ui_markdirty(&fullrect);

		switch (mode)
		{
			case 0: /* palette or clut */
			{
				if (bank == 0)	/* palette */
				{
					total_colors = Machine->drv->total_colors;
					colortable = Machine->pens;
					strcpy(buf,"PALETTE");
				}
				else if (bank == 1) /* clut */
				{
					total_colors = Machine->drv->color_table_len;
					colortable = Machine->remapped_colortable;
					strcpy(buf,"CLUT");
				}
				else
				{
					buf[0] = 0;
					total_colors = 0;
					colortable = 0;
				}

				/*if (changed) -- temporary */
				{
					erase_screen();

					if (total_colors)
					{
						int sx,sy,colors;
						int column_heading_max;
						struct bounds;

						colors = total_colors - 256 * palpage;
						if (colors > 256) colors = 256;

						/* min(colors, 16) */
						if (colors < 16)
							column_heading_max = colors;
						else
							column_heading_max = 16;

						for (i = 0;i < column_heading_max;i++)
						{
							char bf[40];

							sx = 3*uirotcharwidth + (uirotcharwidth*4/3)*(i % 16);
							sprintf(bf,"%X",i);
							ui_text(bf,sx,2*uirotcharheight);
							if (16*i < colors)
							{
								sy = 3*uirotcharheight + (uirotcharheight)*(i % 16);
								sprintf(bf,"%3X",i+16*palpage);
								ui_text(bf,0,sy);
							}
						}

						for (i = 0;i < colors;i++)
						{
							struct rectangle bounds;
							bounds.min_x = uirotbounds.min_x + 3*uirotcharwidth + (uirotcharwidth*4/3)*(i % 16);
							bounds.min_y = uirotbounds.min_y + 2*uirotcharheight + (uirotcharheight)*(i / 16) + uirotcharheight;
							bounds.max_x = bounds.min_x + uirotcharwidth*4/3 - 1;
							bounds.max_y = bounds.min_y + uirotcharheight - 1;
							ui_rot2raw_rect(&bounds);
							fillbitmap(Machine->scrbitmap, colortable[i + 256*palpage], &bounds);
						}
					}
					else
						ui_text("N/A",3*uirotcharwidth,2*uirotcharheight);

					ui_text(buf,0,0);
					changed = 0;
				}

				break;
			}
			case 1: /* characters */
			{
				int crotwidth = (Machine->ui_orientation & ORIENTATION_SWAP_XY) ? Machine->gfx[bank]->height : Machine->gfx[bank]->width;
				int crotheight = (Machine->ui_orientation & ORIENTATION_SWAP_XY) ? Machine->gfx[bank]->width : Machine->gfx[bank]->height;
				cpx = uirotwidth / crotwidth;
				if (cpx == 0) cpx = 1;
				cpy = (uirotheight - uirotcharheight) / crotheight;
				if (cpy == 0) cpy = 1;
				skip_chars = cpx * cpy;
				/*if (changed) -- temporary */
				{
					int flipx,flipy;
					int lastdrawn=0;

					erase_screen();

					/* validity check after char bank change */
					if (firstdrawn >= Machine->gfx[bank]->total_elements)
					{
						firstdrawn = Machine->gfx[bank]->total_elements - skip_chars;
						if (firstdrawn < 0) firstdrawn = 0;
					}

					flipx = 0;
					flipy = 0;

					for (i = 0; i+firstdrawn < Machine->gfx[bank]->total_elements && i<cpx*cpy; i++)
					{
						struct rectangle bounds;
						bounds.min_x = (i % cpx) * crotwidth + uirotbounds.min_x;
						bounds.min_y = uirotcharheight + (i / cpx) * crotheight + uirotbounds.min_y;
						bounds.max_x = bounds.min_x + crotwidth - 1;
						bounds.max_y = bounds.min_y + crotheight - 1;
						ui_rot2raw_rect(&bounds);

						drawgfx(Machine->scrbitmap,Machine->gfx[bank],
								i+firstdrawn,color,  /*sprite num, color*/
								flipx,flipy,bounds.min_x,bounds.min_y,
								0,Machine->gfx[bank]->colortable ? TRANSPARENCY_NONE : TRANSPARENCY_NONE_RAW,0);

						lastdrawn = i+firstdrawn;
					}

					sprintf(buf,"GFXSET %d COLOR %2X CODE %X-%X",bank,color,firstdrawn,lastdrawn);
					ui_text(buf,0,0);
					changed = 0;
				}

				break;
			}
			case 2: /* Tilemaps */
			{
				/*if (changed) -- temporary */
				{
					UINT32 tilemap_width, tilemap_height;
					tilemap_nb_size (bank, &tilemap_width, &tilemap_height);
					while (tilemap_xpos < 0)
						tilemap_xpos += tilemap_width;
					tilemap_xpos %= tilemap_width;

					while (tilemap_ypos < 0)
						tilemap_ypos += tilemap_height;
					tilemap_ypos %= tilemap_height;

					erase_screen();
					tilemap_nb_draw(Machine->scrbitmap, bank, tilemap_xpos, tilemap_ypos);
					sprintf(buf, "TILEMAP %d (%dx%d)  X:%d  Y:%d", bank, tilemap_width, tilemap_height, tilemap_xpos, tilemap_ypos);
					ui_text(buf,0,0);
					changed = 0;
					skip_tmap = 0;
				}
				break;
			}
		}

		update_video_and_audio();

		if (code_pressed(KEYCODE_LCONTROL) || code_pressed(KEYCODE_RCONTROL))
		{
			skip_chars = cpx;
			skip_tmap = 8;
		}
		if (code_pressed(KEYCODE_LSHIFT) || code_pressed(KEYCODE_RSHIFT))
		{
			skip_chars = 1;
			skip_tmap = 1;
		}


		if (input_ui_pressed_repeat(IPT_UI_RIGHT,8))
		{
			int next_bank, next_mode;
			int jumped;

			next_mode = mode;
			next_bank = bank+1;
			do {
				jumped = 0;
				switch (next_mode)
				{
					case 0:
						if (next_bank == 2 || Machine->drv->color_table_len == 0)
						{
							jumped = 1;
							next_mode++;
							next_bank = 0;
						}
						break;
					case 1:
						if (next_bank == MAX_GFX_ELEMENTS || !Machine->gfx[next_bank])
						{
							jumped = 1;
							next_mode++;
							next_bank = 0;
						}
						break;
					case 2:
						if (next_bank == tilemap_count())
							next_mode = -1;
						break;
				}
			}	while (jumped);
			if (next_mode != -1 )
			{
				bank = next_bank;
				mode = next_mode;
//				firstdrawn = 0;
				changed = 1;
			}
		}

		if (input_ui_pressed_repeat(IPT_UI_LEFT,8))
		{
			int next_bank, next_mode;

			next_mode = mode;
			next_bank = bank-1;
			while(next_bank < 0 && next_mode >= 0)
			{
				next_mode = next_mode - 1;
				switch (next_mode)
				{
					case 0:
						if (Machine->drv->color_table_len == 0)
							next_bank = 0;
						else
							next_bank = 1;
						break;
					case 1:
						next_bank = MAX_GFX_ELEMENTS-1;
						while (next_bank >= 0 && !Machine->gfx[next_bank])
							next_bank--;
						break;
					case 2:
						next_bank = tilemap_count() - 1;
						break;
				}
			}
			if (next_mode != -1 )
			{
				bank = next_bank;
				mode = next_mode;
//				firstdrawn = 0;
				changed = 1;
			}
		}

		if (code_pressed_memory_repeat(KEYCODE_PGDN,4))
		{
			switch (mode)
			{
				case 0:
				{
					if (256 * (palpage + 1) < total_colors)
					{
						palpage++;
						changed = 1;
					}
					break;
				}
				case 1:
				{
					if (firstdrawn + skip_chars < Machine->gfx[bank]->total_elements)
					{
						firstdrawn += skip_chars;
						changed = 1;
					}
					break;
				}
				case 2:
				{
					if (skip_tmap)
						tilemap_ypos -= skip_tmap;
					else
						tilemap_ypos -= Machine->scrbitmap->height/4;
					changed = 1;
					break;
				}
			}
		}

		if (code_pressed_memory_repeat(KEYCODE_PGUP,4))
		{
			switch (mode)
			{
				case 0:
				{
					if (palpage > 0)
					{
						palpage--;
						changed = 1;
					}
					break;
				}
				case 1:
				{
					firstdrawn -= skip_chars;
					if (firstdrawn < 0) firstdrawn = 0;
					changed = 1;
					break;
				}
				case 2:
				{
					if (skip_tmap)
						tilemap_ypos += skip_tmap;
					else
						tilemap_ypos += Machine->scrbitmap->height/4;
					changed = 1;
					break;
				}
			}
		}

		if (code_pressed_memory_repeat(KEYCODE_D,4))
		{
			switch (mode)
			{
				case 2:
				{
					if (skip_tmap)
						tilemap_xpos -= skip_tmap;
					else
						tilemap_xpos -= Machine->scrbitmap->width/4;
					changed = 1;
					break;
				}
			}
		}

		if (code_pressed_memory_repeat(KEYCODE_G,4))
		{
			switch (mode)
			{
				case 2:
				{
					if (skip_tmap)
						tilemap_xpos += skip_tmap;
					else
						tilemap_xpos += Machine->scrbitmap->width/4;
					changed = 1;
					break;
				}
			}
		}

		if (input_ui_pressed_repeat(IPT_UI_UP,6))
		{
			switch (mode)
			{
				case 1:
				{
					if (color < Machine->gfx[bank]->total_colors - 1)
					{
						color++;
						changed = 1;
					}
					break;
				}
			}
		}

		if (input_ui_pressed_repeat(IPT_UI_DOWN,6))
		{
			switch (mode)
			{
				case 0:
					break;
				case 1:
				{
					if (color > 0)
					{
						color--;
						changed = 1;
					}
				}
			}
		}

		if (input_ui_pressed(IPT_UI_SNAPSHOT))
			save_screen_snapshot(Machine->scrbitmap);
	} while (!input_ui_pressed(IPT_UI_SHOW_GFX) &&
			!input_ui_pressed(IPT_UI_CANCEL));

	schedule_full_refresh();
}



static int switchmenu(int selected, UINT32 switch_name, UINT32 switch_setting)
{
	#define MAX_DIP_SWITCHES 256
	const char *menu_item[MAX_DIP_SWITCHES+2];	// 2 more for "return to mainmenu" and end-of-list marker
	const char *menu_subitem[MAX_DIP_SWITCHES+2];
	struct InputPort *entry[MAX_DIP_SWITCHES];
	char flag[40];
	int i,sel;
	struct InputPort *in;
	int total;
	int arrowize;


	sel = selected - 1;


	in = Machine->input_ports;

	total = 0;
	while (in->type != IPT_END && total < MAX_DIP_SWITCHES)
	{
		if (in->type == switch_name && input_port_active(in))
		{
			entry[total] = in;
			menu_item[total] = input_port_name(in);

			total++;
		}

		in++;
	}

	if (total == 0) return 0;

	menu_item[total] 	= ui_getstring (UI_returntomain);
	menu_item[total + 1] = 0;	/* terminate array */
	total++;


	for (i = 0;i < total;i++)
	{
		flag[i] = 0; /* TODO: flag the dip if it's not the real default */
		if (i < total - 1)
		{
			in = entry[i] + 1;
			while (in->type == switch_setting &&
					(in->default_value != entry[i]->default_value || !input_port_condition(in)))
				in++;

			if (in->type != switch_setting)
				menu_subitem[i] 	= "INVALID";
			else menu_subitem[i] = input_port_name(in);
		}
		else menu_subitem[i] = 0;	/* no subitem */
	}

	arrowize = 0;
	if (sel < total - 1)
	{
		in = entry[sel] + 1;
		while (in->type == switch_setting &&
				(in->default_value != entry[sel]->default_value || !input_port_condition(in)))
			in++;

		if (in->type != switch_setting)
			/* invalid setting: revert to a valid one */
			arrowize |= 1;
		else
		{
			in--;
			while (in->type == switch_setting && !input_port_condition(in))
				in--;
			if (in->type == switch_setting)
				arrowize |= 1;
		}
	}
	if (sel < total - 1)
	{
		in = entry[sel] + 1;
		while (in->type == switch_setting &&
				(in->default_value != entry[sel]->default_value || !input_port_condition(in)))
			in++;

		if (in->type != switch_setting)
			/* invalid setting: revert to a valid one */
			arrowize |= 2;
		else
		{
			in++;
			while (in->type == switch_setting && !input_port_condition(in))
				in++;
			if (in->type == switch_setting)
				arrowize |= 2;
		}
	}

	ui_displaymenu(menu_item,menu_subitem,flag,sel,arrowize);

	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
		sel = (sel + 1) % total;

	if (input_ui_pressed_repeat(IPT_UI_UP,8))
		sel = (sel + total - 1) % total;

	if (input_ui_pressed_repeat(IPT_UI_RIGHT,8))
	{
		if (sel < total - 1)
		{
			in = entry[sel] + 1;
			while (in->type == switch_setting &&
					(in->default_value != entry[sel]->default_value || !input_port_condition(in)))
				in++;

			if (in->type != switch_setting)
				/* invalid setting: revert to a valid one */
				entry[sel]->default_value = (entry[sel]+1)->default_value & entry[sel]->mask;
			else
			{
				in++;
				while (in->type == switch_setting && !input_port_condition(in))
					in++;
				if (in->type == switch_setting)
					entry[sel]->default_value = in->default_value & entry[sel]->mask;
			}

			/* tell updatescreen() to clean after us (in case the window changes size) */
			schedule_full_refresh();

			input_port_settings_modify =1; //TMK
		}
	}

	if (input_ui_pressed_repeat(IPT_UI_LEFT,8))
	{
		if (sel < total - 1)
		{
			in = entry[sel] + 1;
			while (in->type == switch_setting &&
					(in->default_value != entry[sel]->default_value || !input_port_condition(in)))
				in++;

			if (in->type != switch_setting)
				/* invalid setting: revert to a valid one */
				entry[sel]->default_value = (entry[sel]+1)->default_value & entry[sel]->mask;
			else
			{
				in--;
				while (in->type == switch_setting && !input_port_condition(in))
					in--;
				if (in->type == switch_setting)
					entry[sel]->default_value = in->default_value & entry[sel]->mask;
			}

			/* tell updatescreen() to clean after us (in case the window changes size) */
			schedule_full_refresh();

			input_port_settings_modify =1; //TMK
		}
	}

	if (input_ui_pressed(IPT_UI_SELECT))
	{
		if (sel == total - 1) sel = -1;
	}

	if (input_ui_pressed(IPT_UI_CANCEL))
		sel = -1;

	if (input_ui_pressed(IPT_UI_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}



static int setdipswitches(int selected)
{
	return switchmenu(selected, IPT_DIPSWITCH_NAME, IPT_DIPSWITCH_SETTING);
}


/* This flag is used for record OR sequence of key/joy */
/* when is !=0 the first sequence is record, otherwise the first free */
/* it's used byt setdefkeysettings, setdefjoysettings, setkeysettings, setjoysettings */
static int record_first_insert = 1;

//#define MAX_PORT_ENTRIES		1000
#define MAX_PORT_ENTRIES		512

static char menu_item_buffer[MAX_PORT_ENTRIES][96];
static char menu_subitem_buffer[MAX_PORT_ENTRIES][96];

static int setdefcodesettings(int selected)
{
	static input_seq_t starting_seq;
	static int menugroup = -1;
	const char *menu_item[MAX_PORT_ENTRIES];
	const char *menu_subitem[MAX_PORT_ENTRIES];
	input_seq_t *entry[MAX_PORT_ENTRIES];
	input_seq_t *defentry[MAX_PORT_ENTRIES];
	UINT8 analog[MAX_PORT_ENTRIES];
	char flag[MAX_PORT_ENTRIES];
	int i,sel;
	struct InputPortDefinition *in, *indef;
	int total;

	sel = selected - 1;


	if (Machine->input_ports == 0)
		return 0;

	if (menugroup == -1)
	{
		for (total = 0; total < IPG_TOTAL_GROUPS; total++)
			menu_item[total] = ui_getstring(UI_uigroup + total);
		menu_item[total++] = ui_getstring (UI_returntomain);
		menu_item[total] = 0;

		ui_displaymenu(menu_item,0,0,sel,0);
	}
	else
	{
		in = get_input_port_list();
		indef = get_input_port_list_backup();

		total = 0;

		while (in->type != IPT_END)
		{
			if (in->name != 0 && in->type != IPT_UNKNOWN && in->type != IPT_DIPSWITCH_NAME && in->name[0] != 0 && in->group == menugroup)
			{
				analog[total] = port_type_is_analog(in->type);
				if (!analog[total])
				{
					entry[total] = &in->defaultseq;
					defentry[total] = &indef->defaultseq;
					menu_item[total] = in->name;
					total++;
				}
				else
				{
					entry[total] = &in->defaultseq;
					defentry[total] = &indef->defaultseq;
					sprintf(menu_item_buffer[total], "%s Analog", in->name);
					menu_item[total] = menu_item_buffer[total];
					total++;

					analog[total] = 0;
					entry[total] = &in->defaultdecseq;
					defentry[total] = &indef->defaultdecseq;
					sprintf(menu_item_buffer[total], "%s Dec", in->name);
					menu_item[total] = menu_item_buffer[total];
					total++;

					analog[total] = 0;
					entry[total] = &in->defaultincseq;
					defentry[total] = &indef->defaultincseq;
					sprintf(menu_item_buffer[total], "%s Inc", in->name);
					menu_item[total] = menu_item_buffer[total];
					total++;
				}
			}

			in++;
			indef++;
		}

		if (total == 0) return 0;

		menu_item[total] = ui_getstring (UI_returntogroup);
		menu_item[total + 1] = 0;	/* terminate array */
		total++;

		for (i = 0;i < total;i++)
		{
			if (i < total - 1)
			{
				seq_name(entry[i],menu_subitem_buffer[i],sizeof(menu_subitem_buffer[0]));
				menu_subitem[i] = menu_subitem_buffer[i];
			} else
				menu_subitem[i] = 0;	/* no subitem */
			flag[i] = 0;
		}

		if (sel > SEL_MASK)   /* are we waiting for a new key? */
		{
			int ret;

			menu_subitem[sel & SEL_MASK] = "    ";
			ui_displaymenu(menu_item,menu_subitem,flag,sel & SEL_MASK,3);

			ret = seq_read_async(entry[sel & SEL_MASK],record_first_insert);

			if (ret >= 0)
			{
				sel &= SEL_MASK;

				if (ret > 0 || seq_get_1(entry[sel]) == CODE_NONE)
				{
					if (seq_get_1(&starting_seq) == CODE_NONE)
						seq_copy(entry[sel], defentry[sel]);
					else
						seq_set_1(entry[sel],CODE_NONE);
					ret = 1;
				}

				/* tell updatescreen() to clean after us (in case the window changes size) */
				schedule_full_refresh();

				record_first_insert = ret != 0;
			}

			return sel + 1;
		}


		ui_displaymenu(menu_item,menu_subitem,flag,sel,0);
	}

	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
	{
		sel = (sel + 1) % total;
		record_first_insert = 1;
	}

	if (input_ui_pressed_repeat(IPT_UI_UP,8))
	{
		sel = (sel + total - 1) % total;
		record_first_insert = 1;
	}

	if (input_ui_pressed(IPT_UI_SELECT))
	{
		if (sel == total - 1)
		{
			if (menugroup == -1)
				sel = -1;
			else
			{
				sel = menugroup;
				menugroup = -1;
			}
		}
		else if (menugroup == -1)
		{
			menugroup = sel;
			sel = 0;
		}
		else
		{
			seq_read_async_start(analog[sel]);
			seq_copy(&starting_seq,entry[sel]);

			sel |= 1 << SEL_BITS;	/* we'll ask for a key */

			/* tell updatescreen() to clean after us (in case the window changes size) */
			schedule_full_refresh();

			input_port_settings_modify =1; //TMK
		}
	}

	if (input_ui_pressed(IPT_UI_CANCEL))
	{
		if (menugroup == -1)
			sel = -1;
		else
		{
			sel = menugroup;
			menugroup = -1;
		}
	}

	if (input_ui_pressed(IPT_UI_CLEAR))
	{
		if (menugroup != -1)
		{
			if (sel < (total - 1))
			{
				if (seq_get_1(entry[sel]) == CODE_NONE)
					seq_copy(entry[sel], defentry[sel]);
				else
					seq_set_1(entry[sel], CODE_NONE);

				schedule_full_refresh();

				input_port_settings_modify =1; //TMK
			}
		}
	}

	if (input_ui_pressed(IPT_UI_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		/* tell updatescreen() to clean after us */
		schedule_full_refresh();

		record_first_insert = 1;
	}

	return sel + 1;
}



static int setcodesettings(int selected)
{
	static input_code_t starting_code = CODE_NONE;
	const char *menu_item[MAX_PORT_ENTRIES];
	const char *menu_subitem[MAX_PORT_ENTRIES];
	input_seq_t *seq[MAX_PORT_ENTRIES];
	UINT8 flag[MAX_PORT_ENTRIES];
	UINT8 analog[MAX_PORT_ENTRIES];
	int i,sel;
	struct InputPort *in;
	int total;


	sel = selected - 1;


	if (Machine->input_ports == 0)
		return 0;

	in = Machine->input_ports;

	total = 0;
	while (in->type != IPT_END)
	{
		if (input_port_name(in) != NULL &&
#ifdef MESS
			(in->category == 0 || input_category_active(in->category)) &&
#endif /* MESS */
			((in->type == IPT_OTHER && in->name != IP_NAME_DEFAULT) || port_type_to_group(in->type, in->player) != IPG_INVALID))
		{
			analog[total] = port_type_is_analog(in->type);
			if (!analog[total])
			{
				seq[total] = &in->seq;
				menu_item[total] = input_port_name(in);
				seq_name(input_port_seq(in, SEQ_TYPE_STANDARD), menu_subitem_buffer[total], sizeof(menu_subitem_buffer[0]));
				menu_subitem[total] = menu_subitem_buffer[total];
				total++;
			}
			else
			{
				seq[total] = &in->seq;
				sprintf(menu_item_buffer[total], "%s Analog", input_port_name(in));
				menu_item[total] = menu_item_buffer[total];
				seq_name(input_port_seq(in, SEQ_TYPE_STANDARD), menu_subitem_buffer[total], sizeof(menu_subitem_buffer[0]));
				menu_subitem[total] = menu_subitem_buffer[total];
				total++;

				analog[total] = 0;
				seq[total] = &in->analog.decseq;
				sprintf(menu_item_buffer[total], "%s Dec", input_port_name(in));
				menu_item[total] = menu_item_buffer[total];
				seq_name(input_port_seq(in, SEQ_TYPE_DECREMENT), menu_subitem_buffer[total], sizeof(menu_subitem_buffer[0]));
				menu_subitem[total] = menu_subitem_buffer[total];
				total++;

				analog[total] = 0;
				seq[total] = &in->analog.incseq;
				sprintf(menu_item_buffer[total], "%s Inc", input_port_name(in));
				menu_item[total] = menu_item_buffer[total];
				seq_name(input_port_seq(in, SEQ_TYPE_INCREMENT), menu_subitem_buffer[total], sizeof(menu_subitem_buffer[0]));
				menu_subitem[total] = menu_subitem_buffer[total];
				total++;
			}
		}

		in++;
	}

	for (i = 0; i < total; i++)
	{
		/* If the key isn't the default, flag it */
		if (seq_get_1(seq[i]) != CODE_DEFAULT)
			flag[i] = 1;
		else
			flag[i] = 0;
	}


	if (total == 0) return 0;

	menu_item[total] = ui_getstring (UI_returntomain);
	menu_subitem[total] = 0;	/* no subitem */
	menu_item[total + 1] = 0;	/* terminate array */
	total++;

	if (sel > SEL_MASK)   /* are we waiting for a new key? */
	{
		int ret;

		menu_subitem[sel & SEL_MASK] = "    ";
		ui_displaymenu(menu_item,menu_subitem, (char *) flag,sel & SEL_MASK,3);

		ret = seq_read_async(seq[sel & SEL_MASK],record_first_insert);

		if (ret >= 0)
		{
			sel &= SEL_MASK;

			if (ret > 0 || seq_get_1(seq[sel]) == CODE_NONE)
			{
				seq_set_1(seq[sel], (starting_code == CODE_DEFAULT) ? CODE_NONE : CODE_DEFAULT);
				ret = 1;
			}

			/* tell updatescreen() to clean after us (in case the window changes size) */
			schedule_full_refresh();

			record_first_insert = ret != 0;
		}

		return sel + 1;
	}


	ui_displaymenu(menu_item,menu_subitem, (char *) flag,sel,0);

	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
	{
		sel = (sel + 1) % total;
		record_first_insert = 1;
	}

	if (input_ui_pressed_repeat(IPT_UI_UP,8))
	{
		sel = (sel + total - 1) % total;
		record_first_insert = 1;
	}

	if (input_ui_pressed(IPT_UI_SELECT))
	{
		if (sel == total - 1) sel = -1;
		else
		{
			starting_code = seq_get_1(seq[sel]);
			seq_read_async_start(analog[sel]);

			sel |= 1 << SEL_BITS;	/* we'll ask for a key */

			/* tell updatescreen() to clean after us (in case the window changes size) */
			schedule_full_refresh();

			input_port_settings_modify =1; //TMK
		}
	}

	if (input_ui_pressed(IPT_UI_CANCEL))
		sel = -1;

	if (input_ui_pressed(IPT_UI_CLEAR))
	{
		if (sel < (total - 1))
		{
			if (seq_get_1(seq[sel]) == CODE_NONE)
				seq_set_1(seq[sel], CODE_DEFAULT);
			else
				seq_set_1(seq[sel], CODE_NONE);

			schedule_full_refresh();

			input_port_settings_modify =1; //TMK
		}
	}

	if (input_ui_pressed(IPT_UI_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();

		record_first_insert = 1;
	}

	return sel + 1;
}


static int calibratejoysticks(int selected)
{
	const char *msg;
	static char buf[2048];
	int sel;
	static int calibration_started = 0;

	sel = selected - 1;

	if (calibration_started == 0)
	{
		osd_joystick_start_calibration();
		calibration_started = 1;
		strcpy (buf, "");
	}

	if (sel > SEL_MASK) /* Waiting for the user to acknowledge joystick movement */
	{
		if (input_ui_pressed(IPT_UI_CANCEL))
		{
			calibration_started = 0;
			sel = -1;
		}
		else if (input_ui_pressed(IPT_UI_SELECT))
		{
			osd_joystick_calibrate();
			sel &= SEL_MASK;
		}

		ui_displaymessagewindow(buf);
	}
	else
	{
		msg = osd_joystick_calibrate_next();
		schedule_full_refresh();
		if (msg == 0)
		{
			calibration_started = 0;
			osd_joystick_end_calibration();
			sel = -1;
		}
		else
		{
			strcpy (buf, msg);
			ui_displaymessagewindow(buf);
			sel |= 1 << SEL_BITS;
		}
	}

	if (input_ui_pressed(IPT_UI_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}

#define MAX_ANALOG_ENTRIES 80

static int settraksettings(int selected)
{
	const char *menu_item[MAX_ANALOG_ENTRIES];
	const char *menu_subitem[MAX_ANALOG_ENTRIES];
	struct InputPort *entry[MAX_ANALOG_ENTRIES];
	int i,sel;
	struct InputPort *in;
	int total,total2;
	int arrowize;


	sel = selected - 1;


	if (Machine->input_ports == 0)
		return 0;

	in = Machine->input_ports;

	/* Count the total number of analog controls */
	total = 0;
	while (in->type != IPT_END)
	{
		if (port_type_is_analog(in->type))
		{
			entry[total] = in;
			total++;
		}
		in++;
	}

	if (total == 0) return 0;

	/* Each analog control has 4 entries - key & joy delta, center delta, reverse, sensitivity */

#define ENTRIES 4

	total2 = total * ENTRIES;

	menu_item[total2] = ui_getstring (UI_returntomain);
	menu_item[total2 + 1] = 0;	/* terminate array */
	total2++;

	arrowize = 0;
	for (i = 0;i < total2;i++)
	{
		if (i < total2 - 1)
		{
			char label[MAX_ANALOG_ENTRIES][40];
			char setting[MAX_ANALOG_ENTRIES][40];
			int sensitivity,delta,centerdelta;
			int reverse;

			strcpy (label[i], input_port_name(entry[i/ENTRIES]));
			sensitivity = entry[i/ENTRIES]->analog.sensitivity;
			delta = entry[i/ENTRIES]->analog.delta;
			centerdelta = entry[i/ENTRIES]->analog.centerdelta;
			reverse = entry[i/ENTRIES]->analog.reverse;

			strcat (label[i], " ");
			switch (i%ENTRIES)
			{
				case 0:
					strcat (label[i], ui_getstring (UI_keyjoyspeed));
					sprintf(setting[i],"%d",delta);
					if (i == sel) arrowize = 3;
					break;
				case 1:
					strcat (label[i], ui_getstring (UI_centerspeed));
					sprintf(setting[i],"%d",centerdelta);
					if (i == sel) arrowize = 3;
					break;
				case 2:
					strcat (label[i], ui_getstring (UI_reverse));
					if (reverse)
						strcpy(setting[i],ui_getstring (UI_on));
					else
						strcpy(setting[i],ui_getstring (UI_off));
					if (i == sel) arrowize = 3;
					break;
				case 3:
					strcat (label[i], ui_getstring (UI_sensitivity));
					sprintf(setting[i],"%3d%%",sensitivity);
					if (i == sel) arrowize = 3;
					break;
			}

			menu_item[i] = label[i];
			menu_subitem[i] = setting[i];

			in++;
		}
		else menu_subitem[i] = 0;	/* no subitem */
	}

	ui_displaymenu(menu_item,menu_subitem,0,sel,arrowize);

	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
		sel = (sel + 1) % total2;

	if (input_ui_pressed_repeat(IPT_UI_UP,8))
		sel = (sel + total2 - 1) % total2;

	if (input_ui_pressed_repeat(IPT_UI_LEFT,8))
	{
		if(sel != total2 - 1)
		{
			if ((sel % ENTRIES) == 0)
			{
				/* keyboard/joystick delta */
				int val = entry[sel/ENTRIES]->analog.delta;

				val --;
				if (val < 1) val = 1;
				entry[sel/ENTRIES]->analog.delta = val;
			}
			else if ((sel % ENTRIES) == 1)
			{
				/* center delta */
				int val = entry[sel/ENTRIES]->analog.centerdelta;

				val --;
				if (val < 0) val = 0;
				entry[sel/ENTRIES]->analog.centerdelta = val;
			}
			else if ((sel % ENTRIES) == 2)
			{
				/* reverse */
				entry[sel/ENTRIES]->analog.reverse ^= 1;
			}
			else if ((sel % ENTRIES) == 3)
			{
				/* sensitivity */
				int val = entry[sel/ENTRIES]->analog.sensitivity;

				val --;
				if (val < 1) val = 1;
				entry[sel/ENTRIES]->analog.sensitivity = val;
			}
		}
	}

	if (input_ui_pressed_repeat(IPT_UI_RIGHT,8))
	{
		if(sel != total2 - 1)
		{
			if ((sel % ENTRIES) == 0)
			{
				/* keyboard/joystick delta */
				int val = entry[sel/ENTRIES]->analog.delta;

				val ++;
				if (val > 255) val = 255;
				entry[sel/ENTRIES]->analog.delta = val;
			}
			else if ((sel % ENTRIES) == 1)
			{
				/* center delta */
				int val = entry[sel/ENTRIES]->analog.centerdelta;

				val ++;
				if (val > 255) val = 255;
				entry[sel/ENTRIES]->analog.centerdelta = val;
			}
			else if ((sel % ENTRIES) == 2)
			{
				/* reverse */
				entry[sel/ENTRIES]->analog.reverse ^= 1;
			}
			else if ((sel % ENTRIES) == 3)
			{
				/* sensitivity */
				int val = entry[sel/ENTRIES]->analog.sensitivity;

				val ++;
				if (val > 255) val = 255;
				entry[sel/ENTRIES]->analog.sensitivity = val;
			}
		}
	}

	if (input_ui_pressed(IPT_UI_SELECT))
	{
		if (sel == total2 - 1) sel = -1;
	}

	if (input_ui_pressed(IPT_UI_CANCEL))
		sel = -1;

	if (input_ui_pressed(IPT_UI_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}

#ifndef MESS
static int mame_stats(int selected)
{
	char temp[10];
	char buf[2048];
	int sel, i;


	sel = selected - 1;

	buf[0] = 0;

	if (dispensed_tickets)
	{
		strcat(buf, "Tickets dispensed");
		strcat(buf, ": ");
		sprintf(temp, "%d\n\n", dispensed_tickets);
		strcat(buf, temp);
	}

	for (i=0; i<COIN_COUNTERS; i++)
	{
		strcat(buf, "Coin");
		sprintf(temp, " %c: ", i+'A');
		strcat(buf, temp);
		if (!coin_count[i])
			strcat (buf, "NA");
		else
		{
			sprintf (temp, "%d", coin_count[i]);
			strcat (buf, temp);
		}
		if (coinlockedout[i])
		{
			strcat(buf, " ");
			strcat(buf, "(locked)");
			strcat(buf, "\n");
		}
		else
		{
			strcat(buf, "\n");
		}
	}

	{
		/* menu system, use the normal menu keys */
		strcat(buf,"\n\t");
		strcat(buf,ui_getstring (UI_lefthilight));
		strcat(buf," ");
		strcat(buf,ui_getstring (UI_returntomain));
		strcat(buf," ");
		strcat(buf,ui_getstring (UI_righthilight));

		ui_displaymessagewindow(buf);

		if (input_ui_pressed(IPT_UI_SELECT))
			sel = -1;

		if (input_ui_pressed(IPT_UI_CANCEL))
			sel = -1;

		if (input_ui_pressed(IPT_UI_CONFIGURE))
			sel = -2;
	}

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}
#endif

int showcopyright(void)
{
	int done;
	char buf[1000];
	char buf2[256];

	strcpy (buf, "Usage of emulators in conjunction with ROMs you don't own is forbidden by copyright law.");
	strcat (buf, "\n\n");
	sprintf(buf2, "IF YOU ARE NOT LEGALLY ENTITLED TO PLAY \"%s\" ON THIS EMULATOR, PRESS ESC.", Machine->gamedrv->description);
	strcat (buf, buf2);
	strcat (buf, "\n\n");
	strcat (buf, "Otherwise, type OK or move the joystick left then right to continue");

	setup_selected = -1;////
	done = 0;

	do
	{
		erase_screen();
		ui_drawbox(0,0,uirotwidth,uirotheight);
		ui_displaymessagewindow(buf);

		update_video_and_audio();
		if (input_ui_pressed(IPT_UI_CANCEL))
		{
			setup_selected = 0;////
			return 1;
		}
		if (code_pressed_memory(KEYCODE_O) ||
				input_ui_pressed(IPT_UI_LEFT))
			done = 1;
		if (done == 1 && (code_pressed_memory(KEYCODE_K) ||
				input_ui_pressed(IPT_UI_RIGHT)))
			done = 2;
	} while (done < 2);

	setup_selected = 0;////
	erase_screen();
	update_video_and_audio();

	return 0;
}

static int displaygameinfo(int selected)
{
	int i;
	char buf[2048];
	int sel;

	sel = selected - 1;

	sprintf(buf,"%s\n%s %s\n\nCPU:\n",Machine->gamedrv->description,Machine->gamedrv->year,Machine->gamedrv->manufacturer	);
	i = 0;
	while (i < MAX_CPU && Machine->drv->cpu[i].cpu_type)
	{
		int type,clock,count;

		type  = Machine->drv->cpu[i].cpu_type;
		clock = Machine->drv->cpu[i].cpu_clock;
		count = 1;
		i++;
		while (i < MAX_CPU
				&& Machine->drv->cpu[i].cpu_type  == type
				&& Machine->drv->cpu[i].cpu_clock == clock)
		{
			count++;
			i++;
		}

		if (count > 1)
			sprintf(&buf[strlen(buf)],"%dx",count);

		strcat(buf, cputype_name(type));

		if (clock >= 1000000)
			sprintf(&buf[strlen(buf)]," %d.%06d MHz",
					clock / 1000000,
					clock % 1000000);
		else
			sprintf(&buf[strlen(buf)]," %d.%03d kHz",
					clock / 1000,
					clock % 1000);

		strcat(buf,"\n");
	}
	strcat(buf,"\nSound:\n");

	i = 0;
	while (i < MAX_SOUND && Machine->drv->sound[i].sound_type)
	{
		const char *name;
		int type,clock,count;

		name = sndnum_name(i);
		type = Machine->drv->sound[i].sound_type;
		clock = sndnum_clock(i);
		count = 1;

		i++;

		while (i < MAX_SOUND
				&& Machine->drv->sound[i].sound_type == type
				&& sndnum_clock(i) == clock)
		{
			count++;
			i++;
		}

		if (count > 1)
			sprintf(&buf[strlen(buf)],"%dx",count);

		sprintf(&buf[strlen(buf)],"%s",name);

		if (clock)
		{
			if (clock >= 1000000)
				sprintf(&buf[strlen(buf)]," %d.%06d MHz",
						clock / 1000000,
						clock % 1000000);
			else
				sprintf(&buf[strlen(buf)]," %d.%03d kHz",
						clock / 1000,
						clock % 1000);
		}

		strcat(buf,"\n");
	}
	strcat(buf,"\nScreen Resolution:\n");
	sprintf(&buf[strlen(buf)],"%d x %d (%s) %f Hz\n",
		Machine->visible_area.max_x - Machine->visible_area.min_x + 1,
		Machine->visible_area.max_y - Machine->visible_area.min_y + 1,
		(Machine->gamedrv->flags & ORIENTATION_SWAP_XY) ? "V" : "H",
		Machine->refresh_rate);

	if (sel == -1)
	{
		/* startup info, print MAME version and ask for any key */
		/* \t means that the line will be centered */
		strcat(buf, "\n\t""MAME 0.97 for psp""\n\t""Press Any Key");
		ui_drawbox(0,0,uirotwidth,uirotheight);
		ui_displaymessagewindow(buf);

		sel = 0;
		if (code_read_async() != CODE_NONE)
			sel = -1;
	}
	else
	{
		/* menu system, use the normal menu keys */
		strcat(buf,"\n\t");
		strcat(buf,ui_getstring (UI_lefthilight));
		strcat(buf," ");
		strcat(buf,ui_getstring (UI_returntomain));
		strcat(buf," ");
		strcat(buf,ui_getstring (UI_righthilight));

		ui_displaymessagewindow(buf);

		if (input_ui_pressed(IPT_UI_SELECT))
			sel = -1;

		if (input_ui_pressed(IPT_UI_CANCEL))
			sel = -1;

		if (input_ui_pressed(IPT_UI_CONFIGURE))
			sel = -2;
	}

	if (sel == -1 || sel == -2)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}


int showgamewarnings(void)
{
	int i;
	char buf[2048];

	if (Machine->gamedrv->flags &
			(GAME_NOT_WORKING | GAME_UNEMULATED_PROTECTION | GAME_WRONG_COLORS | GAME_IMPERFECT_COLORS |
			  GAME_NO_SOUND | GAME_IMPERFECT_SOUND | GAME_IMPERFECT_GRAPHICS | GAME_NO_COCKTAIL))
	{
		int done;

		strcpy(buf, ui_getstring (UI_knownproblems));
		strcat(buf, "\n\n");

		if (Machine->gamedrv->flags & GAME_IMPERFECT_COLORS)
		{
			strcat(buf, ui_getstring (UI_imperfectcolors));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & GAME_WRONG_COLORS)
		{
			strcat(buf, ui_getstring (UI_wrongcolors));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & GAME_IMPERFECT_GRAPHICS)
		{
			strcat(buf, ui_getstring (UI_imperfectgraphics));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & GAME_IMPERFECT_SOUND)
		{
			strcat(buf, ui_getstring (UI_imperfectsound));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & GAME_NO_SOUND)
		{
			strcat(buf, ui_getstring (UI_nosound));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & GAME_NO_COCKTAIL)
		{
			strcat(buf, ui_getstring (UI_nococktail));
			strcat(buf, "\n");
		}

		if (Machine->gamedrv->flags & (GAME_NOT_WORKING | GAME_UNEMULATED_PROTECTION))
		{
			const struct GameDriver *maindrv;
			int foundworking;

			if (Machine->gamedrv->flags & GAME_NOT_WORKING)
			{
				strcpy(buf, ui_getstring (UI_brokengame));
				strcat(buf, "\n");
			}
			if (Machine->gamedrv->flags & GAME_UNEMULATED_PROTECTION)
			{
				strcat(buf, ui_getstring (UI_brokenprotection));
				strcat(buf, "\n");
			}

			if (Machine->gamedrv->clone_of && !(Machine->gamedrv->clone_of->flags & NOT_A_DRIVER))
				maindrv = Machine->gamedrv->clone_of;
			else maindrv = Machine->gamedrv;

			foundworking = 0;
			i = 0;
			while (drivers[i])
			{
				if (drivers[i] == maindrv || drivers[i]->clone_of == maindrv)
				{
					if ((drivers[i]->flags & (GAME_NOT_WORKING | GAME_UNEMULATED_PROTECTION)) == 0)
					{
						if (foundworking == 0)
						{
							strcat(buf,"\n\n");
							strcat(buf, ui_getstring (UI_workingclones));
							strcat(buf,"\n\n");
						}
						foundworking = 1;

						sprintf(&buf[strlen(buf)],"%s\n",drivers[i]->name);
					}
				}
				i++;
			}
		}

		strcat(buf,"\n\n");
		strcat(buf,ui_getstring (UI_typeok));

		done = 0;
		do
		{
			erase_screen();
			ui_drawbox(0,0,uirotwidth,uirotheight);
			ui_displaymessagewindow(buf);

			update_video_and_audio();
			if (input_ui_pressed(IPT_UI_CANCEL))
				return 1;
			if (code_pressed_memory(KEYCODE_O) ||
					input_ui_pressed(IPT_UI_LEFT))
				done = 1;
			if (done == 1 && (code_pressed_memory(KEYCODE_K) ||
					input_ui_pressed(IPT_UI_RIGHT)))
				done = 2;
		} while (done < 2);
	}

	erase_screen();
	update_video_and_audio();

	return 0;
}


int showgameinfo(void)
{
	/* clear the input memory */
	while (code_read_async() != CODE_NONE) {};

	while (displaygameinfo(0) == 1)
	{
		update_video_and_audio();
	}

	erase_screen();
	/* make sure that the screen is really cleared, in case autoframeskip kicked in */
	update_video_and_audio();
	update_video_and_audio();
	update_video_and_audio();
	update_video_and_audio();

	return 0;
}

#if 00
/* Word-wraps the text in the specified buffer to fit in maxwidth characters per line.
   The contents of the buffer are modified.
   Known limitations: Words longer than maxwidth cause the function to fail. */
static void wordwrap_text_buffer (char *buffer, int maxwidth)
{
	int width = 0;

	while (*buffer)
	{
		if (*buffer == '\n')
		{
			buffer++;
			width = 0;
			continue;
		}

		width++;

		if (width > maxwidth)
		{
			/* backtrack until a space is found */
			while (*buffer != ' ')
			{
				buffer--;
				width--;
			}
			if (width < 1) return;	/* word too long */

			/* replace space with a newline */
			*buffer = '\n';
		}
		else
			buffer++;
	}
}
#endif

#if 00
static int count_lines_in_buffer (char *buffer)
{
	int lines = 0;
	char c;

	while ( (c = *buffer++) )
		if (c == '\n') lines++;

	return lines;
}
/* Display lines from buffer, starting with line 'scroll', in a width x height text window */
static void display_scroll_message (int *scroll, int width, int height, char *buf)
{
	struct DisplayText dt[256];
	int curr_dt = 0;
	const char *uparrow = ui_getstring (UI_uparrow);
	const char *downarrow = ui_getstring (UI_downarrow);
	char textcopy[2048];
	char *copy;
	int leftoffs,topoffs;
	int first = *scroll;
	int buflines,showlines;
	int i;


	/* draw box */
	leftoffs = (uirotwidth - uirotcharwidth * (width + 1)) / 2;
	if (leftoffs < 0) leftoffs = 0;
	topoffs = (uirotheight - (3 * height + 1) * uirotcharheight / 2) / 2;
	ui_drawbox(leftoffs,topoffs,(width + 1) * uirotcharwidth,(3 * height + 1) * uirotcharheight / 2);

	buflines = count_lines_in_buffer (buf);
	if (first > 0)
	{
		if (buflines <= height)
			first = 0;
		else
		{
			height--;
			if (first > (buflines - height))
				first = buflines - height;
		}
		*scroll = first;
	}

	if (first != 0)
	{
		/* indicate that scrolling upward is possible */
		dt[curr_dt].text = uparrow;
		dt[curr_dt].color = UI_COLOR_NORMAL;
		dt[curr_dt].x = (uirotwidth - uirotcharwidth * strlen(uparrow)) / 2;
		dt[curr_dt].y = topoffs + (3*curr_dt+1)*uirotcharheight/2;
		curr_dt++;
	}

	if ((buflines - first) > height)
		showlines = height - 1;
	else
		showlines = height;

	/* skip to first line */
	while (first > 0)
	{
		char c;

		while ( (c = *buf++) )
		{
			if (c == '\n')
			{
				first--;
				break;
			}
		}
	}

	/* copy 'showlines' lines from buffer, starting with line 'first' */
	copy = textcopy;
	for (i = 0; i < showlines; i++)
	{
		char *copystart = copy;

		while (*buf && *buf != '\n')
		{
			*copy = *buf;
			copy++;
			buf++;
		}
		*copy = '\0';
		copy++;
		if (*buf == '\n')
			buf++;

		if (*copystart == '\t') /* center text */
		{
			copystart++;
			dt[curr_dt].x = (uirotwidth - uirotcharwidth * (copy - copystart)) / 2;
		}
		else
			dt[curr_dt].x = leftoffs + uirotcharwidth/2;

		dt[curr_dt].text = copystart;
		dt[curr_dt].color = UI_COLOR_NORMAL;
		dt[curr_dt].y = topoffs + (3*curr_dt+1)*uirotcharheight/2;
		curr_dt++;
	}

	if (showlines == (height - 1))
	{
		/* indicate that scrolling downward is possible */
		dt[curr_dt].text = downarrow;
		dt[curr_dt].color = UI_COLOR_NORMAL;
		dt[curr_dt].x = (uirotwidth - uirotcharwidth * strlen(downarrow)) / 2;
		dt[curr_dt].y = topoffs + (3*curr_dt+1)*uirotcharheight/2;
		curr_dt++;
	}

	dt[curr_dt].text = 0;	/* terminate array */

	displaytext(dt);
}
#endif


int memcard_menu(int selection)
{
	int sel;
	int menutotal = 0;
	const char *menuitem[10];
	char buf[256];
	char buf2[256];

	sel = selection - 1 ;

	sprintf(buf, "%s %03d", ui_getstring (UI_loadcard), mcd_number);
	menuitem[menutotal++] = buf;
	menuitem[menutotal++] = ui_getstring (UI_ejectcard);
	menuitem[menutotal++] = ui_getstring (UI_createcard);
	menuitem[menutotal++] = ui_getstring (UI_returntomain);
	menuitem[menutotal] = 0;

	if (mcd_action!=0)
	{
		strcpy (buf2, "\n");

		switch(mcd_action)
		{
			case 1:
				strcat (buf2, ui_getstring (UI_loadfailed));
				break;
			case 2:
				strcat (buf2, ui_getstring (UI_loadok));
				break;
			case 3:
				strcat (buf2, ui_getstring (UI_cardejected));
				break;
			case 4:
				strcat (buf2, ui_getstring (UI_cardcreated));
				break;
			case 5:
				strcat (buf2, ui_getstring (UI_cardcreatedfailed));
				strcat (buf2, "\n");
				strcat (buf2, ui_getstring (UI_cardcreatedfailed2));
				break;
			default:
				strcat (buf2, ui_getstring (UI_carderror));
				break;
		}

		strcat (buf2, "\n\n");
		ui_displaymessagewindow(buf2);
		if (input_ui_pressed(IPT_UI_SELECT))
			mcd_action = 0;
	}
	else
	{
		ui_displaymenu(menuitem,0,0,sel,0);

		if (input_ui_pressed_repeat(IPT_UI_RIGHT,8))
			mcd_number = (mcd_number + 1) % 1000;

		if (input_ui_pressed_repeat(IPT_UI_LEFT,8))
			mcd_number = (mcd_number + 999) % 1000;

		if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
			sel = (sel + 1) % menutotal;

		if (input_ui_pressed_repeat(IPT_UI_UP,8))
			sel = (sel + menutotal - 1) % menutotal;

		if (input_ui_pressed(IPT_UI_SELECT))
		{
			switch(sel)
			{
			case 0:
				memcard_intf.eject();
				if (memcard_intf.load(mcd_number))
				{
					memcard_status=1;
					memcard_number=mcd_number;
					mcd_action = 2;
				}
				else
					mcd_action = 1;
				break;
			case 1:
				memcard_intf.eject();
				mcd_action = 3;
				break;
			case 2:
				if (memcard_intf.create(mcd_number))
					mcd_action = 4;
				else
					mcd_action = 5;
				break;
#ifdef MESS
			case 3:
				memcard_manager=1;
				sel=-2;
				machine_reset();
				break;
			case 4:
				sel=-1;
				break;
#else
			case 3:
				sel=-1;
				break;
#endif


			}
		}

		if (input_ui_pressed(IPT_UI_CANCEL))
			sel = -1;

		if (input_ui_pressed(IPT_UI_CONFIGURE))
			sel = -2;

		if (sel == -1 || sel == -2)
		{
			schedule_full_refresh();
		}
	}

	return sel + 1;
}


enum { UI_SWITCH = 0,UI_DEFGROUP,UI_CODE,UI_ANALOG,UI_CALIBRATE,
		UI_STATS,UI_GAMEINFO, UI_QUIT_GAME,
		UI_CHEAT,UI_RESET,UI_MEMCARD,UI_EXIT };

#define MAX_SETUPMENU_ITEMS 16
static const char *menu_item[MAX_SETUPMENU_ITEMS];
static int menu_action[MAX_SETUPMENU_ITEMS];
static int menu_total;


static void append_menu(int uistring, int action)
{
	menu_item[menu_total] = ui_getstring(uistring);
	menu_action[menu_total++] = action;
}


static int has_dipswitches(void)
{
	struct InputPort *in;
	int num;

	/* Determine if there are any dip switches */
	num = 0;
	for (in = Machine->input_ports; in->type != IPT_END; in++)
	{
		if (in->type == IPT_DIPSWITCH_NAME && input_port_active(in))
			num++;
	}
	return num > 0;
}


static int has_analog(void)
{
	struct InputPort *in;
	int num;

	/* Determine if there are any analog controls */
	num = 0;
	for (in = Machine->input_ports; in->type != IPT_END; in++)
	{
		if (port_type_is_analog(in->type) && input_port_active(in))
			num++;
	}
	return num > 0;
}


#ifdef MESS
static int has_configurables(void)
{
	struct InputPort *in;
	int num;

	num = 0;
	for (in = Machine->input_ports; in->type != IPT_END; in++)
	{
		if (in->type == IPT_CONFIG_NAME && input_port_active(in))
			num++;
	}
	return num > 0;
}


static int has_categories(void)
{
	struct InputPort *in;
	int num;

	num = 0;
	for (in = Machine->input_ports; in->type != IPT_END; in++)
	{
		if (in->category > 0 && input_port_active(in))
			num++;
	}
	return num > 0;
}
#endif /* MESS */


static void setup_menu_init(void)
{
	menu_total = 0;

	append_menu(UI_inputgeneral, UI_DEFGROUP);
	append_menu(UI_inputspecific, UI_CODE);

	if (has_dipswitches())	append_menu(UI_dipswitches, UI_SWITCH);
	if (has_analog())		append_menu(UI_analogcontrols, UI_ANALOG);

	/* Joystick calibration possible? */
	if ((osd_joystick_needs_calibration()) != 0)
	{
		append_menu(UI_calibrate, UI_CALIBRATE);
	}

	append_menu(UI_bookkeeping, UI_STATS);
	append_menu(UI_gameinfo, UI_GAMEINFO);

	if (options.cheat)
	{
		append_menu(UI_cheat, UI_CHEAT);
	}

	if (	memcard_intf.create != NULL &&
		memcard_intf.load != NULL &&
		memcard_intf.save != NULL &&
		memcard_intf.eject != NULL)
	{
		append_menu(UI_memorycard, UI_MEMCARD);
	}

	append_menu(UI_resetgame, UI_RESET);
	append_menu(UI_quit_emulator, UI_QUIT_GAME);
	append_menu(UI_returntogame, UI_EXIT);
	menu_item[menu_total] = 0; /* terminate array */
}

extern int psp_exit;	//TMK
static int setup_menu(int selected)
{
	int sel,res=-1;
	static int menu_lastselected = 0;


	if (selected == -1)
		sel = menu_lastselected;
	else sel = selected - 1;

	if (sel > SEL_MASK)
	{
		switch (menu_action[sel & SEL_MASK])
		{
			case UI_SWITCH:			res = setdipswitches(sel >> SEL_BITS);				break;
			case UI_DEFGROUP:		res = setdefcodesettings(sel >> SEL_BITS);			break;
			case UI_CODE:			res = setcodesettings(sel >> SEL_BITS);				break;
			case UI_ANALOG:			res = settraksettings(sel >> SEL_BITS);				break;
			case UI_CALIBRATE:		res = calibratejoysticks(sel >> SEL_BITS);			break;
			case UI_STATS:			res = mame_stats(sel >> SEL_BITS);					break;
			case UI_GAMEINFO:		res = displaygameinfo(sel >> SEL_BITS);				break;
		//	case UI_QUIT_GAME:		res = displayhistory(sel >> SEL_BITS);				break;
			case UI_CHEAT:			res = cheat_menu(sel >> SEL_BITS);					break;
			case UI_MEMCARD:		res = memcard_menu(sel >> SEL_BITS);				break;
			case UI_QUIT_GAME:		psp_exit=1;	break;
		}

		if (res == -1)
		{
			menu_lastselected = sel;
			sel = -1;
		}
		else
			sel = (sel & SEL_MASK) | (res << SEL_BITS);

		return sel + 1;
	}


	ui_displaymenu(menu_item,0,0,sel,0);

	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))
		sel = (sel + 1) % menu_total;

	if (input_ui_pressed_repeat(IPT_UI_UP,8))
		sel = (sel + menu_total - 1) % menu_total;

	if (input_ui_pressed(IPT_UI_SELECT))
	{
		switch (menu_action[sel])
		{
			case UI_SWITCH:
			case UI_DEFGROUP:
			case UI_CODE:
			case UI_ANALOG:
			case UI_CALIBRATE:
			#ifndef MESS
			case UI_STATS:
			case UI_GAMEINFO:
			#else
			case UI_GAMEINFO:
			case UI_IMAGEINFO:
			case UI_FILEMANAGER:
			case UI_TAPECONTROL:
			case UI_CONFIGURATION:
			case UI_CATEGORIES:
#endif /* !MESS */
			case UI_QUIT_GAME:
			case UI_CHEAT:
			case UI_MEMCARD:
				sel |= 1 << SEL_BITS;
				schedule_full_refresh();
				break;

			case UI_RESET:
				machine_reset();
				break;

			case UI_EXIT:
				menu_lastselected = 0;
				sel = -1;
				break;
		}
	}

	if (input_ui_pressed(IPT_UI_CANCEL) ||
			input_ui_pressed(IPT_UI_CONFIGURE))
	{
		menu_lastselected = sel;
		sel = -1;
	}

	if (sel == -1)
	{
		schedule_full_refresh();
	}

	return sel + 1;
}



/*********************************************************************
  start of On Screen Display handling
*********************************************************************/
//static void displayosd(const char *text,int percentage,int default_percentage)
//{
//	struct DisplayText dt[2];
//	int avail;
//	avail = (uirotwidth / uirotcharwidth) * 19 / 20;
//	ui_drawbox((uirotwidth - uirotcharwidth * avail) / 2,
//			(uirotheight - 7*uirotcharheight/2),
//			avail * uirotcharwidth,
//			3*uirotcharheight);
//	avail--;
//	drawbar((uirotwidth - uirotcharwidth * avail) / 2,
//			(uirotheight - 3*uirotcharheight),
//			avail * uirotcharwidth,
//			uirotcharheight,
//			percentage,default_percentage);
//	dt[0].text = text;
//	dt[0].color = UI_COLOR_NORMAL;
//	dt[0].x = (uirotwidth - uirotcharwidth * strlen(text)) / 2;
//	dt[0].y = (uirotheight - 2*uirotcharheight) + 2;
//	dt[1].text = 0; /* terminate array */
//	displaytext(dt);
//}

//static void onscrd_adjuster(int increment,int arg)
//{
//	struct InputPort *in = &Machine->input_ports[arg];
//	char buf[80];
//	int value;
//	if (increment)
//	{
//		value = in->default_value & 0xff;
//		value += increment;
//		if (value > 100) value = 100;
//		if (value < 0) value = 0;
//		in->default_value = (in->default_value & ~0xff) | value;
//	}
//	value = in->default_value & 0xff;
//	sprintf(buf,"%s %d%%",in->name,value);
//	displayosd(buf,value,in->default_value >> 8);
//}

//static void onscrd_volume(int increment,int arg)
//{
//	char buf[20];
//	int attenuation;
//	if (increment)
//	{
//		attenuation = osd_get_mastervolume();
//		attenuation += increment;
//		if (attenuation > 0) attenuation = 0;
//		if (attenuation < -32) attenuation = -32;
//		osd_set_mastervolume(attenuation);
//	}
//	attenuation = osd_get_mastervolume();
//	sprintf(buf,"%s %3ddB", ui_getstring (UI_volume), attenuation);
//	displayosd(buf,100 * (attenuation + 32) / 32,100);
//}

//static void onscrd_mixervol(int increment,int arg)
//{
//	static void *driver = 0;
//	char buf[40];
//	float volume;
//	int ch;
//	int doallchannels = 0;
//	int proportional = 0;
//	if (code_pressed(KEYCODE_LSHIFT) || code_pressed(KEYCODE_RSHIFT))		doallchannels = 1;
//	if (!code_pressed(KEYCODE_LCONTROL) && !code_pressed(KEYCODE_RCONTROL))	increment *= 5;
//	if (code_pressed(KEYCODE_LALT) || code_pressed(KEYCODE_RALT))		proportional = 1;
//	if (increment)
//	{
//		if (proportional)
//		{
//			static float old_vol[100];
//			int num_vals = sound_get_user_gain_count();
//			float ratio = 1.0;
//			int overflow = 0;
//			if (driver != Machine->drv)
//			{
//				driver = (void *)Machine->drv;
//				for (ch = 0; ch < num_vals; ch++)
//					old_vol[ch] = sound_get_user_gain(ch);
//			}
//			volume = sound_get_user_gain(arg);
//			if (old_vol[arg])
//				ratio = (volume + increment * 0.02) / old_vol[arg];
//			for (ch = 0; ch < num_vals; ch++)
//			{
//				volume = ratio * old_vol[ch];
//				if (volume < 0 || volume > 2.0)
//					overflow = 1;
//			}
//			if (!overflow)
//			{
//				for (ch = 0; ch < num_vals; ch++)
//				{
//					volume = ratio * old_vol[ch];
//					sound_set_user_gain(ch,volume);
//				}
//			}
//		}
//		else
//		{
//			driver = 0; /* force reset of saved volumes */
//			volume = sound_get_user_gain(arg);
//			volume += increment * 0.02;
//			if (volume > 2.0) volume = 2.0;
//			if (volume < 0) volume = 0;
//			if (doallchannels)
//			{
//				int num_vals = sound_get_user_gain_count();
//				for (ch = 0;ch < num_vals;ch++)
//					sound_set_user_gain(ch,volume);
//			}
//			else
//				sound_set_user_gain(arg,volume);
//		}
//	}
//	volume = sound_get_user_gain(arg);
//	if (proportional)		sprintf(buf,"ALL CHANNELS Relative %4.2f", volume);
//	else if (doallchannels)	sprintf(buf,"ALL CHANNELS Volume %4.2f", volume);
//	else		sprintf(buf,"%s Volume %4.2f",sound_get_user_gain_name(arg), volume);
//	displayosd(buf,volume*50,sound_get_default_gain(arg)*50);
//}

//static void onscrd_brightness(int increment,int arg)
//{
//	char buf[20];
//	double brightness;
//	if (increment)
//	{
//		//brightness = 1.0;//palette_get_global_brightness();
//		//brightness += 0.05 * increment;
//		//if (brightness < 0.1) brightness = 0.1;
//		//if (brightness > 1.0) brightness = 1.0;
//		//palette_set_global_brightness(brightness);
//	}
//	brightness = 1.0;//palette_get_global_brightness();

//	sprintf(buf,"%s %3d%%", ui_getstring (UI_brightness), (int)(/*brightness*/1.0 * 100));
//	displayosd(buf,/*brightness*  */100,100);
//}

//static void onscrd_gamma(int increment,int arg)
//{
//	char buf[20];
//	double gamma_correction;
//	if (increment)
//	{
//		gamma_correction = palette_get_global_gamma();
//		gamma_correction += 0.05 * increment;
//		if (gamma_correction < 0.5) gamma_correction = 0.5;
//		if (gamma_correction > 2.0) gamma_correction = 2.0;
//		palette_set_global_gamma(gamma_correction);
//	}
//	gamma_correction = palette_get_global_gamma();
//	sprintf(buf,"%s %1.2f", ui_getstring (UI_gamma), /*gamma_correction*/1.0);
//	displayosd(buf,100*(/*gamma_correction*/1.0-0.5)/(2.0-0.5),100*(1.0-0.5)/(2.0-0.5));
//}


//static void onscrd_overclock(int increment,int arg)
//{
//	char buf[30];
//	double overclock;
//	int cpu, doallcpus = 0, oc;
//
//	if (code_pressed(KEYCODE_LSHIFT) || code_pressed(KEYCODE_RSHIFT))
//		doallcpus = 1;
//	if (!code_pressed(KEYCODE_LCONTROL) && !code_pressed(KEYCODE_RCONTROL))
//		increment *= 5;
//	if( increment )
//	{
//		overclock = cpunum_get_clockscale(arg);
//		overclock += 0.01 * increment;
//		if (overclock < 0.01) overclock = 0.01;
//		if (overclock > 2.0) overclock = 2.0;
//		if( doallcpus )
//			for( cpu = 0; cpu < cpu_gettotalcpu(); cpu++ )
//				cpunum_set_clockscale(cpu, overclock);
//		else
//			cpunum_set_clockscale(arg, overclock);
//	}
//	oc = 100 * cpunum_get_clockscale(arg) + 0.5;
//	if( doallcpus )
//		sprintf(buf,"ALL CPUS Overclock %3d%%", oc);
//	else
//		sprintf(buf,"Overclock CPU%d %3d%%", arg, oc);
//	displayosd(buf,oc/2,100/2);
//}

//static void onscrd_refresh(int increment,int arg)
//{
//	float delta = Machine->refresh_rate - Machine->drv->frames_per_second;
//	char buf[30];
//	increment *= 1000;
//	if (code_pressed(KEYCODE_LSHIFT) || code_pressed(KEYCODE_RSHIFT))		increment /= 10;
//	if (code_pressed(KEYCODE_LCONTROL) || code_pressed(KEYCODE_RCONTROL))	increment /= 100;
//	if (code_pressed(KEYCODE_LALT) || code_pressed(KEYCODE_LALT))			increment /= 1000;
//	if (increment)
//	{
//		float newrate;
//		delta += 0.001 * increment;
//		if (delta > 10)			delta = 10;
//		if (delta < -10)		delta = -10;
//		newrate = Machine->drv->frames_per_second;
//		if (delta != 0)
//			newrate = (floor(newrate * 1000) / 1000) + delta;
//		set_refresh_rate(newrate);
//	}
//	sprintf(buf,"%s %.3f", ui_getstring (UI_refresh_rate), Machine->refresh_rate);
//	displayosd(buf,(10 + delta) * 5,100/2);
//}

//#define MAX_OSD_ITEMS 16
//static void (*onscrd_fnc[MAX_OSD_ITEMS])(int increment,int arg);
//static int onscrd_arg[MAX_OSD_ITEMS];
//static int onscrd_total_items;

static void onscrd_init(void)
{
//	struct InputPort *in;
//	int item,ch;

//	item = 0;

//	if (Machine->sample_rate)
//	{
//		int num_vals = sound_get_user_gain_count();
//	//	onscrd_fnc[item] = onscrd_volume;
//	//	onscrd_arg[item] = 0;
//	//	item++;

//		for (ch = 0;ch < num_vals;ch++)
//		{
//			onscrd_fnc[item] = onscrd_mixervol;
//			onscrd_arg[item] = ch;
//			item++;
//		}
//	}

//	for (in = Machine->input_ports; in && in->type != IPT_END; in++)
//		if ((in->type & 0xff) == IPT_ADJUSTER)
//		{
//			onscrd_fnc[item] = onscrd_adjuster;
//			onscrd_arg[item] = in - Machine->input_ports;
//			item++;
//		}

//	if (options.cheat)
//	{
//		for (ch = 0;ch < cpu_gettotalcpu();ch++)
//		{
//			onscrd_fnc[item] = onscrd_overclock;
//			onscrd_arg[item] = ch;
//			item++;
//		}
//		onscrd_fnc[item] = onscrd_refresh;
//		onscrd_arg[item] = ch;
//		item++;
//	}

	//onscrd_fnc[item] = onscrd_brightness;
	//onscrd_arg[item] = 0;
	//item++;

	//onscrd_fnc[item] = onscrd_gamma;
	//onscrd_arg[item] = 0;
	//item++;

//	onscrd_total_items = item;
}

static int on_screen_display(int selected)
{
//	int increment,sel;
//	static int lastselected = 0;
//	if (selected == -1)		sel = lastselected;
//	else sel = selected - 1;
//	increment = 0;
//	if (input_ui_pressed_repeat(IPT_UI_LEFT,8))		increment = -1;
//	if (input_ui_pressed_repeat(IPT_UI_RIGHT,8))	increment = 1;
//	if (input_ui_pressed_repeat(IPT_UI_DOWN,8))		sel = (sel + 1) % onscrd_total_items;
//	if (input_ui_pressed_repeat(IPT_UI_UP,8))		sel = (sel + onscrd_total_items - 1) % onscrd_total_items;
//	(*onscrd_fnc[sel])(increment,onscrd_arg[sel]);
//	lastselected = sel;
//	if (input_ui_pressed(IPT_UI_ON_SCREEN_DISPLAY))
//	{
//		sel = -1;
//		schedule_full_refresh();
//	}
//	return sel + 1;
	return 0;
}

/*********************************************************************

  end of On Screen Display handling

*********************************************************************/


static void displaymessage(const char *text)
{
	struct DisplayText dt[2];
	int avail;


	if (uirotwidth < uirotcharwidth * strlen(text))
	{
		ui_displaymessagewindow(text);
		return;
	}

	avail = strlen(text)+2;

	ui_drawbox((uirotwidth - uirotcharwidth * avail) / 2,
			uirotheight - 3*uirotcharheight,
			avail * uirotcharwidth,
			2*uirotcharheight);

	dt[0].text = text;
	dt[0].color = UI_COLOR_NORMAL;
	dt[0].x = (uirotwidth - uirotcharwidth * strlen(text)) / 2;
	dt[0].y = uirotheight - 5*uirotcharheight/2;
	dt[1].text = 0; /* terminate array */
	displaytext(dt);
}


static char messagetext[200];
static int messagecounter;

void CLIB_DECL usrintf_showmessage(const char *text,...)
{
	va_list arg;
	va_start(arg,text);
	vsprintf(messagetext,text,arg);
	va_end(arg);
	messagecounter = 2 * Machine->refresh_rate;
}

void CLIB_DECL usrintf_showmessage_secs(int seconds, const char *text,...)
{
	va_list arg;
	va_start(arg,text);
	vsprintf(messagetext,text,arg);
	va_end(arg);
	messagecounter = seconds * Machine->refresh_rate;
}

void do_loadsave(int request_loadsave)
{
	int file = 0;

	mame_pause(1);

	do
	{
		input_code_t code;

		if (request_loadsave == LOADSAVE_SAVE)
			displaymessage("Select position to save to");
		else
			displaymessage("Select position to load from");

		update_video_and_audio();
		reset_partial_updates();

		if (input_ui_pressed(IPT_UI_CANCEL))
			break;

		code = code_read_async();
		if (code != CODE_NONE)
		{
			if (code >= KEYCODE_A && code <= KEYCODE_Z)
				file = 'a' + (code - KEYCODE_A);
			else if (code >= KEYCODE_0 && code <= KEYCODE_9)
				file = '0' + (code - KEYCODE_0);
			else if (code >= KEYCODE_0_PAD && code <= KEYCODE_9_PAD)
				file = '0' + (code - KEYCODE_0);
		}
	}
	while (!file);

	mame_pause(0);

	if (file > 0)
	{
		if (request_loadsave == LOADSAVE_SAVE)
			usrintf_showmessage("Save to position %c", file);
		else
			usrintf_showmessage("Load from position %c", file);
		cpu_loadsave_schedule(request_loadsave, file);
	}
	else
	{
		if (request_loadsave == LOADSAVE_SAVE)
			usrintf_showmessage("Save cancelled");
		else
			usrintf_showmessage("Load cancelled");
	}
}


void ui_show_fps_temp(double seconds)
{
	if (!showfps)
		showfpstemp = (int)(seconds * Machine->refresh_rate);
}

void ui_show_fps_set(int show)
{
	if (show)
	{
		showfps = 1;
	}
	else
	{
		showfps = 0;
		showfpstemp = 0;
		schedule_full_refresh();
	}
}

int ui_show_fps_get(void)
{
	return showfps || showfpstemp;
}

void ui_show_profiler_set(int show)
{
	if (show)
	{
		show_profiler = 1;
		profiler_start();
	}
	else
	{
		show_profiler = 0;
		profiler_stop();
		schedule_full_refresh();
	}
}

int ui_show_profiler_get(void)
{
	return show_profiler;
}

void ui_display_fps(void)
{
	const char *text, *end;
	char textbuf[256];
	int done = 0;
	int x, y = 0;
	/* remember which area we cover so that we can
	   schedule a full refresh if it gets smaller */
	int len_hash = 0;
	static int old_len_hash = -1;

	/* if we're not currently displaying, skip it */
	if (!showfps && !showfpstemp)
	{
		/* reset covered area vars */
		old_len_hash = -1;
		return;
	}

	/* get the current FPS text */
	text = osd_get_fps_text(mame_get_performance_info());

	/* loop over lines */
	while (!done)
	{
		/* find the end of this line and copy it to the text buf */
		end = strchr(text, '\n');
		if (end)
		{
			memcpy(textbuf, text, end - text);
			textbuf[end - text] = 0;
			text = end + 1;
		}
		else
		{
			strcpy(textbuf, text);
			done = 1;
		}

		/* render */
		x = uirotwidth - strlen(textbuf) * uirotcharwidth;
		ui_text(textbuf, x, y);
		y += uirotcharheight;
		len_hash += (y / uirotcharheight) * x;
	}

	if ((old_len_hash != -1) &&
		(old_len_hash != len_hash))
	{
		schedule_full_refresh();
		ui_markdirty(&uirawbounds);
	}

	old_len_hash = len_hash;

	/* update the temporary FPS display state */
	if (showfpstemp)
	{
		showfpstemp--;
		if (!showfps && showfpstemp == 0)
			schedule_full_refresh();
	}
}




int handle_user_interface(void)
{
#ifdef MESS
	extern int mess_pause_for_ui;
#endif

	/* if the user pressed F12, save the screen to a file */
//	if (input_ui_pressed(IPT_UI_SNAPSHOT))	save_screen_snapshot(Machine->scrbitmap);

	/* This call is for the cheat, it must be called once a frame */
	if (options.cheat) DoCheat();

	/* if the user pressed ESC, stop the emulation */
	/* but don't quit if the setup menu is on screen */
//TMK	if (setup_selected == 0 && input_ui_pressed(IPT_UI_CANCEL))
//		return 1;

	if (setup_selected == 0 && input_ui_pressed(IPT_UI_CONFIGURE))
	{
		setup_selected = -1;
		if (osd_selected != 0)
		{
			osd_selected = 0;	/* disable on screen display */
			schedule_full_refresh();
		}
	}
	if (setup_selected != 0) setup_selected = setup_menu(setup_selected);

#ifdef MAME_DEBUG
	if (!mame_debug)
#endif
		if (osd_selected == 0 && input_ui_pressed(IPT_UI_ON_SCREEN_DISPLAY))
		{
			osd_selected = -1;
			if (setup_selected != 0)
			{
				setup_selected = 0; /* disable setup menu */
				schedule_full_refresh();
			}
		}
	if (osd_selected != 0) osd_selected = on_screen_display(osd_selected);

	/* if the user pressed F3, reset the emulation */
	if (input_ui_pressed(IPT_UI_RESET_MACHINE))
		machine_reset();

	if (input_ui_pressed(IPT_UI_SAVE_STATE))
		do_loadsave(LOADSAVE_SAVE);

	if (input_ui_pressed(IPT_UI_LOAD_STATE))
		do_loadsave(LOADSAVE_LOAD);

#ifndef MESS
	if (single_step || input_ui_pressed(IPT_UI_PAUSE)) /* pause the game */
	{
#else
	if (setup_selected)
		mess_pause_for_ui = 1;

	if (single_step || input_ui_pressed(IPT_UI_PAUSE) || mess_pause_for_ui) /* pause the game */
	{
#endif
/*		osd_selected = 0;	   disable on screen display, since we are going   */
							/* to change parameters affected by it */

		if (single_step == 0)
			mame_pause(1);

		while (!input_ui_pressed(IPT_UI_PAUSE))
		{
			profiler_mark(PROFILER_VIDEO);
			if (osd_skip_this_frame() == 0)
			{
				/* keep calling vh_screenrefresh() while paused so we can stuff */
				/* debug code in there */
				draw_screen();
			}
			profiler_mark(PROFILER_END);

			if (input_ui_pressed(IPT_UI_SNAPSHOT))
				save_screen_snapshot(Machine->scrbitmap);


			if (input_ui_pressed(IPT_UI_SAVE_STATE))
				do_loadsave(LOADSAVE_SAVE);

			if (input_ui_pressed(IPT_UI_LOAD_STATE))
				do_loadsave(LOADSAVE_LOAD);

			/* if the user pressed F4, show the character set */
			if (input_ui_pressed(IPT_UI_SHOW_GFX))
				showcharset();

//TMK			if (setup_selected == 0 && input_ui_pressed(IPT_UI_CANCEL))
//				return 1;

			if (setup_selected == 0 && input_ui_pressed(IPT_UI_CONFIGURE))
			{
				setup_selected = -1;
				if (osd_selected != 0)
				{
					osd_selected = 0;	/* disable on screen display */
					schedule_full_refresh();
				}
			}
			if (setup_selected != 0) setup_selected = setup_menu(setup_selected);

#ifdef MAME_DEBUG
			if (!mame_debug)
#endif
				if (osd_selected == 0 && input_ui_pressed(IPT_UI_ON_SCREEN_DISPLAY))
				{
					osd_selected = -1;
					if (setup_selected != 0)
					{
						setup_selected = 0; /* disable setup menu */
						schedule_full_refresh();
					}
				}
			if (osd_selected != 0) osd_selected = on_screen_display(osd_selected);

			if (options.cheat) DisplayWatches();

			/* show popup message if any */
			if (messagecounter > 0)
			{
				displaymessage(messagetext);

				if (--messagecounter == 0)
					schedule_full_refresh();
			}

			update_video_and_audio();
			reset_partial_updates();

#ifdef MESS
			if (!setup_selected && mess_pause_for_ui)
			{
				mess_pause_for_ui = 0;
				break;
			}
#endif /* MESS */
		}

		if (code_pressed(KEYCODE_LSHIFT) || code_pressed(KEYCODE_RSHIFT))
			single_step = 1;
		else
		{
			single_step = 0;
			mame_pause(0);
		}

		schedule_full_refresh();
	}

#if defined(__sgi) && !defined(MESS)
	game_paused = 0;
#endif

	/* show popup message if any */
	if (messagecounter > 0)
	{
		displaymessage(messagetext);

		if (--messagecounter == 0)
			schedule_full_refresh();
	}


	if (input_ui_pressed(IPT_UI_SHOW_PROFILER))
	{
		ui_show_profiler_set(!ui_show_profiler_get());
	}

	if (show_profiler) profiler_show();


	/* show FPS display? */
	if (input_ui_pressed(IPT_UI_SHOW_FPS))
	{
		/* toggle fps */
		ui_show_fps_set(!ui_show_fps_get());
	}


	/* if the user pressed F4, show the character set */
	if (input_ui_pressed(IPT_UI_SHOW_GFX))
	{
		osd_sound_enable(0);
		showcharset();
		osd_sound_enable(1);
	}

	/* if the user pressed F1 and this is a lightgun game, toggle the crosshair */
	if (input_ui_pressed(IPT_UI_TOGGLE_CROSSHAIR))
	{
		drawgfx_toggle_crosshair();
	}

	/* add the FPS counter */
	ui_display_fps();
	return 0;
}


void init_user_interface(void)
{
	extern int snapno;	/* in common.c */

	snapno = 0; /* reset snapshot counter */

	/* clear the input memory */
	while (code_read_async() != CODE_NONE) {};

	setup_menu_init();
	setup_selected = 0;

	onscrd_init();
	osd_selected = 0;

	jukebox_selected = -1;

	single_step = 0;
}

int onscrd_active(void)
{
	return osd_selected;
}

int setup_active(void)
{
	return setup_selected;
}

#if defined(__sgi) && !defined(MESS)

/* Return if the game is paused or not */
int is_game_paused(void)
{
	return game_paused;
}

#endif
