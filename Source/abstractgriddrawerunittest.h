#ifndef ABSTRACTGRIDDRAWERUNITTEST
#define ABSTRACTGRIDDRAWERUNITTEST

#include <opencv2/core/core.hpp>
#include <iostream>
#include "abstractgriddrawer.h"

class AbstractGridDrawerUnitTest{
public:
    bool test(){
        bool ok=true;
        cv::Point3d resultOK;
        cv::Point3d result;

        resultOK=cv::Point3d(0,1,0);
        result=AbstractGridDrawer::calculateOrientationVector(cv::Point3d(0,0,1),
            cv::Point3d(-90,0,0));
        //result=round(result);
        //std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        resultOK=cv::Point3d(1,0,0);
        result=AbstractGridDrawer::calculateOrientationVector(cv::Point3d(0,0,1),
            cv::Point3d(0,90,0));
        //result=round(result);
        //std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        resultOK=cv::Point3d(0,90,0);
        result=AbstractGridDrawer::calculateRotationAnglesBetween(cv::Point3d(0,0,1),
            cv::Point3d(1,0,0));
        //result=round(result);
        std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        resultOK=cv::Point3d(-90,0,0);
        result=AbstractGridDrawer::calculateRotationAnglesBetween(cv::Point3d(0,0,1),
            cv::Point3d(0,1,0));
        //result=round(result);
        std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        resultOK=cv::Point3d(-90,0,-45);
        result=AbstractGridDrawer::calculateRotationAnglesBetween(cv::Point3d(0,0,1),
            cv::Point3d(1,1,0));
        //result=round(result);
        std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        resultOK=cv::Point3d(0,0,0);
        result=AbstractGridDrawer::calculateRotationAnglesBetween(cv::Point3d(0,0,1),
            cv::Point3d(0,0,1));
        //result=round(result);
        std::cerr<<"result "<<result<<std::endl;
        ok=ok && result==resultOK;

        return ok;
    }
private:
    cv::Point3d round(cv::Point3d value){
        return cv::Point3d(round(value.x),round(value.y),round(value.z));
    }

    double round(double doubleValue){
        return floor(doubleValue*1000.)/1000.;
    }
};

#endif // ABSTRACTGRIDDRAWERUNITTEST

