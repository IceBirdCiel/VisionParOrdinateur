//
// Created by hung9 on 28/11/2021.
//

#include "Tracking.h"

void Tracking::draw(){
    img = nextInput.clone();
    for(int i = 0; i < prevPoints.size(); i++)
    {
        cv::circle(img, prevPoints.at(i),5, cv::Scalar(0,0,255));
        cv::line(img,prevPoints.at(i),nextPoints.at(i) ,cv::Scalar(0,0,255), 2);
    }
    updateROI();
    cv::imshow("input", img);
    cv::waitKey(10);

}

void Tracking::video(const std::string& videoName){
    cv::VideoCapture vid;

    if(videoName == ""){
        vid = cv::VideoCapture(0);
    }
    else {
        vid = cv::VideoCapture(videoName);
    }

    if(!vid.isOpened()) return;

    cv::Mat frame;

    bool canRead = vid.read(frame);
    nextInput = frame;

    while(!frame.empty()){
        trackPoints();
        draw();

        canRead = vid.read(frame);

        if(!canRead) return;
        nextInput = frame;
    }
}

void Tracking::detectPoints(cv::Mat &img) {
    cv::Mat im;
    cv::cvtColor(img, im, cv::COLOR_BGR2GRAY);

    cv::Mat copy(im.size(), CV_8U);

    //pas de shame, pour des raisons obscures le zeros ne fonctionne pas chez moi, ça me donne une image grise
    for(int i = 0; i < copy.rows; i++){
        for(int j = 0; j < copy.cols; j++){
            copy.data[i* copy.cols + j] = 0;
        }
    }

    cv::rectangle(copy, roi.tl(), roi.br(), 255, -1);

    if(roi.area() > 10 && start.x < 0){
        cv::goodFeaturesToTrack(im, prevPoints, 100, 0.1, 8, copy);

    }
}

void Tracking::trackPoints(){
    if(!prevInput.empty()) {
        prevPoints.clear();
        prevPoints.assign(nextPoints.begin(), nextPoints.end());

        if(prevPoints.size() < 2){
            detectPoints(prevInput);
        }

        if (prevPoints.empty()) {
            return;
        }

        std::vector<uchar> status;
        std::vector<float> err;
        cv::calcOpticalFlowPyrLK(prevInput, nextInput, prevPoints, nextPoints, status, err);

        nextPoints = purgePoints(nextPoints, status);
        prevPoints = purgePoints(prevPoints, status);
    }
    prevInput = nextInput.clone();
}

std::vector<cv::Point2f> Tracking::purgePoints(std::vector<cv::Point2f>& points,
                                     std::vector<uchar>& status){
    std::vector<cv::Point2f> result;
    for(int i = 0; i < points.size(); ++i){
        if(status[i]>0)result.push_back(points[i]);
    } return result;
}

void Tracking::updateROI(){
    if(roi.empty() || start.x >= 0) return;

    cv::Point2f tl, br;
    tl = cv::Point2f(999,0);
    br = cv::Point2f (0,999);

    for(cv::Point2f p : nextPoints){
        if(p.x < tl.x){
            tl.x = p.x;
        }
        if(p.x > br.x){
            br.x = p.x;
        }
        if(p.y > tl.y){
            tl.y = p.y;
        }
        if(p.y < br.y){
            br.y = p.y;
        }
    }

    roi = cv::Rect(tl, br);

    cv::rectangle(img, roi, cv::Scalar(0,255,0), 5);
}