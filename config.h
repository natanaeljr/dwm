/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int rmaster            = 0;        /* 1 means master-area is initially on the right */
static const char *fonts[]          = { "Fira Code:style:medium:size=10", "JetBrainsMono Nerd Font:sytle:medium:size=11", "Noto Color Emoji:size=11", "monospace:size=10" };
static const char dmenufont[]       = "Fira Code:style:medium:size=10";
// Gray color palette
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#888888";
static const char col_gray4[]       = "#bbbbbb";
static const char col_gray5[]       = "#eeeeee";
static const char col_night[]       = "#3b4252";
static const char col_frost[]       = "#88c0d0";
static const char col_green1[]      = "#458b00";
static const char col_green2[]      = "#bced91";
static const char col_green3[]      = "#76ee00";
static const char col_green4[]      = "#83f52c";
// Summer Splash color palette
static const char col_splash1[]     = "#05445E";
static const char col_splash2[]     = "#189AB4";
static const char col_splash4[]     = "#75E6DA";
static const char col_splash5[]     = "#D4F1F4";
// One dark color palette
static const char col_one_black[]   = "#1e222a";
static const char col_one_gray2[]   = "#2e323a";
static const char col_one_gray3[]   = "#545862";
static const char col_one_gray4[]   = "#6d8dad";
static const char col_one_gray5[]   = "#7b8b9c";
static const char col_one_blue[]    = "#61afef";
// Actual colors
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	// [SchemeNorm] = { col_one_white, col_one_black, col_gray2 },
	// [SchemeSel]  = { col_one_white, col_gray2,     col_gray3 },
  [SchemeNorm] = { col_one_gray5, col_one_black, col_one_gray2 },
  [SchemeSel]  = { col_one_gray2, col_one_blue,  col_one_gray3 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance            title                            tags mask  isfloating  canfocus  monitor  float x,y,w,h        floatborderpx*/
	{ "Gimp",             NULL,               NULL,                            0,         1,          1,        -1,      -1,  -1,  -1,  -1,   -1 },
	{ "Firefox",          NULL,               NULL,                            1 << 8,    0,          1,        -1,      -1,  -1,  -1,  -1,   -1 },
	{ NULL,               NULL,               "Picture in picture",            ~0,        1,          1,        -1,      -1,  -1,  -1,  -1,   -1 },
	{ "Pavucontrol",      NULL,               "Volume Control",                0,         1,          1,        -1,      300,200,  -1,  -1,   -1 },
	{ "Alacritty",        NULL,               "Alacritty Floating",            0,         1,          1,        -1,      280,130,  -1,  -1,    1 },
	{ NULL              , NULL,               "Calculator",                    0,         1,          1,        -1,      -1,  -1,  -1,  -1,   -1 },
	{ NULL              , NULL,               "Screenshot",                    0,         1,          1,        -1,      -1,  -1,  -1,  -1,   -1 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },  /* fibonacci patch */
	{ "[\\]",     dwindle }, /* fibonacci patch */
	{ "TTT",      bstack },      /* bottomstach patch */
	{ "===",      bstackhoriz }, /* bottomstach patch */
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray4, "-sb", col_night, "-sf", col_gray5, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *floattermcmd[]  = { "alacritty", "--title='Alacritty Floating'", NULL };
static const char *initcomp[] = { "picom", "-b", NULL };
static const char *killcomp[] = { "killall", "picom", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = floattermcmd } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = initcomp } },
	{ MODKEY,                       XK_z,      spawn,          {.v = killcomp } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_c,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_m,      togglermaster,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ControlMask,           XK_minus,  setborderpx,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_equal,	 setborderpx,    {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_equal,  setborderpx,    {.i = 0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_x,      quit,           {0} },
	{ MODKEY,                       XK_n,    scratchpad_show,  {0} },
	{ MODKEY|ShiftMask,             XK_s,    scratchpad_hide,  {0} },
	{ MODKEY,                       XK_s,    scratchpad_remove,{0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_Down,   moveresize,     {.v = "0x 1y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_Up,     moveresize,     {.v = "0x -1y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_Right,  moveresize,     {.v = "1x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_Left,   moveresize,     {.v = "-1x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ShiftMask|Mod1Mask,    XK_Down,   moveresize,     {.v = "0x 0y 0w 1h" } },
	{ MODKEY|ShiftMask|Mod1Mask,    XK_Up,     moveresize,     {.v = "0x 0y 0w -1h" } },
	{ MODKEY|ShiftMask|Mod1Mask,    XK_Right,  moveresize,     {.v = "0x 0y 1w 0h" } },
	{ MODKEY|ShiftMask|Mod1Mask,    XK_Left,   moveresize,     {.v = "0x 0y -1w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

