#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image& src, image& tgt, int x, int y, int Sx, int Sy, int value);
		static void binarize(image& src, image& tgt, int x, int y, int Sx, int Sy, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void decreaseBrightness(image& src, image& tgt, int x, int y, int Sx, int Sy, int threshold, int value);
		static void overlayROI(image &src, image &tgt, int x, int y, int Sx, int Sy);
		static void rotate(image& src, image& tgt, int angle);
		static void extractROI(image& src, image& tgt, int x, int y, int Sx, int Sy);
		static void changeBrightness(image& src, image& tgt, int brightness);
		


};

#endif

