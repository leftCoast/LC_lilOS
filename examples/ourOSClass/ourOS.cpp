#include <ourOS.h>
#include <strTools.h>

// **************** ourSOClass ***************

// Our global OS. We create it here.
ourOSClass ourOS;

ourOSClass::ourOSClass(void) {  }


ourOSClass::~ourOSClass(void) {  }


bool ourOSClass::begin(void) {

  if (!lilOS::begin()) {
    Serial.println("OS startup failure. Locking process.");
    while(1) delay(10);
  }
  return true;
}

panel* ourOSClass::createPanel(int panelID) {

   panel* newPanel;

   beep();
   newPanel = NULL;
   switch (panelID) {
      case homeApp	: newPanel = new ourHome();		break;   // Make the one we created.
      case app1ID		: newPanel = new app1(app1ID);	break;
      case app2ID		: newPanel = new app2(app2ID);	break;
      default			: newPanel = NULL;
   }
   return newPanel;
}


void ourOSClass::beep(void) { tone(BEEP_PIN, 800,5); }


int ourOSClass::getTonePin(void) { return BEEP_PIN; }


const char*	ourOSClass::getSystemFolder(void) {
  
  heapStr(&pathBuff,SYSTEM_PATH);
  return pathBuff;
}


const char* ourOSClass::getPanelName(int panelID) {

    switch(panelID) {
      case breakoutApp  : return "app1";
      case calcApp      : return "app2";
      default           : return NULL;
    }
}

// ***************** ourHome *****************

#define APP1_X  68
#define APP1_Y  289
#define APP2_X  140
#define APP2_Y  APP1_Y

ourHome::ourHome(void)
  : homePanel() {  }


ourHome::~ourHome(void) { }


void ourHome::setup(void) {
  
   appIcon*  appIconPtr;

   appIconPtr = new appIcon(APP1_X,APP1_Y,app1,iconPath(app1ID));
   addObj(appIconPtr);
   
   appIconPtr = new appIcon(APP2_X,APP@_Y,app2,iconPath(app2ID));
   addObj(appIconPtr);
   
}


void ourHome::loop(void) {  }


void ourHome::drawSelf(void) { 

  screen->fillRectGradient(0,0,width,height,&blue,&black);
}
