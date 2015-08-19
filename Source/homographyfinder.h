#ifndef HOMOGRAPHYFINDER
#define HOMOGRAPHYFINDER

#include <opencv2/core/core.hpp>
#include "virtualgrid.h"
#include "homographybuilder.h"
#include "igriddrawer.h"
#include "iprojection.h"
#include "abstractgriddrawer.h"

class HomographyFinder{
    VirtualGrid grid;
    IGridDrawer *gridDrawer;
    IProjection *projector;
    cv::Size imageSize;
    cv::Point3d cameraLocation;
    cv::Point3d cameraOrientation;
    const double denseFactor;


public:
    HomographyFinder(IGridDrawer *pGridDrawer,IProjection *pProjector):
    denseFactor(2.),grid(cv::Point3d(0,0,0)){
        setCameraLocationAndOrientation(cv::Point3d(0,0,20),cv::Point3d(0.,0.,0.));
        gridDrawer=pGridDrawer;
        projector=pProjector;
        if(gridDrawer==NULL)
            throw std::exception("Grid drawer not specified.");
        if(projector==NULL)
            throw std::exception("Projector not specified.");
    }

    ~HomographyFinder(){

    }

    VirtualGrid getGrid(){
        return grid;
    }

    void setCameraLocationAndOrientation(cv::Point3d newCameraLocation,
    cv::Point3d newCameraOrientation){
        cameraLocation=newCameraLocation;
        cameraOrientation=newCameraOrientation;
    }

    cv::Point3d getCameraOrientation(){
        return cameraOrientation;
    }

    cv::Point3d getCameraLocation(){
        return cameraLocation;
    }

    void rotateCameraAroundXAxis(double angleInDegrees){
        //cameraOrientation=AbstractGridDrawer::calculateOrientationVector(cameraOrientation,
        //    cv::Point3d(angleInDegrees,0,0));
        cameraOrientation.x+=angleInDegrees;
    }

    void rotateCameraAroundYAxis(double angleInDegrees){
        //cameraOrientation=AbstractGridDrawer::calculateOrientationVector(cameraOrientation,
        //    cv::Point3d(0,angleInDegrees,0));
        cameraOrientation.y+=angleInDegrees;
    }

    void rotateCameraAroundZAxis(double angleInDegrees){
        //cameraOrientation=AbstractGridDrawer::calculateOrientationVector(cameraOrientation,
        //    cv::Point3d(0,0,angleInDegrees));
        cameraOrientation.z+=angleInDegrees;
    }

    void translateCamera(cv::Point3d deltaInUnits){
       // int unitInPixels=getUnitSizeInPixels();
        cameraLocation+=cv::Point3d(deltaInUnits.x,
            deltaInUnits.y,deltaInUnits.z);
    }

    void shiftGridLeft(int shiftInPixels){
        grid.translateInPixels(cv::Point(shiftInPixels,0));
    }

    void shiftGridRight(int shiftInPixels){
        grid.translateInPixels(cv::Point(-shiftInPixels,0));
    }

    void shiftGridUp(int shiftInPixels){
        grid.translateInPixels(cv::Point(0,shiftInPixels));
    }

    void shiftGridDown(int shiftInPixels){
        grid.translateInPixels(cv::Point(0,-shiftInPixels));
    }

    void moreDenseGrid(){
        grid.changeLinesDensity(denseFactor);
    }

    void lessDenseGrid(){
        grid.changeLinesDensity(1./denseFactor);
    }

    int getUnitSizeInPixels(){
        return grid.getUnitInPixels();
    }

    Homography getCurrentHomographyMatrix(){
        HomographyBuilder homographyBuilder(projector,grid,imageSize,cameraOrientation,
            cameraLocation);
        return homographyBuilder.toHomography();
    }

    cv::Mat drawVirtualGridOn(cv::Mat image){
        gridDrawer->setCameraLocationAndOrientation(cameraLocation,cameraOrientation);
        return gridDrawer->drawGrid(image,grid);
    }

private:

};

#endif // HOMOGRAPHYFINDER

