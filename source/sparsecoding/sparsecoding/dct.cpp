#include "dct.h"
#include "../core/stdafx.h"


////////////////////////////////////////////////////////////////////////////////
void cos(const cv::Mat& u, cv::Mat& v)
{
    v = cv::Mat::zeros(u.size(), u.type());

    for (int i = 0; i < u.cols; ++i)
    for (int j = 0; j < u.rows; ++j) {
        v.at<float>(j, i) = cosf(u.at<float>(j, i));
    }
}


////////////////////////////////////////////////////////////////////////////////
void kron(const cv::Mat& A, const cv::Mat& B, cv::Mat& C)
{
    const int br = B.rows;
    const int bc = B.cols;

    C = cv::Mat::zeros(A.rows * B.rows, A.cols * B.cols, CV_32F);

    for (int r = 0; r < A.rows; r++)
    for (int c = 0; c < A.cols; c++) 
    {
        cv::Mat v;

        v = C.rowRange(r * br, (r + 1) * br);
        v = v.colRange(c * bc, (c + 1) * bc);
        v = A.at<float>(r, c) * B;
    }
}


////////////////////////////////////////////////////////////////////////////////
//  b:      blocksize
//  a:      number of atoms
//  dct:    output dictionary
void dctBasis2d(const int b, const int a, cv::Mat& dct)
{
    const float p = ceil(sqrt(a));
    const int n = sqrt(b);

    cv::Mat dct1d = cv::Mat::zeros(n, p, CV_32F);
    cv::Mat nums = cv::Mat::zeros(n, 1, CV_32F);

    for (int i = 0; i < n; i++) {
        nums.at<float>(i) = i;
    }

    for (int k = 0; k < p; k++)
    {
        cv::Mat v = dct1d.col(k); 
        cos(nums * k * M_PI / p, v);

        if (k > 0) {
            v -= cv::mean(v);
        }

        v /= cv::norm(v);
    }    

    kron(dct1d, dct1d, dct);
}