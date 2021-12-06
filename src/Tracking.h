//
// Created by hung9 on 28/11/2021.
//

#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include <vector>

class Tracking {

public:
    Tracking(){}

    void draw();
    void video(const std::string& videoName = "");
    void detectPoints(cv::Mat& img);
    void trackPoints();
    std::vector<cv::Point2f> purgePoints(std::vector<cv::Point2f>& points, std::vector<uchar>& status);
    void updateROI();

    cv::Rect roi;
    cv::Point start= cv::Point(-1,-1);
    cv::Mat img;
    cv::Mat nextInput;
    cv::Mat prevInput;
    std::vector<cv::Point2f> prevPoints;
    std::vector<cv::Point2f> nextPoints;

    static int event, x, y , flags;
};
