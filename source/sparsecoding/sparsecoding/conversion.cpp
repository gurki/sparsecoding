#include "conversion.h"

#include <stdlib.h> 
#include <time.h>


////////////////////////////////////////////////////////////////////////////////
void imageToPatches(
    const cv::Mat& img, 
    cv::Mat& X, 
    const int n, 
    const int m)
{
    const int w = img.cols;
    const int h = img.rows;

    const int nx = w - (n - 1);
    const int ny = h - (m - 1);

    X = cv::Mat::zeros(n * m, nx * ny, img.type());

    for (int row = 0; row < ny; row++)
    for (int col = 0; col < nx; col++) {
        cv::Mat x;
        img.rowRange(row, row + n).colRange(col, col + m).copyTo(x);
        x.reshape(0, n * m).copyTo(X.col(row * nx + col));
    }
}


////////////////////////////////////////////////////////////////////////////////
void patchesToImage(
    const cv::Mat& X, 
    cv::Mat& img, 
    const int w, 
    const int h,
    const int n,
    const int m)
{
    img = cv::Mat::zeros(h, w, CV_32FC1);

    const int nx = w - (n - 1);
    const int ny = h - (m - 1);

    cv::Mat ones = cv::Mat::ones(n, m, CV_32FC1);
    cv::Mat norm = cv::Mat::zeros(h, w, CV_32FC1);

    for (int row = 0; row < ny; row++)
    for (int col = 0; col < nx; col++) 
    {
        cv::Mat x;
        X.col(row * nx + col).copyTo(x);
        img.rowRange(row, row + n).colRange(col, col + m) += x.reshape(0, n);
        norm.rowRange(row, row + n).colRange(col, col + m) += ones;
    }

    cv::divide(img, norm, img);
}


////////////////////////////////////////////////////////////////////////////////
void splitImage(
    const cv::Mat& img,
    cv::Mat& X,
    const int n,
    const int m)
{
    //  number of patches per row / column (rounded up)
    const int nx = (img.cols + m - 1) / m;
    const int ny = (img.rows + n - 1) / n;

    //  constants
    const int satom = n * m;
    const int natoms = nx * ny;

    //  extend image if not evenly partitionable
    const int dx = m * nx - img.cols;
    const int dy = n * ny - img.rows;

    cv::Mat E = img.clone();
    
    if (dx > 0) {
        cv::hconcat(E, cv::Mat::zeros(E.rows, dx, E.type()), E);
    }
    
    if (dy > 0) {
        cv::vconcat(E, cv::Mat::zeros(dy, E.cols, E.type()), E);
    }
    
    //  extract, reshape and copy blocks to data matrix
    X = cv::Mat::zeros(satom, natoms, img.type());

    for (int row = 0; row < img.rows; row += n)
    for (int col = 0; col < img.cols; col += m) 
    {
        cv::Mat x;
        x = E.rowRange(row, row + n).colRange(col, col + m).clone();
        x.reshape(0, satom).copyTo(X.col(row / n * nx + col / m));
    } 
}


////////////////////////////////////////////////////////////////////////////////
void mergeImage(
    const cv::Mat& X,
    cv::Mat& img,
    const int w,
    const int h,
    const int n,
    const int m)
{
    const int nx = w / m;

    img = cv::Mat::zeros(h, w, X.type());

    for (int row = 0; row < w; row += n)
    for (int col = 0; col < h; col += m)
    {
        cv::Mat x;
        x = X.col(row / n * nx + col / m).clone();
        x.reshape(0, n).copyTo(img.rowRange(row, row + n).colRange(col, col + m));
    }
}


////////////////////////////////////////////////////////////////////////////////
void selectSample(
    const cv::Mat& X,
    cv::Mat& S,
    const int size,
    const float ratio)
{
    const int n = (size < 0) ? (ratio * X.cols) : size;
    const float alpha = (float)n / X.cols;

    S = cv::Mat::zeros(X.rows, n, X.type());

    std::srand((unsigned int)time(0));

    int count = 0;

    for (int i = 0; i < X.cols; i++)
    {
        const float beta = (std::rand() % 1000000) / 1000000.0f;

        if (beta <= alpha) {
            X.col(i).copyTo(S.col(count));
            count++;
        }

        if (count >= n) {
            break;
        }
    }

    // const int n = MIN(X.cols, MAX(ratio * X.cols, size));
    // S = cv::Mat::zeros(X.rows, n, X.type());

    // std::srand((unsigned int)std::time(0));

    // for (int i = 0; i < n; i++) {
    //     const float idx = std::rand() % n;
    //     X.col(idx).copyTo(S.col(i));
    // }
}


////////////////////////////////////////////////////////////////////////////////
void dictToImage(
    const cv::Mat& D, 
    cv::Mat& img)
{
    const int dim = D.rows;
    const int count = D.cols;

    const int n = sqrt(dim);
    const int w = sqrt(count);

    const int off = n / 8;

    img = cv::Mat::zeros(cv::Size(n * w + (w - 1) * off, n * w + (w - 1) * off), D.type());

    for (int row = 0; row < w; row++)
    {
        for (int col = 0; col < w; col++)
        {
            cv::Mat b_ref, p_ref;
            D.col(row * w + col).copyTo(b_ref);
            p_ref = img.rowRange(row * n + row * off, (row + 1) * n + row * off);
            p_ref = p_ref.colRange(col * n + col * off, (col + 1) * n + col * off);

            b_ref.reshape(0, n).copyTo(p_ref);
            p_ref *= n;
            p_ref += 1.0f;
            p_ref *= 0.5f;
        }
    }
}
