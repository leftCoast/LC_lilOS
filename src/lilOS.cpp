
#include <lilOS.h>
#include <resizeBuff.h>
#include <strTools.h>
//#include <debug.h>

int		nextPanel	= NO_PANEL_ID;	// What panel do we want showing now?
lilOS*	OSPtr			= NULL;			// Used by "packages" that need to access the OS's stuff.
panel*	ourPanel		= NULL;


// *****************************************************
// ********************   appIcon  *********************
// *****************************************************


appIcon::appIcon(int xLoc,int yLoc,int message,const char* path,int pix)
  : iconButton(xLoc,yLoc,path,pix) { mMessage = message;}

  
appIcon::~appIcon(void) { }


void appIcon::doAction(void) { nextPanel = mMessage; }


	
// *****************************************************
// *******************     panel    ********************
// *****************************************************
	
	
// And it all starts up again..
panel::panel(int panelID,menuBarChoices menuBarChoice,eventSet inEventSet)
  : drawGroup(0,0,screen->width(),screen->height(),inEventSet) {

	mPanelID		= panelID;							// Save what "kind" of panel we are.
	mMenuBar		= NULL;								// Default to NULL.
	mFilePath	= NULL;								// This too.
	ourPanel		= this;								// Let's get this in early.
	switch (menuBarChoice) {						// Lets see what kind of bar they wish for?
		case noMenuBar			: break;				// None? Fine, we go now.
		case emptyMenuBar		: 						// Now, panels are created by the O.S. during runtime.
			mMenuBar = new menuBar(this,false);	// Meaning, everything is up and running.
			addObj(mMenuBar);							// So, this call will be fine.
		break;	
		case closeBoxMenuBar	:						// Just like the one above.
			mMenuBar = new menuBar(this);			// Default is to have the close box. (Most do)	
			addObj(mMenuBar);
		break;
	}
 }


// The world as you know it, is ending..
panel::~panel(void) { 
	
	resizeBuff(0,&mFilePath);
	ourPanel = NULL;
}


// Whom ever is managing panels can assign IDs to us for
// their nefarious plans. Here's where we hand it back.
int panel::getPanelID(void) { return mPanelID; }



// Given a filename from our folder, generate the fullpath to it.
bool panel::setFilePath(const char* inName) {
	
	char*			folderPtr;
	int			pathLen;
	bool			success;
	

	success = false;
	folderPtr = NULL;
	if (heapStr(&folderPtr,OSPtr->getPanelFolder(mPanelID))) {	// If we got a folder path..
		pathLen = strlen(folderPtr);										// Num chars in this path..
		pathLen = pathLen + strlen(inName) + 1;						// Add more for the file name, '\' and '\0'.
		if (resizeBuff(pathLen,&mFilePath)) {							// If we can get the RAM for the path..
			strcpy(mFilePath,folderPtr);									// Our folder path goes in.
			strcat(mFilePath,inName);										// File name goes in.
			success = true;													// Looks good!
		}																			//
		freeStr(&folderPtr);													// Recycle our local copy.
	}																				//
	return success;															// Return our result.
}


// setup() & loop() panel style.
void panel::setup(void) { }


void panel::loop(void) {  }


// The default here is to not draw ourselves. You can change that.
void panel::drawSelf(void) {  }


// Call this when its time to close the panel. Default is to go back
// to the home panel. Maybe you want to link to another from time to time?
void panel::close(void) { nextPanel = HOME_PANEL_ID; }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void panel::closing(void) {  }


// We get a close command, we close! At least by default.
void panel::handleCom(stdComs comID) {

	switch(comID) {
		case closeCmd	: close(); break;
		default			: break;
	}
}



// *****************************************************
// *******************   homePanel  ********************
// *****************************************************


homePanel::homePanel(void)
  : panel(HOME_PANEL_ID,noMenuBar) { pathBuff = NULL; } // Home panels have no panel to return to.


homePanel::~homePanel(void) { if (pathBuff) freeStr(&pathBuff); }


void homePanel::setup(void) {  }


void homePanel::loop(void) {  }


void homePanel::drawSelf(void) {

	screen->fillScreen(&white);
	screen->setCursor(5,25);
	screen->setTextColor(&black,&white);
	screen->drawText("Default home panel");
}


char* homePanel::iconPath(int panelID) {

	heapStr(&pathBuff,OSPtr->getPanelIconPath(panelID));
	return pathBuff;
}
   
   
   
// *****************************************************
// *********************   lilOS   ********************
// *****************************************************

 
lilOS::lilOS(void) {

	pathBuff 	= NULL;
	systemPath	= NULL;
	iconPath		= NULL;
	kbdIconPath	= NULL;
	stdIconPath	= NULL;
	imagePath	= NULL;
	appPath		= NULL;
	mPanel		= NULL;
	nextPanel	= HOME_PANEL_ID;
}
				

