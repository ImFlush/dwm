/* See LICENSE file for copyright and license details. */

static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int gappx     = 10;       /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "monospace:size=10";
static char dmenufont[]       = "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";

static char statusfgcolor[]			= "#ffffff";
static char statusbgcolor[]			= "#000000";
static char tagSelfgcolor[]			= "#ffffff";
static char tagSelbgcolor[]			= "#000000";
static char tagNormfgcolor[]		= "#ffffff";
static char tagNormbgcolor[]		= "#000000";
static char infoSelfgcolor[]		= "#ffffff";
static char infoSelbgcolor[]		= "#000000";
static char infoNormfgcolor[]		= "#ffffff";
static char infoNormbgcolor[]		= "#000000";
static char invfgcolor[]			= "#ffffff";
static char invbgcolor[]			= "#000000";
static char invbordercolor[]		= "#787878";
static char *colors[][3] = {
		/*               fg           bg           border   */
		[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },

		[SchemeStatus]  = { statusfgcolor, statusbgcolor,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
		[SchemeTagsSel] = { tagSelfgcolor, tagSelbgcolor,  "#000000"  }, 	// Tagbar left selected {text,background,not used but cannot be empty}
	    [SchemeTagsNorm]= { tagNormfgcolor, tagNormbgcolor,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	    [SchemeInfoSel] = { infoSelfgcolor, infoSelbgcolor,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	    [SchemeInfoNorm]= { infoSelfgcolor, infoSelbgcolor,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
	    [SchemeInv]  	= { invfgcolor, invbgcolor, invbordercolor}		// tagbar selected on second monitor
};

/* tagging */
static const char *tags[] = { "󰞷", "", "", "󰨞", "󰓇", "", "󰔁", "󰙯", "󰓓" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	// class     			instance  title           tags mask  isfloating  isterminal  noswallow  monitor
	{ "Brave-browser",  	NULL,     NULL,       	  1 << 1,  	 0,          0, 		  0, 		-1 },
	{ "code-oss",   		NULL,     NULL,           1 << 3,    0,          0,           0,        -1 },
	{ "Spotify",   			NULL,     NULL,           1 << 4,    0,          0,           0,        -1 },
	{ "Signal",    			NULL,     NULL,           1 << 5,    0,          0,           0,        -1 },
	{ "Whatsapp-for-linux", NULL,     NULL,           1 << 5,    0,          0,           0,        -1 },
	{ "TelegramDesktop",    NULL,     NULL,           1 << 6,    0,          0,           0,        -1 },
	{ "discord",  			NULL,     NULL,       	  1 << 7,    0,          0,			  0,	    -1 },
	{ "Steam",  			NULL,     NULL,       	  1 << 8,    0,          0, 		  0,   		-1 },

	{ "St",      			NULL,     NULL,           0,         0,          0,           0,        -1 },
	{ NULL,      			NULL,     "Event Tester", 0,         0,          0,           1,        -1 } /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },

		{ "gappx",          	INTEGER, &gappx },
		{ "swallowfloating",   	INTEGER, &swallowfloating },
		{ "statusfgcolor",      STRING,  &statusfgcolor },
		{ "statusbgcolor",      STRING,  &statusbgcolor },
		{ "tagSelfgcolor",      STRING,  &tagSelfgcolor },
		{ "tagSelbgcolor",      STRING,  &tagSelbgcolor },
		{ "tagNormfgcolor",     STRING,  &tagNormfgcolor },
		{ "tagNormbgcolor",     STRING,  &tagNormbgcolor },
		{ "infoSelfgcolor",     STRING,  &infoSelfgcolor },
		{ "infoSelbgcolor",     STRING,  &infoSelbgcolor },
		{ "infoNormfgcolor",    STRING,  &infoNormfgcolor },
		{ "infoNormbgcolor",    STRING,  &infoNormbgcolor },
		{ "invfgcolor",        	STRING,  &invfgcolor },
		{ "invbgcolor",        	STRING,  &invbgcolor },
		{ "invbordercolor",     STRING,  &invbordercolor },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ControlMask,      		XK_Return, spawnsshaware,  {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_j,      setcfact,       {.f = +0.25} },
	{ MODKEY|ControlMask,           XK_k,      setcfact,       {.f = -0.25} },
	{ MODKEY,             			XK_r,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             			XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ControlMask,           XK_comma,  keeptagmon,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, keeptagmon,     {.i = +1 } },
	{ MODKEY|ShiftMask,           	XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,           	XK_k,      pushup,         {0} },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

