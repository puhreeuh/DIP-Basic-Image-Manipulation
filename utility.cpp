#include "utility.h"
#include <fstream> // Include this header for file stream handling
#include <iostream> // Include this header for error handling

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image& src, image& tgt, int x, int y, int Sx, int Sy, int value)		//each parameter chnaged to recieve ROIs
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = x; i < x + Sx; i++)		// changed to fit ROIs
    {
        for (int j = y; j < y + Sy; j++)
        {
			// Modify pixel values
            int modifiedValue = checkValue(src.getPixel(i, j) + value);
            tgt.setPixel(i, j, modifiedValue);


		}
	}
}
/*-----------------------------------------------------------------------**/
void utility::binarize(image& src, image& tgt, int x, int y, int Sx, int Sy, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = x; i < x + Sx; i++)
	{
		for (int j = y; j < y + Sy; j++)
        {
            if (i < src.getNumberOfRows() && j < src.getNumberOfColumns()) {
				if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}
/*-----------------------------------------------------------------------**/
void utility::decreaseBrightness(image &src, image &tgt, int x, int y, int Sx, int Sy, int threshold, int value){
    /* Resize target image to match source image dimensions */
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

    /* Loop through pixels in ROI to determine if intensity is below threshold */
    for (int i = x; i < x + Sx; i++)
    {
        for (int j = y; j < y + Sy; j++)  // Fix the loop control variable here
        {    
            if (i < src.getNumberOfRows() && j < src.getNumberOfColumns()) 
            {
                int pixelValue = src.getPixel(i,j);
                if (pixelValue < threshold)
                {
                    int newPixelValue = checkValue(pixelValue - value);
                    tgt.setPixel(i, j, newPixelValue);
                }
                else
                {
                    /* Pixel value not altered if it is above or equal to the threshold */
                    tgt.setPixel(i, j, pixelValue);
                }
            }
        }
    }
}
/*-----------------------------------------------------------------------**/
void utility::overlayROI(image &src, image &tgt, int x, int y, int Sx, int Sy)
{
    // Determine the valid region of the source image that can be overlaid
    int srcXStart = std::max(0, x);
    int srcYStart = std::max(0, y);
    int srcXEnd = std::min(src.getNumberOfRows(), x + Sx);
    int srcYEnd = std::min(src.getNumberOfColumns(), y + Sy);

    // Overlay the ROI from source to target
    for (int i = srcXStart; i < srcXEnd; i++)
    {
        for (int j = srcYStart; j < srcYEnd; j++)
        {
            // Overlay pixel from the modified ROI onto the target at the same position
            tgt.setPixel(i, j, src.getPixel(i, j));
        }
    }
}
/*-----------------------------------------------------------------------**/
void utility::rotate(image& src, image& tgt, int angle)
{
    int rows = src.getNumberOfRows();
    int cols = src.getNumberOfColumns();

    if (angle == 90 || angle == 270)
    {
        // Swap rows and columns for 90 and 270 degrees rotation
        tgt.resize(cols, rows);
    }
    else if (angle == 180)
    {
        tgt.resize(rows, cols);
    }
    else
    {
        // Invalid angle - return original image
        tgt = src;
        return;
    }

    // Create a PGM file for the rotated image
    std::ofstream pgmFile("rotated_image.pgm");
    if (!pgmFile.is_open())
    {
        std::cerr << "Error: Failed to create PGM file." << std::endl;
        return;
    }

    // Write PGM header
    pgmFile << "P2" << std::endl;
    pgmFile << tgt.getNumberOfColumns() << " " << tgt.getNumberOfRows() << std::endl;
    pgmFile << "255" << std::endl;

    for (int i = 0; i < tgt.getNumberOfRows(); i++)
    {
        for (int j = 0; j < tgt.getNumberOfColumns(); j++)
        {
            int originalX, originalY;
            if (angle == 90)
            {
                originalX = j;
                originalY = tgt.getNumberOfRows() - 1 - i;
            }
            else if (angle == 180)
            {
                originalX = tgt.getNumberOfRows() - 1 - i;
                originalY = tgt.getNumberOfColumns() - 1 - j;
            }
            else if (angle == 270)
            {
                originalX = tgt.getNumberOfColumns() - 1 - j;
                originalY = i;
            }

            // Check if the original coordinates are within bounds
            if (originalX >= 0 && originalX < src.getNumberOfRows() &&
                originalY >= 0 && originalY < src.getNumberOfColumns())
            {
                // Copy the pixel value from the original image to the rotated image
                int pixelValue = src.getPixel(originalX, originalY);
                pgmFile << pixelValue << " ";
            }
            else
            {
                // Handle out-of-bounds pixels as needed (e.g., set to a default value)
                pgmFile << MINRGB << " ";
            }
        }
        pgmFile << std::endl;
    }

    pgmFile.close();
}


/*-----------------------------------------------------------------------**/
void utility::extractROI(image& src, image& tgt, int x, int y, int Sx, int Sy)
{
    // Resize the target image to match the ROI size
    tgt.resize(Sx, Sy);

    // Copy pixels from the source image (ROI region) to the target image
    for (int i = x, tgt_i = 0; i < x + Sx && i < src.getNumberOfRows(); i++, tgt_i++)
    {
        for (int j = y, tgt_j = 0; j < y + Sy && j < src.getNumberOfColumns(); j++, tgt_j++)
        {
            tgt.setPixel(tgt_i, tgt_j, src.getPixel(i, j));
        }
    }
}
/*-----------------------------------------------------------------------**/

void utility::changeBrightness(image& src, image& tgt, int brightness)
{
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

    for (int i = 0; i < src.getNumberOfRows(); i++)
    {
        for (int j = 0; j < src.getNumberOfColumns(); j++)
        {
            int pixelValue = src.getPixel(i, j);
            int newPixelValue = checkValue(pixelValue - brightness);
            tgt.setPixel(i, j, newPixelValue);
        }
    }
}
