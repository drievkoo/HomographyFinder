#ifndef VIRTUALGRID
#define VIRTUALGRID

#include <opencv2/core/core.hpp>

/**
 * @brief The VirtualGrid class reprezents line grid in 3D coordinate system. Grid is placed on
 * custom plane x=0, y=0 or z=0.
 */
class VirtualGrid{
    int unitInPixels;
    int linesCount[2];
    cv::Point3d bottomLeftGridCorner;
public:

    VirtualGrid(cv::Point3i gridPlane){
        unitInPixels=10;
    }

    ~VirtualGrid(){

    }

    void setUnitInPixels(int unitSizeInPixels){

    }

    int getUnitInPixels(){
        return unitInPixels;
    }

    void translateInPixels(cv::Point translationVector){

    }

    void rebuildMoreDense(float denseFactor){

    }

    void rebuildLessDense(float denseFactor){

    }

    std::vector<std::pair<cv::Point3d,cv::Point3d>> getLines(){

    }

private:
};

#endif // VIRTUALGRID

