#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "Disparity.h"
#include "Tracking.h"

Tracking T;

static void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if ( event == cv::EVENT_LBUTTONDOWN )
    {
        T.start = cv::Point(x,y);
        T.roi = cv::Rect();
        T.prevPoints.clear();
        T.nextPoints.clear();
    }
    else if ( event == cv::EVENT_MOUSEMOVE )
    {
        if(T.start.x>=0){
            cv::Point end(x,y);
            T.roi = cv::Rect(T.start, end);
        }
    }else if( event == cv::EVENT_LBUTTONUP){
        cv::Point end(x,y);
        T.roi = cv::Rect(T.start, end);
        T.start = cv::Point(-1,-1);
    }
}

int main()
{
    cv::namedWindow("input");
    //------------------ DISPARITE ----------------------//
    /*cv::Mat im1 = cv::imread("../resources/image1.jpg",1);
    cv::Mat im2 = cv::imread("../resources/image2_bis.jpg",1);

    cv::resize(im2,im2, im1.size());

    std::vector<cv::Point2f> *pA, *pB;
    pA = new std::vector<cv::Point2f>();
    pB = new std::vector<cv::Point2f>();
    Disparity d;
    d.findMatching(&im1, &im2, pA, pB);
    d.displayMatching(im1, im2, pA, pB);
    cv::Mat out1, out2;
    d.rectify(im1, im2,pA, pB, &out1, &out2);
    d.computeDisparity(out1, out2);*/

    //------------------ TRACKING ----------------------//

    cv::setMouseCallback("input", CallBackFunc);

    T.video("../resources/wallBounce.webm");
    //T.video();

    std::cout << "Close Project" << std::endl;
}

