#ifndef VISUALIZER_H
#define VISUALIZER_H


#include <opencv2/opencv.hpp>

#include <vector>
#include <stack>
#include <string>


class Visualizer
{
    public:   

        //  multi-image displays
        static void imageGrid(const std::vector<cv::Mat>& images, 
                              cv::Mat& dst, 
                              const int columns = 2);


        //  points
        static void drawPoints(const cv::Mat& image,
                               const std::vector<cv::Point2f>& points,
                               cv::Mat& output);

        static void drawPoints(const cv::Mat& image1,
                               const cv::Mat& image2,
                               const std::vector<cv::Point2f>& points1,
                               const std::vector<cv::Point2f>& points2,
                               cv::Mat& output);


        //  keypoints
        static void drawPoints(const cv::Mat& image,
                               const std::vector<cv::KeyPoint>& keypoints,
                               cv::Mat& output);
        
        static void drawPoints(const cv::Mat& image1,
                               const cv::Mat& image2,
                               const std::vector<cv::KeyPoint>& keypoints1,
                               const std::vector<cv::KeyPoint>& keypoints2,
                               cv::Mat& output1,
                               cv::Mat& output2);

        static void drawPoints(const cv::Mat& image1,
                               const cv::Mat& image2,
                               const std::vector<cv::KeyPoint>& keypoints1,
                               const std::vector<cv::KeyPoint>& keypoints2,
                               cv::Mat& output);


        //  point matching, pairwise
        static void drawMatching(const cv::Mat& image,
                                 const std::vector<cv::Point2f>& points1,
                                 const std::vector<cv::Point2f>& points2,
                                 cv::Mat& output);

        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2, 
                                 const std::vector<cv::Point2f>& points1,
                                 const std::vector<cv::Point2f>& points2,
                                 cv::Mat& output1, 
                                 cv::Mat& output2);

        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2, 
                                 const std::vector<cv::Point2f>& points1,
                                 const std::vector<cv::Point2f>& points2,
                                 cv::Mat& output);


        //  keypoint matching, pairwise
        static void drawMatching(const cv::Mat& image,
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 cv::Mat& output);

        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2,
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 cv::Mat& output1,
                                 cv::Mat& output2);
        
        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2,
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 cv::Mat& output);


        //  keypoint matching, indexed
        static void drawMatching(const cv::Mat& image,
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 const std::vector<cv::DMatch>& matches,
                                 const int targetImage,
                                 cv::Mat& output);

        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2, 
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 const std::vector<cv::DMatch>& matches12,
                                 cv::Mat& output1, 
                                 cv::Mat& output2);

        static void drawMatching(const cv::Mat& image1,
                                 const cv::Mat& image2, 
                                 const std::vector<cv::KeyPoint>& keypoints1,
                                 const std::vector<cv::KeyPoint>& keypoints2,
                                 const std::vector<cv::DMatch>& matches12,
                                 cv::Mat& output);


        //  keypoint matching, indexed inlier/outlier
        static void drawInlierOutlier(const cv::Mat& image1,
                                      const cv::Mat& image2,
                                      const std::vector<cv::KeyPoint>& keypoints1,
                                      const std::vector<cv::KeyPoint>& keypoints2,
                                      const std::vector<cv::DMatch>& matches12,
                                      const std::vector<cv::DMatch>& inlier12,
                                      cv::Mat& output);


        //  visualization (draw and show)
        static void visualizePoints(const cv::Mat& image1,
                                    const cv::Mat& image2,
                                    const std::vector<cv::Point2f>& points1, 
                                    const std::vector<cv::Point2f>& points2);

        static void visualizePoints(const cv::Mat& image1,
                                    const cv::Mat& image2,
                                    const std::vector<cv::KeyPoint>& keypoints1,
                                    const std::vector<cv::KeyPoint>& keypoints2);

        static void visualizeMatching(const cv::Mat& image1,
                                      const cv::Mat& image2,
                                      const std::vector<cv::Point2f>& points1, 
                                      const std::vector<cv::Point2f>& points2);
        
        static void visualizeMatching(const cv::Mat& image1,
                                      const cv::Mat& image2,
                                      const std::vector<cv::KeyPoint>& keypoints1,
                                      const std::vector<cv::KeyPoint>& keypoints2);

        static void visualizeMatching(const cv::Mat& image1,
                                      const cv::Mat& image2,
                                      const std::vector<cv::KeyPoint>& keypoints1,
                                      const std::vector<cv::KeyPoint>& keypoints2,
                                      const std::vector<cv::DMatch>& matches12);

        static void visualizeInlierOutlier(const cv::Mat& image1,
                                           const cv::Mat& image2,
                                           const std::vector<cv::KeyPoint>& keypoints1,
                                           const std::vector<cv::KeyPoint>& keypoints2,
                                           const std::vector<cv::DMatch>& matches12,
                                           const std::vector<cv::DMatch>& inlier12);


        //  showing (display and optionally save)
        static void showImagePair(const cv::Mat& image1,
                                  const cv::Mat& image2);

        static void showImage(const cv::Mat& image);
        static void saveImage(const cv::Mat& image);


        //  functionality
        static void pushVisual(const bool visual) { visual_.push(visual); };
        static void pushInteractive(const bool interactive) { interactive_.push(interactive); };
        static void pushAutoScreenshot(const bool autoScreenshot) { autoScreenshot_.push(autoScreenshot); };
        static void pushScreenshotName(const std::string& name) { name_.push(name); };
        static void pushScreenshotSubfolder(const std::string& folder) { subfolders_.push_back(folder); };

        static void popVisual() { visual_.pop(); };
        static void popInteractive() { interactive_.pop(); };
        static void popAutoScreenshot() { autoScreenshot_.pop(); };
        static void popScreenshotName() { name_.pop(); };
        static void popScreenshotSubfolder() { subfolders_.pop_back(); };

        static bool visual() { return visual_.empty() ? true : visual_.top(); };
        static bool interactive() { return interactive_.empty() ? true : interactive_.top(); };
        static bool autoScreenshot() { return autoScreenshot_.empty() ? false : autoScreenshot_.top(); };
        static std::string screenshotSubfolder() { return subfolders_.empty() ? "./" : subfolders_.back(); };
        static std::string screenshotName() { return name_.empty() ? "screenshot" : name_.top(); };
        
        //  variables
        static std::string screenshotFolder;     
        static int delayInMilliseconds;

        static int pointRadius;
        static int lineThickness;
        static int circleThickness;
        static cv::Scalar pointColor;
        static cv::Scalar lineColor;


    private:

        static std::stack<bool> visual_;
        static std::stack<bool> interactive_;
        static std::stack<bool> autoScreenshot_;
        static std::vector<std::string> subfolders_;
        static std::stack<std::string> name_;
                
        static int screenshotNo_;

        Visualizer() {};
        ~Visualizer() {};
};
   

#endif