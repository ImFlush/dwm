/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  		= 3;        /* border pixel of windows */
static unsigned int gappx     		= 10;       /* gaps between windows */
static unsigned int snap     		= 32;       /* snap pixel */
static int swallowfloating   		= 0;        /* 1 means swallow floating windows by default */
static int showbar           	 	= 1;        /* 0 means no bar */
static int topbar            	 	= 1;        /* 0 means bottom bar */
static char *fonts[]         	 	= { "SFMono-Regular:size=15" };
static char dmenufont[]      	 	= "SFMono-Regular:size=15";
static char dmenu_font[]       		= "#f5f5dc";
static char dmenu_focused[]    		= "#ff34b3";
static char dmenu_background[]      = "#000000";
static char col_font[]        		= "#f5f5dc";
static char col_focused[]     		= "#ff34b3";
static char col_invfocused[]  		= "#50162C";
static char col_unfocused[]   		= "#b03060";
static char col_background[]  		= "#050307";
static char *colors[][3]      		= {
	/*               		fg         bg         border   */
	[SchemeNorm] 	= { col_font, col_unfocused, col_background},
	[SchemeSel]  	= { col_font, col_focused,  col_font},		
	[SchemeStatus]  = { col_font, col_unfocused,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel] = { col_font, col_focused,  "#000000"  }, 	// Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]= { col_font, col_unfocused,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel] = { col_font, col_unfocused,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]= { col_font, col_unfocused,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
    [SchemeInv]  	= { col_font, col_invfocused, col_font}		// tagbar on second monitor
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
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", dmenu_background, "-nf", dmenu_font, "-sb", dmenu_focused, "-sf", dmenu_font, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "borderpx",          	INTEGER, &borderpx },
		{ "gappx",          	INTEGER, &gappx },
		{ "snap",          		INTEGER, &snap },
		{ "swallowfloating",   	INTEGER, &swallowfloating },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		//{ "fonts",     		STRING,  &fonts },
		{ "dmenufont",     		STRING,  &dmenufont },
		{ "col_font",        	STRING,  &col_font },
		{ "col_focused",        STRING,  &col_focused },
		{ "col_invfocused",     STRING,  &col_invfocused },
		{ "col_unfocused",     	STRING,  &col_unfocused },
		{ "col_background",     STRING,  &col_background },
		{ "dmenu_font",        	STRING,  &dmenu_font },
		{ "dmenu_focused",      STRING,  &dmenu_focused },
		{ "dmenu_background",	STRING,  &dmenu_background },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
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
//	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
//	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
//	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
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
	{ MODKEY|ShiftMask,           	XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,           	XK_k,      pushup,         {0} },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	{ MODKEY|ControlMask,           XK_comma,  keeptagmon,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, keeptagmon,     {.i = +1 } },
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

