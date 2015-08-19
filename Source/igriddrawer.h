#ifndef IGRIDDRAWER_H
#define IGRIDDRAWER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "virtualgrid.h"

class IGridDrawer
{
public:
    /**
     * @brief drawGrid Draws grid specified in grid in image, where camera is translated and
     * rotated from its default position  set by 'setCameraLocationAndOrientation'. By defaults
     * at (0,0,0) and facing along positive Z axis. Coordinate system is right-handed as in openGL.
     * @param image Image that is used for background.
     * @param grid Grid to be drawn.
     * @return Image with drawn grid in it.
     */
    virtual cv::Mat drawGrid(cv::Mat image,VirtualGrid grid)=0;

    /**
     * @brief setCameraLocationAndOrientation changes default camera location that is (0,0,0) and default
     * camera orientation that is along posiive Z axis.
     * @param newCameraLocation New default camera location.
     * @param newCameraOrientation New default camera orientation given by normal vector of image plane.
     */
    virtual void setCameraLocationAndOrientation(cv::Point3d newCameraLocation,cv::Point3d newCameraOrientation)=0;
protected:
    IGridDrawer(){}
    virtual ~IGridDrawer(){}
};

#endif // IGRIDDRAWER_H
