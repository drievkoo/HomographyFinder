#ifndef IGRIDDRAWER_H
#define IGRIDDRAWER_H

#include <opencv2/core/core.hpp>

class IGridDrawer
{
public:
    /**
     * @brief drawGrid Draws grid specified in grid in image, where camera is translated and
     * rotated from its default position at (0,0,0) and facing along positive Z axis. Coordinate
     * system is right-handed as in openGL.
     * @param image Image that is used for background.
     * @param grid Grid to be drawn.
     * @param cameraRotation Camera rotation from its default facing (positive Z).
     * @param cameraTranslation Camera translation from its default (0,0,0).
     * @return Image with drawn grid in it.
     */
    cv::Mat drawGrid(cv::Mat image,VirtualGrid grid,cv::Point3d cameraRotation,
            cv::Point3d cameraTranslation)=0;
protected:
    IGridDrawer(){}
};

#endif // IGRIDDRAWER_H
