#ifndef IPROJECTION_H
#define IPROJECTION_H

#include <opencv2/core/core.hpp>

class IProjection
{
public:
    /**
     * @brief project Projects (transforms) point from world coordinate 3D to image coordinate 2D.
     * @param pointInWorld Point in 3D to be projected onto image plane.
     * @param cameraOrientation Camera orientation vector, normal vector of image plane.
     * @param cameraLocation Camera location.
     * @return 2D point at image plane that is projection of pointInWorld.
     */
    virtual cv::Point2d project(cv::Point3d pointInWorld,cv::Point3d cameraOrientation,
        cv::Point3d cameraLocation)=0;
protected:
    IProjection(){}
    virtual ~IProjection(){}
};

#endif // IPROJECTION_H
