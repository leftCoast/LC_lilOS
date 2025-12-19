#ifndef ourOS_h
#define ourOS_h

#include <lilOS.h>      // From LC_lilOS library
#include <app1.h>			// Include files for applications.
#include <app2.h>			// ...


#define  BEEP_PIN     xx          // Pin# must match your hardware.
#define SYSTEM_PATH   "/system/"  // This needs to be set as it is on the SD card.


// The apps enum is our list of application IDs. The code for each application can live in the Arduino
// library.
//
// Optionally, each application can have an application folder for it's own, stored in
// the /system/appFiles/ folder on your SD card. This folder must bear the
// application's name. This is so the OS can find it when asked for it by the
// application.
//
// The application can use this folder as it pleases. Data, images what have you. If
// there is a 32x32 pixel .bmp file bearing the application's name it will be used as
// the application's icon for your home screen.

enum  apps {        
  homeApp = HOME_PANEL_ID,
  app1ID,
  app2ID
};



// **************** ourSOClass ***************

// Our version of the OS that includes our hardware configureation info. These are
// The methods that need to be filled in.

class ourOSClass : public lilOS {

   public:
            ourOSClass(void);
   virtual  ~ourOSClass(void);

	virtual	panel*      createPanel(int panelID);
   virtual  bool        begin(void);
	virtual	void        beep(void);
	virtual	int         getTonePin(void);
	virtual	const char*	getSystemFolder(void);
	virtual	const char*	getPanelName(int panelID);
};



// ***************** ourHome ***********

// Our verstion of a homePanel, we get to decide how it looks and functions. This is
// just a basic starting point.

class ourHome : public homePanel {

   public:
           ourHome(void);
   virtual ~ourHome(void);
  
   virtual void    setup(void);
   virtual void    loop(void);
   virtual void    drawSelf(void);
};


extern ourOSClass ourOS;   // We "expose" our OS to the world here.

#endif