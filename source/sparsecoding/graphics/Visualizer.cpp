#include "Visualizer.h"


//  visualizer statics
std::string Visualizer::screenshotFolder = "screenshots/";
int Visualizer::delayInMilliseconds = 10;

int Visualizer::pointRadius = 3;
int Visualizer::lineThickness = 1;
int Visualizer::circleThickness = 1;

cv::Scalar Visualizer::pointColor = cv::Scalar(0, 255, 255, 255);
cv::Scalar Visualizer::lineColor = cv::Scalar(0, 255, 0, 255);


//  visualizer private statics
int Visualizer::screenshotNo_ = 0;

std::stack<bool> Visualizer::visual_ = std::stack<bool>();
std::stack<bool> Visualizer::interactive_ = std::stack<bool>();
std::stack<bool> Visualizer::autoScreenshot_ = std::stack<bool>();
std::vector<std::string> Visualizer::subfolders_ = std::vector<std::string>();
std::stack<std::string> Visualizer::name_ = std::stack<std::string>();


////////////////////////////////////////////////////////////////////////////////
void Visualizer::imageGrid(const std::vector<cv::Mat>& images, cv::Mat& dst, const int columns)
{
    const int rows = (int)images.size() / columns;
    std::vector<cv::Mat> gridRows(rows);

    for (int r = 0; r < rows; r++) 
    {
        std::vector<cv::Mat>::const_iterator begin = images.begin() + r * columns;
        std::vector<cv::Mat>::const_iterator end = begin + columns;
        std::vector<cv::Mat> rowImages(begin, end);

        cv::hconcat(rowImages, gridRows[r]);
    }

    cv::vconcat(gridRows, dst);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawPoints(const cv::Mat& image,
                            const std::vector<cv::Point2f>& points,
                            cv::Mat& output)
{
    image.copyTo(output);

    for (int i = 0; i < points.size(); i++) {
        const cv::Point2f& p = points[i];
        cv::circle(output, p, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawPoints(const cv::Mat& image1,
                            const cv::Mat& image2,
                            const std::vector<cv::Point2f>& points1,
                            const std::vector<cv::Point2f>& points2,
                            cv::Mat& output)
{
    cv::Mat out1, out2;
    Visualizer::drawPoints(image1, points1, out1);
    Visualizer::drawPoints(image2, points2, out2);

    cv::hconcat(out1, out2, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawPoints(const cv::Mat& image,
                            const std::vector<cv::KeyPoint>& keypoints,
                            cv::Mat& output)
{
    image.copyTo(output);

    for (int i = 0; i < keypoints.size(); i++) {
        const cv::KeyPoint& p = keypoints[i];
        cv::circle(output, p.pt, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawPoints(const cv::Mat& image1,
                            const cv::Mat& image2,
                            const std::vector<cv::KeyPoint>& keypoints1,
                            const std::vector<cv::KeyPoint>& keypoints2,
                            cv::Mat& output1,
                            cv::Mat& output2)
{
    Visualizer::drawPoints(image1, keypoints1, output1);
    Visualizer::drawPoints(image2, keypoints2, output2);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawPoints(const cv::Mat& image1,
                            const cv::Mat& image2,
                            const std::vector<cv::KeyPoint>& keypoints1,
                            const std::vector<cv::KeyPoint>& keypoints2,
                            cv::Mat& output)
{
    cv::Mat out1, out2;
    Visualizer::drawPoints(image1, image2, keypoints1, keypoints2, out1, out2);

    cv::hconcat(out1, out2, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image,
                              const std::vector<cv::Point2f>& points1,
                              const std::vector<cv::Point2f>& points2,
                              cv::Mat& output)
{
    image.copyTo(output);

    for (int i = 0; i < points1.size(); i++)
    {
        const cv::Point2f& p1 = points1[i];
        const cv::Point2f& p2 = points2[i];

        cv::circle(output, p1, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness);
        cv::line(output, p1, p2, Visualizer::lineColor, Visualizer::lineThickness);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::Point2f>& points1,
                              const std::vector<cv::Point2f>& points2,
                              cv::Mat& output1, 
                              cv::Mat& output2)
{
    Visualizer::drawMatching(image1, points1, points2, output1);
    Visualizer::drawMatching(image2, points2, points1, output2);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::Point2f>& points1,
                              const std::vector<cv::Point2f>& points2,
                              cv::Mat& output)
{
    cv::Mat out1, out2;
    Visualizer::drawMatching(image1, points1, points2, out1);
    Visualizer::drawMatching(image2, points2, points1, out2);

    cv::hconcat(out1, out2, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image,
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              cv::Mat& output)
{
    image.copyTo(output);

    for (int i = 0; i < keypoints1.size(); i++)
    {
        const cv::Point2f& p1 = keypoints1[i].pt;
        const cv::Point2f& p2 = keypoints2[i].pt;

        cv::circle(output, p1, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness, CV_FILLED);
        cv::line(output, p1, p2, Visualizer::lineColor, Visualizer::lineThickness);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              cv::Mat& output1, 
                              cv::Mat& output2)
{
    Visualizer::drawMatching(image1, keypoints1, keypoints2, output1);
    Visualizer::drawMatching(image2, keypoints2, keypoints1, output2);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              cv::Mat& output)
{
    cv::Mat out1, out2;
    Visualizer::drawMatching(image1, keypoints1, keypoints2, out1);
    Visualizer::drawMatching(image2, keypoints2, keypoints1, out2);

    cv::hconcat(out1, out2, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image,
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              const std::vector<cv::DMatch>& matches,
                              const int targetImage,
                              cv::Mat& output)
{
    image.copyTo(output);

    for (int i = 0; i < matches.size(); i++)
    {
        const cv::DMatch& match = matches[i];
        const int& id1 = match.queryIdx;
        const int& id2 = match.trainIdx;

        const cv::Point2f& p1 = keypoints1[id1].pt;
        const cv::Point2f& p2 = keypoints2[id2].pt;

        if (targetImage <= 1) {
            cv::circle(output, p1, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness, CV_FILLED);
            cv::line(output, p1, p2, Visualizer::lineColor, Visualizer::lineThickness);
        } else {
            cv::circle(output, p2, Visualizer::pointRadius, Visualizer::pointColor, Visualizer::circleThickness, CV_FILLED);
            cv::line(output, p2, p1, Visualizer::lineColor, Visualizer::lineThickness);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              const std::vector<cv::DMatch>& matches12,
                              cv::Mat& output1, 
                              cv::Mat& output2)
{
    Visualizer::drawMatching(image1, keypoints1, keypoints2, matches12, 1, output1);
    Visualizer::drawMatching(image2, keypoints1, keypoints2, matches12, 2, output2);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawMatching(const cv::Mat& image1,
                              const cv::Mat& image2, 
                              const std::vector<cv::KeyPoint>& keypoints1,
                              const std::vector<cv::KeyPoint>& keypoints2,
                              const std::vector<cv::DMatch>& matches12,
                              cv::Mat& output)
{
    cv::Mat out1, out2;
    Visualizer::drawMatching(image1, image2, keypoints1, keypoints2, matches12, out1, out2);

    cv::hconcat(out1, out2, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::drawInlierOutlier(const cv::Mat& image1,
                                   const cv::Mat& image2,
                                   const std::vector<cv::KeyPoint>& keypoints1,
                                   const std::vector<cv::KeyPoint>& keypoints2,
                                   const std::vector<cv::DMatch>& matches12,
                                   const std::vector<cv::DMatch>& inlier12,
                                   cv::Mat& output)
{   
    const cv::Scalar tempLineColor = Visualizer::lineColor;
    Visualizer::lineColor = cv::Scalar(0, 0, 255, 255);

    cv::Mat out1, out2;
    Visualizer::drawMatching(image1, image2, keypoints1, keypoints2, matches12, out1, out2);

    Visualizer::lineColor = tempLineColor;
    Visualizer::drawMatching(out1, out2, keypoints1, keypoints2, inlier12, output);
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizePoints(const cv::Mat& image1,
                                 const cv::Mat& image2,
                                 const std::vector<cv::Point2f>& points1, 
                                 const std::vector<cv::Point2f>& points2)
{
    if (visual()) {
        cv::Mat output;
        Visualizer::drawPoints(image1, image2, points1, points2, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizePoints(const cv::Mat& image1,
                                 const cv::Mat& image2,
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2)
{
    if (visual()) {        
        cv::Mat output;
        Visualizer::drawPoints(image1, image2, keypoints1, keypoints2, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizeMatching(const cv::Mat& image1,
                                   const cv::Mat& image2,
                                   const std::vector<cv::Point2f>& points1, 
                                   const std::vector<cv::Point2f>& points2)
{
    if (visual()) {
        cv::Mat output;
        Visualizer::drawMatching(image1, image2, points1, points2, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizeMatching(const cv::Mat& image1,
                                   const cv::Mat& image2,
                                   const std::vector<cv::KeyPoint>& keypoints1,
                                   const std::vector<cv::KeyPoint>& keypoints2)
{
    if (visual()) {
        cv::Mat output;
        Visualizer::drawMatching(image1, image2, keypoints1, keypoints2, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizeMatching(const cv::Mat& image1,
                                   const cv::Mat& image2,
                                   const std::vector<cv::KeyPoint>& keypoints1,
                                   const std::vector<cv::KeyPoint>& keypoints2,
                                   const std::vector<cv::DMatch>& matches12)
{
    if (visual()) {
        cv::Mat output;
        Visualizer::drawMatching(image1, image2, keypoints1, keypoints2, matches12, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::visualizeInlierOutlier(const cv::Mat& image1,
                                        const cv::Mat& image2,
                                        const std::vector<cv::KeyPoint>& keypoints1,
                                        const std::vector<cv::KeyPoint>& keypoints2,
                                        const std::vector<cv::DMatch>& matches12,
                                        const std::vector<cv::DMatch>& inlier12)
{
    if (visual()) {
        cv::Mat output;
        Visualizer::drawInlierOutlier(image1, image2, keypoints1, keypoints2, matches12, inlier12, output);
        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::showImagePair(const cv::Mat& image1,
                               const cv::Mat& image2)
{    
    if (visual()) 
    {
        cv::Mat output;
        cv::hconcat(image1, image2, output);

        Visualizer::showImage(output);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::showImage(const cv::Mat& image)
{    
    if (visual()) 
    {
        cv::namedWindow("output");
        cv::imshow("output", image);

        int key = -1;

        if (Visualizer::interactive()) {
            key = cv::waitKey();
        } else {
            cv::waitKey(Visualizer::delayInMilliseconds);
        }

        if (key == 's' || Visualizer::autoScreenshot())
            saveImage(image);
    }
}


////////////////////////////////////////////////////////////////////////////////
void Visualizer::saveImage(const cv::Mat& image)
{   
    // std::stringstream directory;
    // directory << Visualizer::screenshotFolder;
    
    // for (const auto& sub : subfolders_)
    //     directory << sub;
        
    // boost::filesystem::path path(directory.str());
    // boost::filesystem::create_directories(path);

    // directory << Visualizer::screenshotName() << Visualizer::screenshotNo_++ << ".png";

    // bool succ = cv::imwrite(directory.str(), image);
    // //cout << "image saved to " << directory.str() << endl;
}