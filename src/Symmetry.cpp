//
// Created by hung9 on 21/10/2021.
//

#include "Symmetry.h"

void Symmetry::isSymmetric(cv::Mat im) {
    cv::Mat im1 = im;
    cv::Mat im2;
    cv::Mat houghIm;
    im.copyTo(houghIm);

    cv::flip(im, im2, 1);

    cv::Ptr<cv::ORB> detection = cv::ORB::create(5000);

    std::vector<cv::KeyPoint> points1, points2;

    cv::Mat description1, description2;

    detection->detect(im1, points1);
    detection->compute(im1, points1, description1);

    detection->detect(im2, points2);
    detection->compute(im2, points2, description2);

    std::vector<std::vector<cv::DMatch>> match;

    cv::BFMatcher matcher;
    matcher.knnMatch(description1, description2, match, 2);

    std::vector<cv::DMatch> matchShort;

    for(auto m : match){
        if(m[0].distance < 0.75 * m[1].distance) matchShort.push_back(m[0]);
    }

    cv::Mat result = im1;

    cv::Mat resultMatch;
    cv::drawMatches(im1, points1, im2, points2, matchShort,resultMatch);

    cv::imshow("symetrymatch", resultMatch);

    std::vector<cv::Point2f> im1Points, im2Points;

    for (int j = 0; j < matchShort.size(); j++) {
        if(points1.size() > matchShort.at(j).queryIdx && points2.size() > matchShort.at(j).trainIdx) {
            im1Points.push_back(points1[matchShort.at(j).queryIdx].pt);
            im2Points.push_back(points2[matchShort.at(j).trainIdx].pt);
        }
    }

    /*for(int i = 0; i < im2Points.size(); i++){
    }*/

    std::vector<cv::Point2f> barycentre;
    cv::Mat baryResult(im.size(), CV_8U);

    //pas de shame, pour des raisons obscures le zeros ne fonctionne pas chez moi, ça me donne une image grise
    for(int i = 0; i < baryResult.rows; i++){
        for(int j = 0; j < baryResult.cols; j++){
            baryResult.data[i* baryResult.cols + j] = 0;
        }
    }

    for(int i = 0; i < im1Points.size(); i++){
        im2Points.at(i).x = (im2.cols - im2Points.at(i).x) -1;
        barycentre.push_back((im2Points.at(i) + im1Points.at(i))/2);

        cv::line(result, im1Points.at(i), im2Points.at(i), cv::Scalar(255,0,0), 2);
        cv::circle(result, barycentre.at(i),5, cv::Scalar(0,0,255),2);
        cv::circle(baryResult, barycentre.at(i), 5, cv::Scalar(255,255,255), 2);
    }


    std::vector<cv::Vec2f> lines;
    cv::HoughLines(baryResult, lines, 1, CV_PI/180, 10,0);

    float rho = lines[1][0], theta = lines[1][1];
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = cvRound(x0 + 1000*(-b));
    pt1.y = cvRound(y0 + 1000*(a));
    pt2.x = cvRound(x0 - 1000*(-b));
    pt2.y = cvRound(y0 - 1000*(a));
    cv::line( houghIm, pt1, pt2, cv::Scalar(0,255,0), 3, cv::LINE_AA);


    cv::imshow("hough", houghIm);
    cv::imshow("barycenter map", baryResult);
    cv::imshow("symmetry", result);
    cv::waitKey(0);

}