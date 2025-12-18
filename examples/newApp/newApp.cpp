#include  "appName.h"


// We create our appName object and, at least, pass the newAppID into our
// base panel constructor. Then you are free to do whatever you need in here.
appName::appName(int newAppID)
  : panel(newAppID) { }


// The world as you know it, is ending. Deallocate what you need to. Don't
// deallocate stuff you've handed to addObj(). Those items will be dealt
// with internally.
appName::~appName(void) { }


// setup() & loop() panel style.
void appName::setup(void) { }


void appName::loop(void) {  }


// The default here is to not draw ourselves. You can change that.
void appName::drawSelf(void) {  }


// Whereas you call close() when you are complete and want to close.. Sometimes
// you don't have control of that. So? No matter if you call close, or something
// else calls close on you, this gets called so you can clean up before being
// deleted.
void appName::closing(void) {  }
