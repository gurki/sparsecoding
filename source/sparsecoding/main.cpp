////////////////////////////////////////////////////////////////////////////////
//
//  Sparse Coding and Dictionary Learning
//
////////////////////////////////////////////////////////////////////////////////
//
//  Tobias Gurdan, tobias@gurdan.de
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES


////////////////////////////////////////////////////////////////////////////////
#include <opencv2/opencv.hpp>

#include "core/stdafx.h"
#include "core/Logger.h"

#include "graphics/Visualizer.h"

#include "sparsecoding/sparsecoder.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
void corruptSaltPepper(cv::Mat& img, const float ratio)
{
    std::srand((unsigned int)std::time(0));

    cv::Mat imgv = img.reshape(0, 1);

    for (int i = 0; i < img.cols * img.rows; i++)
    {
        const float rnd = (std::rand() % 10000) / 10000.0f;

        if (rnd <= ratio / 2.0f) {
            imgv.at<float>(i) = 0;
        } else if (rnd <= ratio) {
            imgv.at<float>(i) = 1;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    Logger log("main");
    // Visualizer::pushVisual(false);


    //  load, convert, resize and corrupt image
    string  dfile = "../../data/standard/barbara.png";
    cv::Mat img   = cv::imread(dfile, cv::IMREAD_GRAYSCALE);
    // cv::resize(img, img, cv::Size(128, 128));
    if(img.empty()) {
        log << "Test image not found" << endl;
        log << "Please replace \"" << dfile << '"' << endl;
        return EXIT_FAILURE;
    }


    cv::Mat noise = cv::Mat(img.size(), img.type());
    cv::randn(noise, 0, 20);
    img += noise;

    img.convertTo(img, CV_32F);
    img /= 255.0f;

    // corruptSaltPepper(img, 0.01);

    // Visualizer::showImage(img);


    //  convert image
    const int n = 8;

    cv::Mat X;
    imageToPatches(img, X, n, n);
    // splitImage(img, X, n, n);

    const int nsamples = cv::min(10000, X.cols);

    cv::Mat S;
    selectSample(X, S, nsamples);
    
    
    //  initialize coder
    const int natoms = 256;
    const int sparsity = 4;

    Visualizer::pushInteractive(false);

    SparseCoder coder;
    coder.initialize(S, natoms);
    coder.fitOnline(S, sparsity);
    
    Visualizer::popInteractive();

    coder.showDictionary();
    

    //  encode data
    cv::Mat Z;
    coder.encode(X, Z, sparsity);


    //  decode data
    cv::Mat Xrec;
    coder.decode(Xrec, Z);

    cv::Mat imgrec;
    patchesToImage(Xrec, imgrec, img.cols, img.rows, n, n);
    // mergeImage(Xrec, imgrec, img.cols, img.rows, n, n);
    Visualizer::showImagePair(img, imgrec);


    return EXIT_SUCCESS;
}












