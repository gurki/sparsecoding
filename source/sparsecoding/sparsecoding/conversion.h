#ifndef CONVERSION_H
#define CONVERSION_H


#include <opencv2/opencv.hpp>


////////////////////////////////////////////////////////////////////////////////
void imageToPatches(
    const cv::Mat& img, 
    cv::Mat& X, 
    const int n, 
    const int m);

void splitImage(
    const cv::Mat& img,
    cv::Mat& X,
    const int n,
    const int m);

void mergeImage(
    const cv::Mat& X,
    cv::Mat& img,
    const int w,
    const int h,
    const int n,
    const int m);

void patchesToImage(
    const cv::Mat& X, 
    cv::Mat& img, 
    const int w, 
    const int h,
    const int n,
    const int m);

void selectSample(
    const cv::Mat& X,
    cv::Mat& S,
    const int size,
    const float ratio = -1.0f);

void dictToImage(
    const cv::Mat& D, 
    cv::Mat& img);


#endif