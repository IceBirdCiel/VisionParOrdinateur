//
// Created by hung9 on 21/10/2021.
//

#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace opencv;

class Symmetry {
    Symmetry(Mat imO, Mat imS);
    match(Mat imO, Mat imS);

private:
    Mat imO;
    Mat imS;
};