#ifndef IPROJECTION_H
#define IPROJECTION_H

#include <opencv2/core/core.hpp>

class IProjection
{
public:
    /**
     * @brief project Projects (transforms) point from world coordinate 3D to image coordinate 2D, where
     * camera is in (0,0,0) coordinates before translation and is oriented along Z positive axis before
     * rotation. Coordinate system is right-handed x,y,z (as openGL).
     * @param pointInWorld Point in 3D to be projected onto image plane.
     * @return 2D point at image plane that is projection of pointInWorld.
     */
    cv::Point2d project(cv::Point3d pointInWorld,cv::Point3d cameraRotation,
        cv::Point3d cameraTranslation)=0;
protected:
    IProjection(){}
};

#endif // IPROJECTION_H
