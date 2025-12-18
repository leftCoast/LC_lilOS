#ifndef appName_h
#define appName_h

#include "panel.h"
#include "lilOS.h"

// **********************************************************************
//                            newApp template. 
//
// This is the starting point for creating a new "application" panel for
// a handheld device.
//
// First you must pick a name for your app. It can be 8 chars or less.
// no dots or slashes please.
//  
// The application name will typically be the name used for your app's
// .h and .cpp files. As well as your app's main class name.
//
// All your app's code files should be placed in a folder That can live 
// in your Arduino libraries folder. This folder's name is up to you.
//
// Now, if your App needs any data? files? artork? What have you. THIS
// ALL GOES IN A FOLDER WITH YOUR APP's NAME ON IT. Typically stored
// in the folder with all the source code.
//
// ON YOUR SD CARD, you should have a /system/appFiles/ folder. Drop a
// copy of your App's data folder into there. (The one with your App's
// name on it.) Then reinstall your SD card. Now your OS will know where
// to find your App's goodies when asked.
// 
// **********************************************************************


// **********************************************************************
// 						The base lilOS class
//
#include	<lilOS.h>


class appName	: public panel {

	public:
					appName(void);
	virtual		~appName(void);
	
	virtual void  setup(void);			// Run once. do your setup here.
	virtual void  loop(void);			// Run over and over. Do stuff here.
	virtual void  drawSelf(void);		// If you want to draw your background, do it here.
	virtual void  closing(void);		// Need to save anything before closing shop? Do it here.
};

#endif