#ifndef GRIDDRAWER
#define GRIDDRAWER

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include <opencv2/core/core.hpp>
#include "virtualgrid.h"
#include "abstractgriddrawer.h"
#include "iprojection.h"

class GridDrawerOpenGL : public AbstractGridDrawer, public IProjection
{
public:
    GridDrawerOpenGL(){

    }

    ~GridDrawerOpenGL(){

    }

    virtual void clearCanvas(){
        throw std::exception("Not implemented yet.");
    }

    virtual void drawLine3D(cv::Point3i startPoint,cv::Point3i stopPoint){
        throw std::exception("Not implemented yet.");
    }

    virtual cv::Mat getDrawnImage(cv::Size imageSize){
        throw std::exception("Not implemented yet.");
    }

    virtual cv::Point2d project(cv::Point3d pointInWorld,cv::Point3d cameraOrientation,
    cv::Point3i cameraLocation){
        throw std::exception("Not implemented yet.");
    }
};

#endif // GRIDDRAWER

