#ifndef SPARSE_CODER_H
#define SPARSE_CODER_H


#include <opencv2/opencv.hpp>

#include "dct.h"
#include "conversion.h"


#define DICTIONARY_DCT      1
#define DICTIONARY_RANDOM   2


class SparseCoder
{
    public:

        SparseCoder();

        void encode(
            const cv::Mat& X, 
            cv::Mat& Z,
            const int sparsity,
            const float maxError = -1.0f
        );

        void encodeL0(
            const cv::Mat& X, 
            cv::Mat& Z,
            const int sparsity,
            const float maxError = -1.0f
        );

        void decode(
            cv::Mat& X,
            const cv::Mat& Z
        );

        void fit(
            const cv::Mat& X,
            const int sparsity,
            const float maxError = -1.0f,
            const int k = 16
        );

        void fitOnline(
            const cv::Mat& X,
            const int sparsity,
            const float maxError = -1.0f
        );

        void initialize(
            const cv::Mat& X,
            const int size,
            const int mode = DICTIONARY_DCT
        );

        void showDictionary() const;

        const cv::Mat& getDictionary() const { return D_; };


    private:

        int sparsity_;
        float error_;

        cv::Mat A_;
        cv::Mat B_;
        cv::Mat D_;
};


#endif