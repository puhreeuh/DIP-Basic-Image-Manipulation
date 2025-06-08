#include "../iptools/core.h"
#include <strings.h>
#include <string.h>
#include <cstdlib> // Include this header for atoi and atof
#include <cstdio>

using namespace std;

#define MAXLEN 256

int main(int argc, char** argv)
{
    image src, tgt;
    FILE* fp;
    char str[MAXLEN];
    char outfile[MAXLEN];
    char* pch;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "Can't open file: %s\n", argv[1]);
        exit(1);
    }

    while (fgets(str, MAXLEN, fp) != NULL)
    {
        
        pch = strtok(str, " ");
        if (pch == NULL)
        {
            fprintf(stderr, "Invalid input name format.\n");
            break;
        }
        src.read(pch);
        // Initialize tgt image to match src
        tgt = src;

        // Print src image dimensions and sample pixel value
        printf("src image dimensions: Rows=%d, Columns=%d\n", src.getNumberOfRows(), src.getNumberOfColumns());
        if (src.getNumberOfRows() > 0 && src.getNumberOfColumns() > 0) {
            printf("Sample pixel value from src image: %d\n", src.getPixel(0, 0));
        }

        pch = strtok(NULL, " ");
        if (pch == NULL)
        {
            fprintf(stderr, "Invalid output name format.\n");
            break;
        }
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");
        if (pch == NULL)
        {
            fprintf(stderr, "Invalid number of ROIs format.\n");
            break;
        }
        int numROIs = atoi(pch);


        // Loop through each ROI
        for (int i = 0; i < numROIs; i++)
        {
            // Read x, y, Sx, Sy, and function name for the current ROI set
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                fprintf(stderr, "Invalid input x format for ROI %d.\n", i + 1);
                break;
            }
            int x = atoi(pch);

            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                fprintf(stderr, "Invalid y input format for ROI %d.\n", i + 1);
                break;
            }
            int y = atoi(pch);

            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                fprintf(stderr, "Invalid Sx input format for ROI %d.\n", i + 1);
                break;
            }
            int Sx = atoi(pch);

            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                fprintf(stderr, "Invalid Sy input format for ROI %d.\n", i + 1);
                break;
            }
            int Sy = atoi(pch);

            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                fprintf(stderr, "Invalid function name format for ROI %d.\n", i + 1);
                break;
            }

            // Before processing each ROI, print the ROI parameters
            printf("ROI %d: x=%d, y=%d, Sx=%d, Sy=%d\n", i + 1, x, y, Sx, Sy);

            // Read the original ROI image
            image originalROI;
            originalROI = src;  // Make a copy of the original image

            // A. Rotate the original ROI image
            image roiRotated90, roiRotated180, roiRotated270;
            utility::rotate(originalROI, roiRotated90, 90);
            utility::rotate(originalROI, roiRotated180, 180);
            utility::rotate(originalROI, roiRotated270, 270);

            // Generate unique filenames for each rotated ROI image
            char rotated90Filename[MAXLEN];
            char rotated180Filename[MAXLEN];
            char rotated270Filename[MAXLEN];

            snprintf(rotated90Filename, sizeof(rotated90Filename), "%s_rotated90_%d.png", outfile, i + 1);
            snprintf(rotated180Filename, sizeof(rotated180Filename), "%s_rotated180_%d.png", outfile, i + 1);
            snprintf(rotated270Filename, sizeof(rotated270Filename), "%s_rotated270_%d.png", outfile, i + 1);

            // Save each rotated ROI image with its respective filename
            roiRotated90.save(rotated90Filename);
            roiRotated180.save(rotated180Filename);
            roiRotated270.save(rotated270Filename);

            // B. Scale the original ROI image and rotate it by 90, 180, and 270 degrees
            float scaleFactors[] = {1.5, 1.75, 2.0};  // You can adjust the scale factors as needed
            for (int scaleFactorIndex = 0; scaleFactorIndex < sizeof(scaleFactors) / sizeof(float); scaleFactorIndex++)
            {
                float scale = scaleFactors[scaleFactorIndex];
                image roiScaled;
                utility::scale(originalROI, roiScaled, scale);

                // Rotate the scaled ROI image by 90, 180, and 270 degrees
                image roiScaledRotated90, roiScaledRotated180, roiScaledRotated270;
                utility::rotate(roiScaled, roiScaledRotated90, 90);
                utility::rotate(roiScaled, roiScaledRotated180, 180);
                utility::rotate(roiScaled, roiScaledRotated270, 270);

                // Generate unique filenames for each rotated and scaled ROI image
                char scaledRotated90Filename[MAXLEN];
                char scaledRotated180Filename[MAXLEN];
                char scaledRotated270Filename[MAXLEN];

                snprintf(scaledRotated90Filename, sizeof(scaledRotated90Filename), "%s_scaled_rotated90_%d_%d.png", outfile, i + 1, scaleFactorIndex + 1);
                snprintf(scaledRotated180Filename, sizeof(scaledRotated180Filename), "%s_scaled_rotated180_%d_%d.png", outfile, i + 1, scaleFactorIndex + 1);
                snprintf(scaledRotated270Filename, sizeof(scaledRotated270Filename), "%s_scaled_rotated270_%d_%d.png", outfile, i + 1, scaleFactorIndex + 1);

                // Save each rotated and scaled ROI image with its respective filename
                roiScaledRotated90.save(scaledRotated90Filename);
                roiScaledRotated180.save(scaledRotated180Filename);
                roiScaledRotated270.save(scaledRotated270Filename);
            }

            // C. Change the brightness of the original ROI image and rotate it by 90, 180, and 270 degrees
            int brightnessValues[] = { /* adjust the brightness values as needed */ };

            for (int brightnessIndex = 0; brightnessIndex < sizeof(brightnessValues) / sizeof(int); brightnessIndex++)
            {
                int brightness = brightnessValues[brightnessIndex];

                // Clone the original ROI image to work on
                image roiBrightnessAdjusted = originalROI;

                // Adjust the brightness of the ROI image using the utility function
                utility::changeBrightness(roiBrightnessAdjusted, roiBrightnessAdjusted, brightness);

                // Rotate the brightness-adjusted ROI image by 90, 180, and 270 degrees
                image roiBrightnessAdjustedRotated90, roiBrightnessAdjustedRotated180, roiBrightnessAdjustedRotated270;
                utility::rotate(roiBrightnessAdjusted, roiBrightnessAdjustedRotated90, 90);
                utility::rotate(roiBrightnessAdjusted, roiBrightnessAdjustedRotated180, 180);
                utility::rotate(roiBrightnessAdjusted, roiBrightnessAdjustedRotated270, 270);

                // Generate unique filenames for each rotated and brightness-adjusted ROI image
                char brightnessRotated90Filename[MAXLEN];
                char brightnessRotated180Filename[MAXLEN];
                char brightnessRotated270Filename[MAXLEN];

                snprintf(brightnessRotated90Filename, sizeof(brightnessRotated90Filename), "%s_brightness_rotated90_%d_%d.png", outfile, i + 1, brightnessIndex + 1);
                snprintf(brightnessRotated180Filename, sizeof(brightnessRotated180Filename), "%s_brightness_rotated180_%d_%d.png", outfile, i + 1, brightnessIndex + 1);
                snprintf(brightnessRotated270Filename, sizeof(brightnessRotated270Filename), "%s_brightness_rotated270_%d_%d.png", outfile, i + 1, brightnessIndex + 1);

                // Save each rotated and brightness-adjusted ROI image with its respective filename
                roiBrightnessAdjustedRotated90.save(brightnessRotated90Filename);
                roiBrightnessAdjustedRotated180.save(brightnessRotated180Filename);
                roiBrightnessAdjustedRotated270.save(brightnessRotated270Filename);
            }

            
            // Process the ROI based on the function name
            if (strcmp(pch, "add") == 0)
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    fprintf(stderr, "Invalid add function format for ROI %d.\n", i + 1);
                    break;
                }
                int value = atoi(pch);
                // Process the ROI using addGrey function
                image roiAddResult;
                utility::addGrey(src, roiAddResult, x, y, Sx, Sy, value);

                // Overlay the modified ROI onto the target image
                utility::overlayROI(roiAddResult, tgt, x, y, Sx, Sy);
        
            }
            else if (strcmp(pch, "binarize") == 0)
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    fprintf(stderr, "Invalid binarize function format for ROI %d.\n", i + 1);
                    break;
                }
                image roiBinarizeResult;
                int threshold = atoi(pch);
                // Process the ROI using binarize function
                utility::binarize(src, roiBinarizeResult, x, y, Sx, Sy, threshold);

                utility::overlayROI(roiBinarizeResult, tgt, x, y, Sx, Sy);
                
            }
            else if (strcmp(pch, "scale") == 0)
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    fprintf(stderr, "Invalid scale function format for ROI %d.\n", i + 1);
                    break;
                }
                utility::scale(src, tgt, atof(pch));

            }
            else if (strcmp(pch, "decreaseBrightness") == 0)
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    fprintf(stderr, "Invalid threshold DB function format for ROI %d.\n", i + 1);
                    break;
                }
                int threshold = atoi(pch);
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    fprintf(stderr, "Invalid value DB format for ROI %d.\n", i + 1);
                    break;
                }
                image roiDBResult;
                int value = atoi(pch);
                // Process the ROI using decreaseBrightness function
                utility::decreaseBrightness(src, roiDBResult, x, y, Sx, Sy, threshold, value);

                utility::overlayROI(roiDBResult, tgt, x, y, Sx, Sy);
                
        
            }
            else
            {
                fprintf(stderr, "No function: %s for ROI %d.\n", pch, i + 1);
                break;
            }
        }
        // Save the output image after processing all ROIs
        tgt.save(outfile);
    }

    

    fclose(fp);
    return 0;
}