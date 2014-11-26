#ifndef STDAFX_H
#define STDAFX_H

#include <opencv2/opencv.hpp>

#include <ctime>

#include <string>
#include <sstream>
#include <iostream>


using namespace std;

typedef unsigned int uint;


////////////////////////////////////////////////////////////////////////////////
void loadImagePair(int argc, char* argv[], cv::Mat& img1, cv::Mat& img2);
void loadImage(int argc, char* argv[], cv::Mat& img);
void loadDisparityMap(int argc, char* argv[], cv::Mat& disp);


////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool getParam(std::string param, T &var, int argc, char **argv)
{
    const char *c_param = param.c_str();
    for(int i=argc-1; i>=1; i--)
    {
        if (argv[i][0]!='-') continue;
        if (strcmp(argv[i]+1, c_param)==0)
        {
            if (!(i+1<argc)) continue;
            std::stringstream ss;
            ss << argv[i+1];
            ss >> var;
            return (bool)ss;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////
class Timer
{
    public:
	Timer() : tStart(0), running(false), sec(0.f)
	{
	}
	void start()
	{
		tStart = clock();
		running = true;
	}
	void end()
	{
		if (!running) { sec = 0; return; }
		clock_t tEnd = clock();
		sec = (float)(tEnd - tStart) / CLOCKS_PER_SEC;
		running = false;
	}
	float get()
	{
		if (running) end();
		return sec;
	}
    private:
	clock_t tStart;
	bool running;
	float sec;
};


#endif
