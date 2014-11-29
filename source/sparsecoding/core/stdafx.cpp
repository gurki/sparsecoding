#include "stdafx.h"
#include "Logger.h"

#include <cstdlib>
#include <iostream>

using std::stringstream;
using std::cerr;
using std::cout;
using std::endl;
using std::string;


////////////////////////////////////////////////////////////////////////////////
void loadImagePair(int argc, char* argv[], cv::Mat& img1, cv::Mat& img2)
{
    Logger logger("loadImagePair");

    //  validate cmd arguments
    if (argc <= 3) 
    { 
        logger << "Usage: " << argv[0] << "-p <path> -l <image> -r <image> ";
        logger << " [-d <depth>] [-mu <mu>] [-alpha <alpha>] ";
        logger << "[-theta <theta>] [-reg <regularizer>]"; 
        logger.eol(); 

        exit(EXIT_FAILURE); 
    }


    //  get image names
    string path = "", left = "", right = "";
    bool retp = getParam("p", path, argc, argv);
    bool retl = getParam("l", left, argc, argv);
    bool retr = getParam("r", right, argc, argv);

    if (!retp) {
        cerr << "ERROR: no path specified"; logger.eol();
    }

    if (!retl) {
        cerr << "ERROR: no left image specified"; logger.eol();
    }

    if (!retr) {
        cerr << "ERROR: no right image specified"; logger.eol();
    }

    string name1, name2;
    name1 = path + left;
    name2 = path + right;

    //  load images
    logger.push("load " + name1);
    img1 = cv::imread(name1);
    logger.pop(!img1.empty());

    logger.push("load " + name2);
    img2 = cv::imread(name2);
    logger.pop(!img2.empty());  
}


////////////////////////////////////////////////////////////////////////////////
void loadImage(int argc, char* argv[], cv::Mat& img)
{
    Logger logger("loadImage");

    //  get image names
    string name = "";
    bool ret = getParam("i", name, argc, argv);

    if (!ret) {
        logger << "ERROR: no image specified"; logger.eol();
        logger << "Usage: " << argv[0] << " -i <image> "; logger.eol();
        logger.pop(false); 
    }

    //  load image
    logger.push("load " + name);
    img = cv::imread(name);
    logger.pop(!img.empty());
}



////////////////////////////////////////////////////////////////////////////////
void loadDisparityMap(int argc, char* argv[], cv::Mat& disp)
{
    Logger logger("loadDisparityMap");

    //  get image names
    string name = "";
    bool ret = getParam("disp", name, argc, argv);

    if (!ret) {
        logger << "ERROR: no disparity map specified"; logger.eol();
        logger << "Usage: " << argv[0] << " -disp <image> "; logger.eol();
        logger.pop(false); 
    }

    //  load image
    logger.push("load " + name);
    disp = cv::imread(name, cv::IMREAD_GRAYSCALE);
    logger.pop(!disp.empty());
}


////////////////////////////////////////////////////////////////////////////////
template<>
bool getParam<bool>(std::string param, bool &var, int argc, char **argv)
{
    const char *c_param = param.c_str();
    for(int i=argc-1; i>=1; i--)
    {
        if (argv[i][0]!='-') continue;
        if (strcmp(argv[i]+1, c_param)==0)
        {
            if (!(i+1<argc) || argv[i+1][0]=='-') { var = true; return true; }
            std::stringstream ss;
            ss << argv[i+1];
            ss >> var;
            return (bool)ss;
        }
    }
    return false;
}