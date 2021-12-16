//
// Created by hung9 on 22/11/2021.
//

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>


class Disparity {
public:
    Disparity(){};
    void findMatching(cv::Mat *imageA, cv::Mat *imageB, std::vector<cv::Point2f> *pA, std::vector<cv::Point2f> *pB);
    void displayMatching(cv::Mat imageA, cv::Mat imageB, std::vector<cv::Point2f> *pA, std::vector<cv::Point2f> *pB);
    void rectify(cv::Mat imageL, cv::Mat imageR, std::vector<cv::Point2f> *pL, std::vector<cv::Point2f> *pR, cv::Mat *imageL2, cv::Mat *imageR2);
    void computeDisparity(cv::Mat imageL, cv::Mat imageR);
};
