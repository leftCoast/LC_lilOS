#include <lilOS.h>
#include <slider.h>

class quickApp : public panel {

	public:
				quickApp(int panelID);
				~quickApp(void);
				
	virtual	void setup(void);
	virtual	void loop();
	
				slider*	ourSlider;
				label*	result;			
};


quickApp::quickApp(int panelID)
	:panel(panelID) {
	
	ourSlider	= NULL;
	result		= NULL;
}
	
	
quickApp::~quickApp(void) { }
				
void quickApp::setup(void) {

	ourSlider = new slider(20,40,int 200,int 10,false);
	addObj(ourSlider);
}


void quickApp::loop() {


}

