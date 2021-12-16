#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include "Symmetry.h"
#include "Disparity.h"
#include "Tracking.h"
#include "Features.h"

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
    //------------------ DISPARITE ----------------------//
    /*
    cv::Mat im1 = cv::imread("../resources/image1.jpg",1);
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
    d.computeDisparity(out1, out2);
*/
    //------------------ TRACKING ----------------------//
    /*cv::namedWindow("input");
    cv::setMouseCallback("input", CallBackFunc);

    T.video("../resources/wallBounce.webm");
    //T.video();
*/

    //------------------ FEATURES ----------------------//
/*
    cv::VideoCapture vid("../resources/tpfeatures_set1/set1/video.mp4");
    cv::Mat naruto = cv::imread("../resources/tpfeatures_set1/set1/naruto.jpg",1);
    cv::Mat bleach = cv::imread("../resources/tpfeatures_set1/set1/bleach.jpg",1);
    cv::Mat gantz = cv::imread("../resources/tpfeatures_set1/set1/gantz.jpg",1);
    cv::Mat deathnote = cv::imread("../resources/tpfeatures_set1/set1/deathnote.jpg",1);
    cv::Mat yakitate = cv::imread("../resources/tpfeatures_set1/set1/yakitate.jpg",1);

    std::vector<cv::Mat> vec;
    vec.push_back(naruto);
    vec.push_back(bleach);
    vec.push_back(gantz);
    vec.push_back(deathnote);
    vec.push_back(yakitate);
*/
    cv::VideoCapture vid("../resources/tpfeatures_set2/set2/video.mp4");
    cv::Mat p = cv::imread("../resources/tpfeatures_set2/set2/p.jpg");
    cv::Mat k = cv::imread("../resources/tpfeatures_set2/set2/k.jpg");
    cv::Mat d = cv::imread("../resources/tpfeatures_set2/set2/d.jpg");
    cv::Mat b = cv::imread("../resources/tpfeatures_set2/set2/b.jpg");
    cv::Mat g = cv::imread("../resources/tpfeatures_set2/set2/g.jpg");

    std::vector<cv::Mat> vec;
    vec.push_back(p);
    vec.push_back(d);
    vec.push_back(b);
    vec.push_back(k);
    vec.push_back(g);

    Features f(&vid, &vec);
    f.show();


    //------------------ SYMMETRY ----------------------//
/*
    Symmetry s;

    cv::Mat im = cv::imread("../resources/Symmetry/n.jpg");

    s.isSymmetric(im);
*/
    std::cout << "Close Project" << std::endl;
}