lilOS::~lilOS(void) {

	freeStr(&pathBuff);
	freeStr(&systemPath);
	freeStr(&iconPath);
	freeStr(&kbdIconPath);
	freeStr(&stdIconPath);
	freeStr(&imagePath);
	freeStr(&appPath);
}


// A good plan would be to inherit, call this, the do your begin.
bool lilOS::begin(void) {
	
	if (initPaths()) {
		icon32Mask.readFromBMP(getStdIconPath(mask32));	// Read out and setup the standard 32x32 icon mask.
		icon22Mask.readFromBMP(getStdIconPath(mask22));	// Read out and setup the standard 22x22 icon mask.
		OSPtr = this;												// Hookup the global pointer to ourselves.
		hookup();													// Want to use idle()? Its ready.
		nextPanel = HOME_PANEL_ID;								// Set to the default home panel.
		return true;												// True means good to go!
	}																	//
	return false;													// If we get here? we failed.	
}																		//


// This is the guy you inherit and use to create your custom panels.
panel* lilOS::createPanel(int panelID) { return new homePanel(); }


void lilOS::launchPanel(void) {

	while(screen->touched());							// Hold 'till their finger is off the screen. Its important!
	if (mPanel) {											// If we have a panel now..
		mPanel->closing();								// Give the panel one last chance to get its affairs in order.
		delete mPanel;										// Delete the panel.
		mPanel = NULL;										// Flag it. Just in case..
	}
	if (nextPanel!=NO_PANEL_ID) {						// Someone wants a panel.
		mPanel = createPanel(nextPanel);				// See if they can create it.
		if (mPanel) {										// So, we get a panel in the end?
			viewList.addObj(mPanel);					// Hook it up to the global viewList to allow drawing etc.
			mPanel->setup();								// Give it its setup() call. (Just like back home!)
			mPanel->hookup();								// Hookup idle stuff as well.
		} else {
			nextPanel = HOME_PANEL_ID;					// If we could not create a panel, send 'em home.
		}
	}
}



// Tell the current panel its loop time.
void lilOS::loop(void) {
	
	if(!mPanel) {											// If we don't have a panel..
		if (nextPanel != NO_PANEL_ID) {				// And if we want one.
			screen->fillScreen(&green);
			launchPanel();									// Launch a new panel.
		}														//
	} else if(nextPanel!=mPanel->getPanelID()) {	// Else, we have a panel, But we want a change.
		launchPanel();										// Launch a new panel.
	} else { mPanel->loop(); }							// As always, if there is a panel, let it have some loop time.
}


const char* lilOS::getStdIconPath(stdIcons theIcon) {
	
	filePath	ourPath;
	char		nameBuff[20];
	
	freeStr(&pathBuff);																// Clear out pathBuff.
	if (ourPath.setPath(stdIconPath)) {											// If we can set the stdIcon path..
		switch(theIcon) {																// Lets map the icon ID to it's name..
			case mask22 	: strcpy(nameBuff,"mask22.bmp"); 	break;	// Lots of 'em..
			case mask32 	: strcpy(nameBuff,"mask32.bmp"); 	break;	//
			case app32 		: strcpy(nameBuff,"app32.bmp"); 		break;	//
			case check22 	: strcpy(nameBuff,"check22.bmp"); 	break;	//
			case check32 	: strcpy(nameBuff,"check32.bmp"); 	break;	//
			case choice32 	: strcpy(nameBuff,"choice32.bmp"); 	break;	//
			case copy32 	: strcpy(nameBuff,"copy32.bmp"); 	break;	//
			case cross22 	: strcpy(nameBuff,"cross22.bmp"); 	break;	//
			case cross32 	: strcpy(nameBuff,"cross32.bmp"); 	break;	//
			case cut32 		: strcpy(nameBuff,"cut32.bmp"); 		break;	//
			case doc16 		: strcpy(nameBuff,"doc16.bmp"); 		break;	//
			case edit22 	: strcpy(nameBuff,"edit22.bmp"); 	break;	//
			case edit32 	: strcpy(nameBuff,"edit32.bmp"); 	break;	//
			case fNew22		: strcpy(nameBuff,"fNew22.bmp"); 	break;	//
			case fNew32		: strcpy(nameBuff,"fNew32.bmp"); 	break;	//
			case folder16	: strcpy(nameBuff,"fldr16.bmp"); 	break;	//
			case fldrRet16	: strcpy(nameBuff,"fldrBk16.bmp"); 	break;	//
			case fSave22	: strcpy(nameBuff,"fSave22.bmp"); 	break;	//
			case fSave32	: strcpy(nameBuff,"fSave32.bmp"); 	break;	//
			case fOpen22 	: strcpy(nameBuff,"fOpen22.bmp"); 	break;	//
			case fOpen32 	: strcpy(nameBuff,"fOpen32.bmp"); 	break;	//
			case FdrNew22	: strcpy(nameBuff,"FdrNew22.bmp"); 	break;	//
			case FdrNew32	: strcpy(nameBuff,"FdrNew32.bmp"); 	break;	//
			case note32		: strcpy(nameBuff,"note32.bmp"); 	break;	//
			case paste32 	: strcpy(nameBuff,"paste32.bmp"); 	break;	//
			case pref22 	: strcpy(nameBuff,"pref22.bmp");		break;	//
			case pref32 	: strcpy(nameBuff,"pref32.bmp"); 	break;	//
			case SDCard16 	: strcpy(nameBuff,"SD16.bmp"); 		break;	//
			case search22	: strcpy(nameBuff,"search22.bmp"); 	break;	//
			case search32	: strcpy(nameBuff,"search32.bmp"); 	break;	//
			case sort22		: strcpy(nameBuff,"sort22.bmp"); 	break;	//
			case sort32		: strcpy(nameBuff,"sort32.bmp"); 	break;	//
			case trashC22	: strcpy(nameBuff,"trashC22.bmp"); 	break;	//
			case trashC32	: strcpy(nameBuff,"trashC32.bmp"); 	break;	//
			case trashR22	: strcpy(nameBuff,"trashR22.bmp"); 	break;	//
			case trashR32	: strcpy(nameBuff,"trashR32.bmp"); 	break;	//
			case warn32		: strcpy(nameBuff,"warn32.bmp"); 	break;	//
			case x22			: strcpy(nameBuff,"x22.bmp"); 		break;	//
			case x32			: strcpy(nameBuff,"x32.bmp"); 		break;	//
		}																					//
		if (ourPath.pushChildItemByName(nameBuff)) {							// Move the path to the icon file.
			heapStr(&pathBuff,ourPath.getPath()); 								// Save it off.	
		}																					//
	}																						//
	return pathBuff;																	//
}


