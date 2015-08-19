#ifndef VIRTUALGRID
#define VIRTUALGRID

#include <opencv2/core/core.hpp>
#include "planeparallel.h"

/**
 * @brief The VirtualGrid class reprezents line grid in 3D coordinate system. Grid is placed on
 * plane that is parallel to one of the planes: xy, yz, xz. Coordinates in plane are interpreted
 * in order: (X,Y), (Y,Z) or (X,Z) respectively.
 */
class VirtualGrid{
    int unitInPixels;
    int linesCount[2];
    PlaneParallel planeParalelism;
    cv::Point3d bottomLeftGridCorner;
    bool alwaysCenterInOrigin;
public:

    VirtualGrid(cv::Point3d planeOrigin=cv::Point3d(0,0,0),bool palwaysCenterInOrigin=true,
    PlaneParallel pPlaneParalelism=PlaneParallel::XY_PARALLEL,cv::Size pLineCount=cv::Size(11,8)){
        unitInPixels=10;
        planeParalelism=pPlaneParalelism;
        linesCount[0]=pLineCount.width;
        linesCount[1]=pLineCount.height;
        if(linesCount[0]<2||linesCount[1]<2)
            throw std::exception("Line count must be >= 2.");
        bottomLeftGridCorner=planeOrigin;
        alwaysCenterInOrigin=palwaysCenterInOrigin;
        if(alwaysCenterInOrigin)
            setCenterToOrigin();
    }

    ~VirtualGrid(){

    }

    void setCenterToOrigin(){
        cv::Point3d currentGridCenter;
        int gridWidth=(linesCount[0]-1);
        int gridHeight=(linesCount[1]-1);
        currentGridCenter=translatePoint3D(bottomLeftGridCorner,cv::Point2f(float(gridWidth)/2.,
            float(gridHeight)/2.));
        switch(planeParalelism){
            case PlaneParallel::XY_PARALLEL:
                bottomLeftGridCorner+=cv::Point3d(-currentGridCenter.x,-currentGridCenter.y,0);
                break;
            case PlaneParallel::YZ_PARALLEL:
                bottomLeftGridCorner+=cv::Point3d(0,-currentGridCenter.y,-currentGridCenter.z);
                break;
            case PlaneParallel::XZ_PARALLEL:
                bottomLeftGridCorner+=cv::Point3d(-currentGridCenter.x,0,-currentGridCenter.z);
                break;
        }
    }

    void setUnitInPixels(int unitSizeInPixels){
        unitInPixels=unitSizeInPixels;
    }

    cv::Size getLineCount(){
        return cv::Size(linesCount[0],linesCount[1]);
    }

    int getUnitInPixels(){
        return unitInPixels;
    }

    void translateInPixels(cv::Point translationVector){
        if(alwaysCenterInOrigin)
            return;
        bottomLeftGridCorner=translatePoint3D(bottomLeftGridCorner,
            cv::Point2f(float(translationVector.x)/unitInPixels,float(translationVector.y)/unitInPixels));
    }

    void changeLinesDensity(float denseFactor){
        int newLinesCount[2];
        newLinesCount[0]=int(linesCount[0]*denseFactor);
        newLinesCount[1]=int(linesCount[1]*denseFactor);
        if(newLinesCount[0]<2||newLinesCount[1]<2)
            return;
        //int d1=newLinesCount[0]-linesCount[0],d2=newLinesCount[1]-linesCount[1];
        /*if(d1>0&&d2>0)
            bottomLeftGridCorner=translatePoint3D(bottomLeftGridCorner,cv::Point2f(-d1/2.,-d2/2.));
        else
            bottomLeftGridCorner=translatePoint3D(bottomLeftGridCorner,cv::Point2f(-d1/2.,-d2/2.));
        */linesCount[0]=newLinesCount[0];
        linesCount[1]=newLinesCount[1];
        if(alwaysCenterInOrigin)
            setCenterToOrigin();
    }

    std::vector<std::pair<cv::Point3d,cv::Point3d>> getLines(){
        std::vector<std::pair<cv::Point3d,cv::Point3d>> gridLines;
        int gridWidth=(linesCount[0]-1);
        int gridHeight=(linesCount[1]-1);
        for(int i=0;i<linesCount[0];++i){
            cv::Point3d start=translatePoint3D(bottomLeftGridCorner,cv::Point(i,0));
            cv::Point3d stop=translatePoint3D(bottomLeftGridCorner,cv::Point(i,gridHeight));
            gridLines.push_back(std::make_pair(start,stop));
        }
        for(int j=0;j<linesCount[1];++j){
            cv::Point3d start=translatePoint3D(bottomLeftGridCorner,cv::Point(0,j));
            cv::Point3d stop=translatePoint3D(bottomLeftGridCorner,cv::Point(gridWidth,j));
            gridLines.push_back(std::make_pair(start,stop));
        }
        return gridLines;
    }

    std::vector<cv::Point3d> getGridCornerPoints(){
        std::vector<cv::Point3d> cornerPoints;
        int gridWidth=(linesCount[0]-1);
        int gridHeight=(linesCount[1]-1);
        cornerPoints.push_back(bottomLeftGridCorner);
        cornerPoints.push_back(translatePoint3D(bottomLeftGridCorner,cv::Point(0,gridHeight)));
        cornerPoints.push_back(translatePoint3D(bottomLeftGridCorner,cv::Point(gridWidth,gridHeight)));
        cornerPoints.push_back(translatePoint3D(bottomLeftGridCorner,cv::Point(gridWidth,0)));
        return cornerPoints;
    }

private:
    cv::Point3i translatePoint3D(cv::Point3d origin,cv::Point2f translationVectorInPixels){
        switch(planeParalelism){
            case PlaneParallel::XY_PARALLEL:
                origin+=cv::Point3d(translationVectorInPixels.x,translationVectorInPixels.y,0);
                break;
            case PlaneParallel::YZ_PARALLEL:
                origin+=cv::Point3d(0,translationVectorInPixels.x,translationVectorInPixels.y);
                break;
            case PlaneParallel::XZ_PARALLEL:
                origin+=cv::Point3d(translationVectorInPixels.x,0,translationVectorInPixels.y);
                break;
        }
        return origin;
    }
};

#endif // VIRTUALGRID

