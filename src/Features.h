//
// Created by hung9 on 07/12/2021.
//

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <opencv2/opencv.hpp>
#include <vector>

class Features {
public :
    Features(cv::VideoCapture *vid, std::vector<cv::Mat> *images);

    void show();

private:

    cv::VideoCapture vid;
    std::vector<cv::Mat> images;
};
