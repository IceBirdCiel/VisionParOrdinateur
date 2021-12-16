//
// Created by hung9 on 07/12/2021.
//

#include "Features.h"

Features::Features(cv::VideoCapture *vid, std::vector<cv::Mat> *images){
    this->vid = *vid;
    this->images = *images;
}

bool compare(cv::DMatch a, cv::DMatch b) { return a.distance < b.distance; }

float angle(cv::Point2f a, cv::Point2f b, cv::Point2f c){
    float angle = atan2(c.y - b.y, c.x - b.x) - atan2(a.y - b.y, a.x-b.x);
    angle = angle * 180.0 / M_PI;
    if(angle < 0) return (angle + 360);
    else return angle;
}

void Features::show() {
    if(!vid.isOpened()) return;

    std::vector<cv::Scalar> colors;
    for(int i = 0; i < images.size(); i++){
        colors.push_back(cv::Scalar(rand()%255,rand()%255, rand()%255));
    }

    cv::Ptr<cv::ORB> detection = cv::ORB::create(1000);

    std::vector<std::vector<cv::KeyPoint>> pointsIm;
    pointsIm.resize(images.size());
    std::vector<cv::Mat> description; // caca la doc est pas à jour Dx
    description.resize(images.size());

    for(int i = 0; i < images.size(); i++) {
        detection->detect(images.at(i), pointsIm.at(i));
        detection->compute(images.at(i), pointsIm.at(i), description.at(i));
    }

    cv::BFMatcher matcher;
    matcher.add(description);
    matcher.train();

    cv::Mat frame;
    bool canRead = vid.read(frame);
    cv::Mat nextInput = frame;

    while(!frame.empty()){
        canRead = vid.read(frame);

        std::vector<cv::KeyPoint> pointsVid;
        cv::Mat descriptionVid;
        detection->detect(nextInput, pointsVid);
        detection->compute(nextInput, pointsVid, descriptionVid);

        std::vector<std::vector<cv::DMatch>> matches;
        matcher.knnMatch(descriptionVid, matches, 2);

        std::vector<cv::DMatch> matchShort;
        for(auto m : matches){
            if(m[0].distance < 0.81*m[1].distance){
                matchShort.push_back(m[0]);
            }
        }

        float angleUpRight;
        float angleBottomLeft;
        for (int i = 0; i < images.size(); i++) {
            std::vector<cv::Point2f> imagePoints;
            std::vector<cv::Point2f> scenePoints;
            for (int j = 0; j < matchShort.size(); j++) {
                if(pointsIm.at(i).size() > matchShort.at(j).trainIdx) {
                    imagePoints.push_back(pointsIm.at(i)[matchShort.at(j).trainIdx].pt);
                }
                if(pointsVid.size() > matchShort.at(j).queryIdx){
                    scenePoints.push_back(pointsVid[matchShort.at(j).queryIdx].pt);
                }
            }

            cv::Mat matTransform;
            if(imagePoints.size() == scenePoints.size() && imagePoints.size() >= 4) {
                matTransform = cv::findHomography(imagePoints, scenePoints, cv::RANSAC,4);
            }

            std::vector<cv::Point2f> corners(4);
            std::vector<cv::Point2f> scene_corners(4);
            corners[0] = cv::Point2f(0, 0);
            corners[1] = cv::Point2f(0,images.at(i).rows);
            corners[2] = cv::Point2f(images.at(i).cols, images.at(i).rows);
            corners[3] = cv::Point2f( images.at(i).cols,0);

            if (!matTransform.empty()) {
                cv::perspectiveTransform(corners, scene_corners, matTransform);
                angleUpRight = angle(scene_corners[0], scene_corners[1], scene_corners[2]);
                angleBottomLeft = angle(scene_corners[2], scene_corners[3], scene_corners[0]);

                if(angleUpRight > 70 && angleUpRight < 110 && angleBottomLeft > 70 && angleBottomLeft < 110){
                cv::line(nextInput, scene_corners[0], scene_corners[1], colors.at(i), 2);
                cv::line(nextInput, scene_corners[1], scene_corners[2], colors.at(i), 2);
                cv::line(nextInput, scene_corners[2], scene_corners[3], colors.at(i), 2);
                cv::line(nextInput, scene_corners[3], scene_corners[0], colors.at(i), 2);
                }
            }
        }

        cv::imshow("scene", frame);
        cv::waitKey(10);

        if(!canRead) return;
        nextInput = frame;
    }
}

