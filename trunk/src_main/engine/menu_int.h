/*
menu_int.h - interface between engine and menu
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef MENU_INT_H
#define MENU_INT_H

#include "cvardef.h"
#include "gameinfo.h"
#include "wrect.h"

typedef int		HIMAGE;		// handle to a graphic

typedef struct ui_globalvars_s
{	
	float		time;		// unclamped host.realtime
	float		frametime;

	int		scrWidth;		// actual values
	int		scrHeight;

	int		maxClients;
	int		developer;
	int		demoplayback;
	int		demorecording;
	char		demoname[64];	// name of currently playing demo
	char		maptitle[64];	// title of active map
} ui_globalvars_t;

typedef struct ui_enginefuncs_s
{
	// image handlers
	HIMAGE	(*pfnPIC_Load)( const char *szPicName, const byte *ucRawImage, long ulRawImageSize );
	void	(*pfnPIC_Free)( const char *szPicName );
	int	(*pfnPIC_Width)( HIMAGE hPic );
	int	(*pfnPIC_Height)( HIMAGE hPic );
	void	(*pfnPIC_Set)( HIMAGE hPic, int r, int g, int b, int a );
	void	(*pfnPIC_Draw)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*pfnPIC_DrawHoles)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*pfnPIC_DrawTrans)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*pfnPIC_DrawAdditive)( int x, int y, int width, int height, const wrect_t *prc );
	void	(*pfnPIC_EnableScissor)( int x, int y, int width, int height );
	void	(*pfnPIC_DisableScissor)( void );

	// screen handlers
	void	(*pfnFillRGBA)( int x, int y, int width, int height, int r, int g, int b, int a );

	// cvar handlers
	cvar_t*	(*pfnRegisterVariable)( const char *szName, const char *szValue, int flags );
	float	(*pfnGetCvarFloat)( const char *szName );
	char*	(*pfnGetCvarString)( const char *szName );
	void	(*pfnCvarSetString)( const char *szName, const char *szValue );
	void	(*pfnCvarSetValue)( const char *szName, float flValue );

	// command handlers
	int	(*pfnAddCommand)( const char *cmd_name, void (*function)(void) );
	void	(*pfnClientCmd)( int execute_now, const char *szCmdString );
	void	(*pfnDelCommand)( const char *cmd_name );
	int       (*pfnCmdArgc)( void );	
	char*	(*pfnCmdArgv)( int argc );
	char*	(*pfnCmd_Args)( void );

	// debug messages (in-menu shows only notify)	
	void	(*Con_Printf)( char *fmt, ... );
	void	(*Con_DPrintf)( char *fmt, ... );
	void	(*Con_NPrintf)( int pos, char *fmt, ... );
	void	(*Con_NXPrintf)( struct con_nprint_s *info, char *fmt, ... );

	// sound handlers
	void	(*pfnPlayLocalSound)( const char *szSound );

	// cinematic handlers
	void	(*pfnDrawLogo)( const char *filename, float x, float y, float width, float height );
	int	(*pfnGetLogoWidth)( void );
	int	(*pfnGetLogoHeight)( void );
	float	(*pfnGetLogoLength)( void );

	// text message system
	void	(*pfnDrawCharacter)( int x, int y, int width, int height, int ch, int ulRGBA, HIMAGE hFont );
	int	(*pfnDrawConsoleString)( int x, int y, const char *string );
	void	(*pfnDrawSetTextColor)( int r, int g, int b, int alpha );
	void	(*pfnDrawConsoleStringLen)(  const char *string, int *length, int *height );
	void	(*pfnSetConsoleDefaultColor)( int r, int g, int b ); // color must came from colors.lst

	// custom rendering (for playermodel preview)
	struct cl_entity_s* (*pfnGetPlayerModel)( void );	// for drawing playermodel previews
	void	(*pfnSetModel)( struct cl_entity_s *ed, const char *path );
	void	(*pfnClearScene)( void );
	void	(*pfnRenderScene)( const struct ref_params_s *fd );
	int	(*CL_CreateVisibleEntity)( int type, struct cl_entity_s *ent );

	// misc handlers
	void	(*pfnHostError)( const char *szFmt, ... );
	int	(*pfnFileExists)( const char *filename, int gamedironly );
	void	(*pfnGetGameDir)( char *szGetGameDir );

	// gameinfo handlers
	int	(*pfnCreateMapsList)( int fRefresh );
	int	(*pfnClientInGame)( void );
	void	(*pfnClientJoin)( const struct netadr_s adr );
	
	// parse txt files
	byte*	(*COM_LoadFile)( const char *filename, int *pLength );
	char*	(*COM_ParseFile)( char *data, char *token );
	void	(*COM_FreeFile)( void *buffer );

	// keyfuncs
	void	(*pfnKeyClearStates)( void );				// call when menu open or close
	void	(*pfnSetKeyDest)( int dest );
	const char *(*pfnKeynumToString)( int keynum );
	const char *(*pfnKeyGetBinding)( int keynum );
	void	(*pfnKeySetBinding)( int keynum, const char *binding );
	int	(*pfnKeyIsDown)( int keynum );
	int	(*pfnKeyGetOverstrikeMode)( void );
	void	(*pfnKeySetOverstrikeMode)( int fActive );
	void	*(*pfnKeyGetState)( const char *name );			// for mlook, klook etc

	// engine memory manager
	void*	(*pfnMemAlloc)( size_t cb, const char *filename, const int fileline );
	void	(*pfnMemFree)( void *mem, const char *filename, const int fileline );

	// collect info from engine
	int	(*pfnGetGameInfo)( GAMEINFO *pgameinfo );
	GAMEINFO	**(*pfnGetGamesList)( int *numGames );			// collect info about all mods
	char 	**(*pfnGetFilesList)( const char *pattern, int *numFiles, int gamedironly );	// find in files
	int 	(*pfnGetSaveComment)( const char *savename, char *comment );
	int	(*pfnGetDemoComment)( const char *demoname, char *comment );
	int	(*pfnCheckGameDll)( void );				// returns false if hl.dll is missed
	char	*(*pfnGetClipboardData)( void );

	// engine launcher
	void	(*pfnShellExecute)( const char *name, const char *args, int closeEngine );
	void	(*pfnWriteServerConfig)( const char *name );
	void	(*pfnChangeInstance)( const char *newInstance, const char *szFinalMessage );
	void	(*pfnPlayBackgroundTrack)( const char *introName, const char *loopName );
	void	(*pfnHostEndGame)( const char *szFinalMessage );

	// menu interface is freezed at version 0.75
	// new functions starts here 
	float	(*pfnRandomFloat)( float flLow, float flHigh );	
	long	(*pfnRandomLong)( long lLow, long lHigh );

	void	(*pfnSetCursor)( void *hCursor );			// change cursor
	int	(*pfnIsMapValid)( char *filename );
} ui_enginefuncs_t;

typedef struct
{
	int	(*pfnVidInit)( void );
	void	(*pfnInit)( void );
	void	(*pfnShutdown)( void );
	void	(*pfnRedraw)( float flTime );
	void	(*pfnKeyEvent)( int key, int down );
	void	(*pfnMouseMove)( int x, int y );
	void	(*pfnSetActiveMenu)( int active );
	void	(*pfnAddServerToList)( struct netadr_s adr, const char *info );
	void	(*pfnGetCursorPos)( int *pos_x, int *pos_y );
	void	(*pfnSetCursorPos)( int pos_x, int pos_y );
	void	(*pfnShowCursor)( int show );
	void	(*pfnCharEvent)( int key );
	int	(*pfnMouseInRect)( void );	// mouse entering\leave game window
	int	(*pfnIsVisible)( void );
	int	(*pfnCreditsActive)( void );	// unused
	void	(*pfnFinalCredits)( void );	// show credits + game end
} UI_FUNCTIONS;

typedef int (*MENUAPI)( UI_FUNCTIONS *pFunctionTable, ui_enginefuncs_t* engfuncs, ui_globalvars_t *pGlobals );

#endif//MENU_INT_H