/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 4;       /* vert inner gap between windows */
static const unsigned int gappoh    = 4;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* for font: yay ttf-fixedsys-excelsior-linux */
static const char *fonts[]          = {"Fixedsys Excelsior:size=16" };
static const char dmenufont[]       = "Fixedsys Excelsior:size=16";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_maroon[]      = "#5f0000";
static const char col_gold[]        = "#cea200";
static const char *colors[][3]      = {
	/*               fg         bg           border   */
	[SchemeNorm] = { col_gray3, col_gray1,   col_gray2 },
	[SchemeSel]  = { col_gray4, col_maroon,  col_gold  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "arandr",   NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]= tile",      tile },    /* first entry is default */
	{ "[M] monocle",      monocle },
	{ "[\\] dwindle",     dwindle },
	{ "[@] spiral",      spiral },
	{ "H[] deck",      deck },
	{ "TTT bstack",      bstack },
	{ "=== bstackhoriz",      bstackhoriz },
	{ "HHH grid",      grid },
	{ "### nrowgrid",      nrowgrid },
	{ "--- horizgrid",      horizgrid },
	{ "::: gaplessgrid",      gaplessgrid },
	{ "|M| ctrmaster",      centeredmaster },
	{ ">M> ctrfltmaster",      centeredfloatingmaster },
	{ "><> NULL",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
/* ASZ: Use Mod1Mask for Alt, Mod4Mask for Meta */ 
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* symbols for media keys - use xev in terminal to extract them */
/* these were taken from a Fujitsu P728 laptop */
#define MUTEKEY       0x1008ff12
#define VOLUPKEY      0x1008ff13
#define VOLDOWNKEY    0x1008ff11
#define BRIGHTUPKEY   0x1008ff02
#define BRIGHTDOWNKEY 0x1008ff03

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb",
	col_gray1, "-nf", col_gray3, "-sb", col_gold, "-sf", col_gray1, NULL };
static const char *addcmd[] = { "dmenu_add", "-m", dmenumon, "-fn", dmenufont, "-nb",
	col_gray1, "-nf", col_gray3, "-sb", col_gold, "-sf", col_gray1, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *slockcmd[] = {"slock",NULL};
static const char *togglemutecmd[] = {"pulsemixer", "--toggle-mute", NULL};
static const char *volumeupcmd[] = {"pulsemixer", "--change-volume", "+5", NULL};
static const char *volumedowncmd[] = {"pulsemixer", "--change-volume", "-5", NULL};
static const char *screenshotcmd[] = {"sc", NULL};
static const char *brightnessupcmd[] = {"xbacklight", "-inc", "15%", NULL};
static const char *brightnessdowncmd[] = {"xbacklight", "-dec", "15%", NULL};
static const char *playpausecmd[] = {"cmus-remote", "--pause", NULL};
static const char *nextcmd[] = {"cmus-remote", "--next", NULL};
static const char *prevcmd[] = {"cmus-remote", "--prev", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_a,      spawn,          {.v = addcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_BackSpace,spawn,        {.v = slockcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Tab,      focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	//{ MODKEY,                       XK_Tab,    view,           {0} },
	//{ MODKEY|ShiftMask,             XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[12]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[13]} },
	/*{ MODKEY,                       XK_space,  setlayout,      {0} },*/
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,       quit,           {0} },
	{ MODKEY,                       XK_F2,      spawn,          {.v = togglemutecmd} },
	{ MODKEY,                       XK_F3,      spawn,          {.v = volumedowncmd} },
	{ MODKEY,                       XK_F4,      spawn,          {.v = volumeupcmd} },
	{ MODKEY,                       XK_F7,      spawn,          {.v = brightnessdowncmd} },
	{ MODKEY,                       XK_F8,      spawn,          {.v = brightnessupcmd} },
	{ MODKEY,                       XK_F11,     spawn,          {.v = screenshotcmd} },
	{ MODKEY|ShiftMask,             XK_F2,      spawn,          {.v = playpausecmd} },
	{ MODKEY|ShiftMask,             XK_F3,      spawn,          {.v = prevcmd} },
	{ MODKEY|ShiftMask,             XK_F4,      spawn,          {.v = nextcmd} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
/* TODO modify first two so that
 *  - Button1 (left click) ClkLtSymbol goes back to default layout (tile) 
 *  - Button3 (right click) ClkLtSymbol cycles between all layouts
 */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