// Looking for a panel's folder path.
const char* lilOS::getPanelFolder(int panelID) {
	
	filePath	ourPath;
	
	freeStr(&pathBuff);															// Clear out pathBuff.
	if (ourPath.setPath(appPath)) {											// Start with appFiles folder.
		if (ourPath.pushChildItemByName(getPanelName(panelID))) {	// Add the panel's name.
			heapStr(&pathBuff,ourPath.getPath()); 							// Save it off.	
		}																				//
	}																					//
	return pathBuff;																// return our results.
}


const char* lilOS::getPanelIconPath(int panelID) {

	filePath	ourPath;													// File path tool.
	char		appName[20];											// A holder for the name, extra sized.
	
	freeStr(&pathBuff);												// Clear out pathBuff.
	if (ourPath.setPath(getPanelFolder(panelID))) {			// If we can get the app's folder..
		strcpy(appName,ourPath.getCurrItemName());			// Copy the name..
		if (appName[0]!='\0') {										// Not an empty name..
			strcat(appName,".bmp");									// panel name + .bmp = icon name.
			if (ourPath.pushChildItemByName(appName)) {		// Add the icon's name.
				heapStr(&pathBuff,ourPath.getPath());			// Copy the result to our pathBuff.
			}																//
		}																	//
	}																		//
	return pathBuff;													// return our results.
}
					

bool lilOS::initPaths(void) {

	filePath	ourPath;
	
	if (ourPath.setPath(getSystemFolder())) {															// Start with the system folder.
		if (heapStr(&systemPath,ourPath.getPath())) {												// Save it off.
			if (ourPath.pushChildItemByName("appFiles")) {											// Move the path to the appFiles folder.
				if (heapStr(&appPath,ourPath.getPath())) {											// Save it off.
					ourPath.popItem();																		// Back to system folder.
					if (ourPath.pushChildItemByName("icons")) {										// Move the path to the icons folder.
						if (heapStr(&iconPath,ourPath.getPath())) {									// Save it off.
							ourPath.popItem();																// Back to system folder.
							if (ourPath.pushChildItemByName("images")) {								// Move the path to the images folder.
								if (heapStr(&imagePath,ourPath.getPath())) {							// Save it off.
									if (ourPath.setPath(iconPath)) {										// Back to icons folder.
										if (ourPath.pushChildItemByName("keyboard")) {				// Move the path to the keyboard folder.
											if (heapStr(&kbdIconPath,ourPath.getPath())) {			// Save it off.
												ourPath.popItem();											// Back to icons folder.
												if (ourPath.pushChildItemByName("standard")) {		// Move the path to the standard folder.
													if (heapStr(&stdIconPath,ourPath.getPath())) {	// Save it off.
														return true;											// Call it good!
													}																//
												}																	//
											}																		//
										}																			//
									}																				//
								}																					//
							}																						//
						}																							//
					}																								//
				}																									//
			}																										//
		}																											//
	}																												//	
	return false;																								// If we got here, something failed.
}																












