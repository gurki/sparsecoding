#ifndef DCT_H
#define DCT_H


#include <opencv2/opencv.hpp>


////////////////////////////////////////////////////////////////////////////////
void cos(const cv::Mat& u, cv::Mat& v);
void kron(const cv::Mat& A, const cv::Mat& B, cv::Mat& C);
void dctBasis2d(const int b, const int a, cv::Mat& dct);


#endif