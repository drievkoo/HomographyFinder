#ifndef HOMOGRAPHYBUILDER
#define HOMOGRAPHYBUILDER

#include "homography.h"
#include "iprojection.h"
#include "virtualgrid.h"
#include <opencv2/core/core.hpp>

class HomographyBuilder{
    IProjection *projector;
    VirtualGrid grid;
    cv::Point3d cameraOrientation;
    cv::Point3d cameraLocation;
    cv::Size imageSize;
public:
    HomographyBuilder(IProjection *pProjector,VirtualGrid pGrid,cv::Size pimageSize,cv::Point3d pCameraOrientation,
    cv::Point3d pCameraLocation){
        projector=pProjector;
        if(projector==NULL)
            throw std::exception("Projection not specified.");
        grid=pGrid;
        cameraOrientation=pCameraOrientation;
        cameraLocation=pCameraLocation;
        imageSize=pimageSize;
    }

    ~HomographyBuilder(){

    }

    Homography toHomography(){
        std::vector<cv::Point2f> pointsOnGridPlane;
        cv::Size lineCount=grid.getLineCount();
        pointsOnGridPlane.push_back(cv::Point2f(0.,0.));
        pointsOnGridPlane.push_back(cv::Point2f(0.,lineCount.height));
        pointsOnGridPlane.push_back(cv::Point2f(lineCount.width,lineCount.height));
        pointsOnGridPlane.push_back(cv::Point2f(lineCount.width,0.));
        std::vector<cv::Point2f> pointsOnImagePlane;
        std::vector<cv::Point3d> gridCorners3D=grid.getGridCornerPoints();
        cv::Point2d screenPoint;
        screenPoint=projector->project(gridCorners3D[0],cameraOrientation,cameraLocation);
        pointsOnImagePlane.push_back(cv::Point2f(screenPoint.x,imageSize.height-screenPoint.y));
        screenPoint=projector->project(gridCorners3D[1],cameraOrientation,cameraLocation);
        pointsOnImagePlane.push_back(cv::Point2f(screenPoint.x,imageSize.height-screenPoint.y));
        screenPoint=projector->project(gridCorners3D[2],cameraOrientation,cameraLocation);
        pointsOnImagePlane.push_back(cv::Point2f(screenPoint.x,imageSize.height-screenPoint.y));
        screenPoint=projector->project(gridCorners3D[3],cameraOrientation,cameraLocation);
        pointsOnImagePlane.push_back(cv::Point2f(screenPoint.x,imageSize.height-screenPoint.y));
        return Homography(pointsOnImagePlane,pointsOnGridPlane);
    }

private:
};

#endif // HOMOGRAPHYBUILDER

