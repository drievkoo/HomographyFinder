#ifndef HOMOGRAPHYBUILDER
#define HOMOGRAPHYBUILDER

#include "homography.h"
#include "iprojection.h"
#include "virtualgrid.h"
#include <opencv2/core/core.hpp>

class HomographyBuilder{

public:
    HomographyBuilder(IProjection *projector,VirtualGrid grid,cv::Point3d cameraRotation,
        cv::Point3d cameraTranslation){

    }

    ~HomographyBuilder(){

    }

    Homography toHomography(){

    }

private:
};

#endif // HOMOGRAPHYBUILDER

