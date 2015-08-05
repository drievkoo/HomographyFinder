#ifndef HOMOGRAPHYFINDER
#define HOMOGRAPHYFINDER

#include <opencv2/core/core.hpp>
#include "virtualgrid.h"
#include "homographybuilder.h"
#include "igriddrawer.h"
#include "iprojection.h"

class HomographyFinder{
    VirtualGrid grid;
    IGridDrawer *gridDrawer;
    IProjection *projector;
    cv::Size imageSize;
    cv::Point3d cameraTranslation;
    cv::Point3d cameraRotation;
    const double denseFactor;

public:
    HomographyFinder(IGridDrawer *pGridDrawer,IProjection *pProjector):
    denseFactor(2.){
        resetCameraRotationAndTranslation();
        gridDrawer=pGridDrawer;
        projector=pProjector;
        if(gridDrawer==NULL)
            throw std::exception("Grid drawer not specified.");
        if(projector==NULL)
            throw std::exception("Projector not specified.");
    }

    ~HomographyFinder(){

    }

    void resetCameraRotationAndTranslation(){
        cameraTranslation=cv::Point3d(0.,0.,0.);
        cameraRotation=cv::Point3d(0.,0.,0.);
    }

    void rotateCameraAroundXAxis(double angleInDegrees){
        cameraRotation.x+=angleInDegrees;
    }

    void rotateCameraAroundYAxis(double angleInDegrees){
        cameraRotation.y+=angleInDegrees;
    }

    void rotateCameraAroundZAxis(double angleInDegrees){
        cameraRotation.z+=angleInDegrees;
    }

    void translateCamera(cv::Point3d delta){
        cameraTranslation+=delta;
    }

    void shiftGridLeft(int shiftInPixels){
        virtualGrid.translateInPixels(cv::Point(shiftInPixels,0));
    }

    void shiftGridRight(int shiftInPixels){
        virtualGrid.translateInPixels(cv::Point(-shiftInPixels,0));
    }

    void shiftGridUp(int shiftInPixels){
        virtualGrid.translateInPixels(cv::Point(0,shiftInPixels));
    }

    void shiftGridDown(int shiftInPixels){
        virtualGrid.translateInPixels(cv::Point(0,-shiftInPixels));
    }

    void moreDenseGrid(){
        virtualGrid.rebuildMoreDense(denseFactor);
    }

    void lessDenseGrid(){
        virtualGrid.rebuildLessDense(denseFactor);
    }

    int getUnitSizeInPixels(){
        return virtualGrid.getUnitInPixels();
    }

    Homography getCurrentHomographyMatrix(){
        HomographyBuilder homographyBuilder(projector,grid,cameraRotation,
            cameraTranslation);
        return homographyBuilder.toHomography();
    }

    void drawVirtualGridOn(cv::Mat image){
        gridDrawer->drawGrid(image,grid,cameraRotation,cameraTranslation);
    }

private:

};

#endif // HOMOGRAPHYFINDER

