#ifndef HOMOGRAPHYFINDER
#define HOMOGRAPHYFINDER

#include <opencv2/core/core.hpp>
#include "virtualgrid.h"
#include "homographybuilder.h"

class HomographyFinder{
    VirtualGrid virtualGrid;
    HomographyBuilder homographyBuilder;
    cv::Size imageSize;

public:
    HomographyFinder(cv::Size pImageSize){
        imageSize=pImageSize;
    }
    ~HomographyFinder(){

    }
    void rotateGridAroundXAxis(double angleInRadians){

    }
    void rotateGridAroundYAxis(double angleInRadians){

    }
    void rotateGridAroundZAxis(double angleInRadians){

    }
    void shiftGridLeft(int shiftInPixels){

    }
    void shiftGridRight(int shiftInPixels){

    }
    void shiftGridUp(int shiftInPixels){

    }
    void shiftGridDown(int shiftInPixels){

    }
    void setGridOriginAt(cv::Point gridOriginInImage){

    }
    void moreDenseGrid(){

    }
    void lessDenseGrid(){

    }
    int getUnitSizeInPixels(){
        return virtualGrid.getUnitInPixels();
    }
    void translate(cv::Point3f delta){

    }
    Homography getCurrentHomographyMatrix(){
        return homographyBuilder.toHomography();
    }
    void drawVirtualGridOn(cv::Mat image){

    }

private:

};

#endif // HOMOGRAPHYFINDER

