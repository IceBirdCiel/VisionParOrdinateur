//
// Created by hung9 on 07/12/2021.
//

#include "Features.h"

Features::Features(cv::VideoCapture *vid, std::vector<cv::Mat> *images){
    this->vid = *vid;
    this->images = *images;
}

bool compare(cv::DMatch a, cv::DMatch b) { return a.distance < b.distance; }

void Features::show() {
    if(!vid.isOpened()) return;

    cv::Ptr<cv::ORB> detection = cv::ORB::create();
    cv::Ptr<cv::ORB> detectionVid = cv::ORB::create();

    std::vector<cv::KeyPoint> pointsIm;
    cv::Mat description; // caca la doc est pas à jour Dx
    cv::Mat descriptionVid;
    detection->detect(images.at(0), pointsIm);
    detection->compute(images.at(0), pointsIm, description);

    cv::BFMatcher matcher;
    std::vector<cv::DMatch> matches;
    std::vector<cv::DMatch> matchShort;

    cv::Mat frame;
    bool canRead = vid.read(frame);
    cv::Mat nextInput = frame;

    cv::Mat result;

    std::vector<cv::KeyPoint> pointsVid;

    std::vector<cv::Point2f> corners(4);
    std::vector<cv::Point2f> scene_corners(4);

    cv::Mat matTransform;

    while(!frame.empty()){
        canRead = vid.read(frame);

        detectionVid->detect(frame, pointsVid);
        detectionVid->compute(frame, pointsVid, descriptionVid);

        matcher.match(description, descriptionVid, matches);

        std::sort(matches.begin(), matches.end(), compare);
        for(int i = 0; i < 30; i++) matchShort.push_back(matches.at(i));

        //cv::drawMatches(images.at(0), pointsIm, frame, pointsVid, matchShort, result);

        std::vector<cv::Point2f> imagePoints;
        std::vector<cv::Point2f> scenePoints;

        for(int i = 0; i < matchShort.size(); i++){
            imagePoints.push_back(pointsIm[matchShort.at(i).queryIdx].pt);
            scenePoints.push_back(pointsVid[matchShort.at(i).trainIdx].pt);
        }

        matTransform = cv::findHomography(imagePoints, scenePoints, cv::RANSAC);

        corners[0] = cv::Point2f(0,0);
        corners[1] = cv::Point2f((float) images.at(0).cols, 0);
        corners[2] = cv::Point2f((float)images.at(0).cols, (float)images.at(0).rows);
        corners[3] = cv::Point2f(0, (float)images.at(0).rows);

        if(!matTransform.empty())
        {
            cv::perspectiveTransform(corners, scene_corners, matTransform);
            cv::line(frame, scene_corners[0], scene_corners[1], cv::Scalar(255,0,255), 2);
            cv::line(frame, scene_corners[1], scene_corners[2], cv::Scalar(255,0,255), 2);
            cv::line(frame, scene_corners[2], scene_corners[3], cv::Scalar(255,0,255), 2);
            cv::line(frame, scene_corners[3], scene_corners[0], cv::Scalar(255,0,255), 2);
        }


        cv::imshow("scene", frame);
        cv::waitKey(10);

        if(!canRead) return;
        nextInput = frame;

        matchShort.clear();
    }
}

