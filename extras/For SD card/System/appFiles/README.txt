Application folders go in here. Each application should have a folder with it's name and, 
at the minimum, a .bmp file (With the same name as the app & it's folder) of a 32x32 icon
that can be displayed on the home screen to be clicked on to launch that app.

LC__xyzApp  		      - The library containing xyzApp. And inside you find..
xyzApp.h & xyzApp.cpp	- At least the main application code files.. And folder called..
xyzApp                  - And in that folder will be at least..
xyzApp.bmp              - The icon file.

The xyzApp folder also stores the xyzApp  data it needs to do it's thing.

When installing an application. You need to copy the xyzApp folder into /system/appFiles 
folder. Where you found this entrancing document.

Make sense?

Lord, I hope so..
