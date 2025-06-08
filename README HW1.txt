This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** UTILITY FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.
parameters: source, target, x, y, Sx, Sy, value

2. Binarization: binarize
Binarize the pixels with the threshold.
parameters: source, target, x, y, Sx, Sy, threshold

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.
parameters: source, target, ratio

4. Decrease brightness: decreaseBrightness
Decrease pixel brightness based on a specified threshold
parameters: ource, target, x, y, Sx, Sy, threshold, value
  

5. Overlay ROI: overlayROI
Determine the valid region of the source image that can be overla
Overlay the ROI from source to target
parameters: source, target, x, y, Sx, Sy, value

6. Rotation: rotate
Rotates image 90, 180, 270 degrees
parameters: ource, target, angle

7. Extract: extractROI
Copys pixels of specified ROI into target image
parameters: source, target, x, y, Sx, Sy

8. Brightness: changeBrightness
Adjusts pixel brightness based on given value
This function does not include threshold
parameters: source, target, brightness

*** PARAMETERS FILE ***

Parameters for each function call:
1. source: the input file name;
2. output: the output file name;
3. #roi: number of ROIS
4. func: function to be called
5. x: inital row, 1st pixel in leftmost position in ROI calculations
6. y: initial column, 1st pixel in the topmost position in ROI calculations.
7. Sx: total number of pixels in the x axis
8. Sy: total number of pixels in the y axis
9. p1, p2, . . ., px: parameters needed for the function

Input.pgm output.pgm 3 0 0 50 50 binarize 50 50 50 100 100 binarize 100
100 100 100 100 binarize 150 rotate 90 180 270 

example:


*** Run the program: ./iptool parameters.txt