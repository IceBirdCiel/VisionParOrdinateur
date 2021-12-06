//
// Created by hung9 on 22/11/2021.
//

#include <iostream>
#include "Disparity.h"

void Disparity::findMatching(cv::Mat *imageA, cv::Mat *imageB, std::vector<cv::Point2f> *pA, std::vector<cv::Point2f> *pB) {
    cv::Mat imageL, imageR;
    cv::cvtColor(*imageA, imageL, cv::COLOR_BGR2GRAY);
    cv::cvtColor(*imageB, imageR, cv::COLOR_BGR2GRAY);
    std::vector<cv::Point2f> tmpA;
    std::vector<cv::Point2f> tmpB;

    cv::goodFeaturesToTrack(imageL, tmpA, 1000, 0.05, 10);

    std::vector<uchar> status;
    std::vector<float> errors;
    cv::calcOpticalFlowPyrLK(imageL, imageR, tmpA, tmpB, status, errors);

    for(int i = 0; i < tmpA.size(); i++){
        if(status.at(i) > 0){
            pA->push_back(tmpA.at(i));
            pB->push_back(tmpB.at(i));
        }
    }
}

void Disparity::displayMatching(cv::Mat imageA, cv::Mat imageB, std::vector<cv::Point2f> *pA,
                                std::vector<cv::Point2f> *pB) {
    cv::Mat copyA, copyB;
    imageA.copyTo(copyA);
    imageB.copyTo(copyB);

    for(int i = 0; i < pA->size(); i++){
        cv::line(copyA, pA->at(i), pB->at(i), cv::Scalar(255,0,0), 2);
        cv::line(copyB, pB->at(i), pA->at(i), cv::Scalar(255,0,0), 2);
    }

    cv::imshow("display Matching Left", copyA);
    cv::waitKey(0);
    cv::imshow("display Matching Right", copyB);
    cv::waitKey(0);
}

void Disparity::rectify(cv::Mat imageL, cv::Mat imageR, std::vector<cv::Point2f> *pL, std::vector<cv::Point2f> *pR,
                        cv::Mat *imageL2, cv::Mat *imageR2) {
    cv::Mat imageA, imageB;
    cv::cvtColor(imageL, imageA, cv::COLOR_BGR2GRAY);
    cv::cvtColor(imageR, imageB, cv::COLOR_BGR2GRAY);

    cv::Mat f;
    f = cv::findFundamentalMat(*pL, *pR, cv::FM_RANSAC);
    cv::Mat hL, hR;
    cv::stereoRectifyUncalibrated(*pL, *pR, f, imageL.size(), hL, hR);
    cv::warpPerspective(imageA, *imageL2, hL, imageL.size());
    cv::warpPerspective(imageB, *imageR2, hR, imageR.size());

    cv::imshow("rectify left", *imageL2);
    cv::waitKey(0);
    cv::imshow("rectify right", *imageR2);
    cv::waitKey(0);
}

void Disparity::computeDisparity(cv::Mat imageL, cv::Mat imageR) {
    auto sbm = cv::StereoBM::create();
    cv::Mat disparity;

    sbm->compute(imageL, imageR,disparity);

    double min, max;

    cv::minMaxLoc(disparity, &min, &max);

    double coeff = 255.0/(max - min);
    double offset = -min * 255.0/(max - min);

    disparity = offset + disparity * coeff;

    disparity.convertTo(disparity, CV_8U);

    cv::imshow("Disparité finale" , disparity);
    cv::waitKey(0);
}