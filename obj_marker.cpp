#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>

// for filelisting
#include <stdio.h>
#include <sys/io.h>

// for fileoutput
#include <string>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

IplImage* image = 0;
IplImage* image2 = 0;

int roi_x0 = 0;
int roi_y0 = 0;
int roi_x1 = 0;
int roi_y1 = 0;
int numOfRec = 0;
int nImg = 0;
int startDraw = 0;

string IntToString(int num)
{
    ostringstream myStream; //creates an ostringstream object
    myStream << num << flush;
    return (myStream.str()); //returns the string form of the stringstream object
};

void on_mouse(int event, int x, int y, int flag, void *param)
{
    if (event == CV_EVENT_LBUTTONDOWN)
    {
        if (!startDraw) // catch the first rectangle dot
        {
            roi_x0 = x;
            roi_y0 = y;
            startDraw = 1;
        } else { // catch the second rectangle dot
            roi_x1 = x;
            roi_y1 = y;
            startDraw = 0;
        }
    }
    if (event == CV_EVENT_MOUSEMOVE && startDraw) // draws the rectangle
    {
        image2 = cvCloneImage(image);
        cvRectangle(image2, cvPoint(roi_x0, roi_y0), cvPoint(x, y), CV_RGB(255, 0, 255), 1);
        cvShowImage("<SPACE>add <B>save and load next <ESC>exit", image2);
        cvReleaseImage(&image2);
    }
}

int main(int argc, char** argv)
{
    char iKey = 0;
    string strPrefix;
    string strPostfix;
    string input_directory;
    string output_file;

    if (argc != 3) {
        fprintf(stderr, "%s output_info.txt raw/data/directory/\n", argv[0]);
        return -1;
    }

    input_directory = argv[2];
    output_file = argv[1];

    /* Get a file listing of all files with in the input directory */
    DIR *dir_p = opendir (input_directory.c_str());
    struct dirent *dir_entry_p;

    if (dir_p == NULL) {
        fprintf(stderr, "Failed to open directory %s\n", input_directory.c_str());
        return -1;
    }

    fprintf(stderr, "Object: Input Directory: %s  Output File: %s\n", input_directory.c_str(), output_file.c_str());

    //    init highgui
    cvAddSearchPath(input_directory);

    cvNamedWindow("<SPACE>add <B>save and load next <ESC>exit", 1);

    cvSetMouseCallback("<SPACE>add <B>save and load next <ESC>exit", on_mouse, NULL);

    fprintf(stderr, "Opening directory...");

    // clean text file
    std::ofstream ofs;
    ofs.open(output_file.c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    fprintf(stderr, "done.\n");

    int nImg = 0;
    while ((dir_entry_p = readdir(dir_p)) != NULL)
    {
        nImg++;
        numOfRec = 0;

        if (strcmp(dir_entry_p->d_name, ""))
            fprintf(stderr, "Examining file %s\n", dir_entry_p->d_name);

        // image location and name
        strPrefix = input_directory + dir_entry_p->d_name;
        strPostfix = ""; // the rest of the info

        fprintf(stderr, "Loading image %s\n", strPrefix.c_str());

        if ((image = cvLoadImage(strPrefix.c_str(), 1)) != 0)
        {
            // work on current image
            do

            {
                cvShowImage("<SPACE>add <B>save and load next <ESC>exit", image);

                // used cvWaitKey returns:
                //    <B>=66        save added rectangles and show next image
                //    <b>=98
                //    <ESC>=27        exit program
                //    <Space>=32        add rectangle to current image
                //  any other key clears rectangle drawing only
                iKey = cvWaitKey(0);
                switch (iKey)
                {
                case 27: // ESC
                    cvReleaseImage(&image);
                    cvDestroyWindow("<SPACE>add <B>save and load next <ESC>exit");
                    return 0;
                case 32: // Space
                    numOfRec++;
                    printf("   %d. rect x=%d\ty=%d\tx2h=%d\ty2=%d\n", numOfRec, roi_x0, roi_y0, roi_x1, roi_y1);
                    //printf("   %d. rect x=%d\ty=%d\twidth=%d\theight=%d\n",numOfRec,roi_x1,roi_y1,roi_x0-roi_x1,roi_y0-roi_y1);
                    // currently two draw directions possible:
                    //        from top left to bottom right or vice versa
                    if (roi_x1 > roi_x0 && roi_y1 > roi_y0)
                    {
                        printf("   %d. rect x=%d\ty=%d\twidth=%d\theight=%d\n", numOfRec, roi_x0, roi_y0, roi_x1 - roi_x0, roi_y1 - roi_y0);
                        // append rectangle coord to previous line content
                        strPostfix += " " + IntToString(roi_x0) + " " + IntToString(roi_y0) + " " + IntToString(roi_x1 - roi_x0) + " " + IntToString(roi_y1 - roi_y0);
                    }
                    else if (roi_x0 > roi_x1 && roi_y0 > roi_y1)
                    {
                        printf("   %d. rect x=%d\ty=%d\twidth=%d\theight=%d\n", numOfRec, roi_x1, roi_y1, roi_x0 - roi_x1, roi_y0 - roi_y1);
                        // append rectangle coord to previous line content
                        strPostfix += " " + IntToString(roi_x1) + " " + IntToString(roi_y1) + " " + IntToString(roi_x0 - roi_x1) + " " + IntToString      (roi_y0 - roi_y1);
                    }
                    else if (roi_x1 > roi_x0 && roi_y0 > roi_y1)
                    {
                        printf("   %d. rect x=%d\ty=%d\twidth=%d\theight=%d\n", numOfRec, roi_x1, roi_y1, roi_x0 - roi_x1, roi_y0 - roi_y1);
                        // append rectangle coord to previous line content
                        strPostfix += " " + IntToString(roi_x0) + " " + IntToString(roi_y1) + " " + IntToString(roi_x1 - roi_x0) + " " + IntToString      (roi_y0 - roi_y1);
                    }
                    else if (roi_x0 > roi_x1 && roi_y1 > roi_y0)
                    {
                        printf("   %d. rect x=%d\ty=%d\twidth=%d\theight=%d\n", numOfRec, roi_x1, roi_y1, roi_x0 - roi_x1, roi_y0 - roi_y1);
                        // append rectangle coord to previous line content
                        strPostfix += " " + IntToString(roi_x1) + " " + IntToString(roi_y0) + " " + IntToString(roi_x0 - roi_x1) + " " + IntToString      (roi_y1 - roi_y0);
                    }

                    break;
                }
            }
            while (iKey != 98);
            {
                // save to file  <rel_path>\bmp_file.name numOfRec x0 y0 width0 height0 x1 y1 width1 height1...
                if (numOfRec > 0 && iKey == 98)
                {
                    // append line to text file
                    ofstream output(output_file.c_str(), std::fstream::app);
                    output << strPrefix << " " << numOfRec << strPostfix << "\n";
                    output.close();

                    cvReleaseImage(&image);
                }

                else
                {
                    fprintf(stderr, "Failed to load image, %s\n", strPrefix.c_str());
                }
            }
        }
    }
    cvDestroyWindow("<SPACE>add <B>save and load next <ESC>exit");
    closedir(dir_p);

    return 0;
}
