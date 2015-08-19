#ifndef ABSTRACTGRIDDRAWER_H
#define ABSTRACTGRIDDRAWER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "igriddrawer.h"
#include "virtualgrid.h"

class AbstractGridDrawer : public IGridDrawer
{
public:
    static const cv::Point3d defaultCameraLocation;
    static const cv::Point3d defaultCameraOrientation;
protected:
    cv::Point3d cameraLocation;
    cv::Point3d cameraOrientation;

    AbstractGridDrawer(){
        cameraLocation=defaultCameraLocation;
        cameraOrientation=defaultCameraOrientation;
    }

    virtual ~AbstractGridDrawer(){

    }
public:
    static cv::Point3d calculateOrientationVector(cv::Point3d originalOrientationVector,
    cv::Point3d rotationVectorAnglesInDegrees){
        double degToRad=CV_PI/180.0;
        double x=rotationVectorAnglesInDegrees.x*degToRad,y=rotationVectorAnglesInDegrees.y*degToRad,
            z=rotationVectorAnglesInDegrees.z*degToRad;
        cv::Mat rotationMatrix=(cv::Mat_<double>(3,3)<<
            cos(y)*cos(z),sin(x)*sin(y)*cos(z)-cos(x)*sin(z),cos(x)*sin(y)*cos(z)+sin(x)*sin(z),
            cos(y)*sin(z),sin(x)*sin(y)*sin(z)+cos(x)*cos(z),cos(x)*sin(y)*sin(z)-sin(x)*cos(z),
            -sin(y),sin(x)*cos(y),cos(x)*cos(y));
        cv::Mat vectorInMatrixForm=(cv::Mat_<double>(3,1)<<
            originalOrientationVector.x,
            originalOrientationVector.y,
            originalOrientationVector.z);
        cv::Mat orientationVectorInMatrixForm=rotationMatrix*vectorInMatrixForm;
        cv::Point3d resultOrientationVector(orientationVectorInMatrixForm.at<double>(0,0),
            orientationVectorInMatrixForm.at<double>(1,0),
            orientationVectorInMatrixForm.at<double>(2,0));
        double resultOrientationVectorMagnitude=sqrt(resultOrientationVector.x*resultOrientationVector.x+
            resultOrientationVector.y*resultOrientationVector.y+resultOrientationVector.z*resultOrientationVector.z);
        return round(cv::Point3d(resultOrientationVector.x/resultOrientationVectorMagnitude,
            resultOrientationVector.y/resultOrientationVectorMagnitude,
            resultOrientationVector.z/resultOrientationVectorMagnitude));
    }

    static cv::Point3d calculateRotationAnglesBetween(cv::Point3d currentOrientationVector,
    cv::Point3d desiredOrientationVector){
        double radToDeg=180./CV_PI;
        cv::Point3d eulerRotations;
        cv::Point3d rotationAxis=currentOrientationVector.cross(desiredOrientationVector);
        double rotationAngle=acos(currentOrientationVector.dot(desiredOrientationVector)/
            ((currentOrientationVector.x*currentOrientationVector.x+currentOrientationVector.y*
              currentOrientationVector.y+currentOrientationVector.z*currentOrientationVector.z)
             *(desiredOrientationVector.x*desiredOrientationVector.x+desiredOrientationVector.y*
               desiredOrientationVector.y+desiredOrientationVector.z*desiredOrientationVector.z)));
        double x=rotationAxis.x,y=rotationAxis.y,z=rotationAxis.z;
        double s=sin(rotationAngle);
        double c=cos(rotationAngle);
        double t=1-c;
        double magnitude = sqrt(x*x + y*y + z*z);
        if (magnitude==0){
            return cv::Point3d(0,0,0);
        }
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
        if ((x*y*t + z*s) > 0.998) { // north pole singularity detected
            //heading = 2*atan2(x*Math.sin(angle/2),Math.cos(angle/2));
            //attitude = Math.PI/2;
            //bank = 0;
            eulerRotations.y=2*atan2(x*sin(rotationAngle/2),cos(rotationAngle/2));
            eulerRotations.z=CV_PI/2;
            eulerRotations.x=0;
            return eulerRotations;
        }
        if ((x*y*t + z*s) < -0.998) { // south pole singularity detected
            //heading = -2*atan2(x*Math.sin(angle/2),Math.cos(angle/2));
            //attitude = -Math.PI/2;
            //bank = 0;
            eulerRotations.y=-2*atan2(x*sin(rotationAngle/2),cos(rotationAngle/2));
            eulerRotations.z=-CV_PI/2;
            eulerRotations.x=0;
            return eulerRotations;
        }
        //heading = Math.atan2(y * s- x * z * t , 1 - (y*y+ z*z ) * t);
        //attitude = Math.asin(x * y * t + z * s) ;
        //bank = Math.atan2(x * s - y * z * t , 1 - (x*x + z*z) * t);
        eulerRotations.y=atan2(y * s- x * z * t , 1 - (y*y+ z*z ) * t);
        eulerRotations.z=asin(x * y * t + z * s);
        eulerRotations.x=atan2(x * s - y * z * t , 1 - (x*x + z*z) * t);
        return cv::Point3d(eulerRotations.x*radToDeg,
            eulerRotations.y*radToDeg,
            eulerRotations.z*radToDeg);
    }

    virtual cv::Mat drawGrid(cv::Mat image,VirtualGrid grid){
        std::vector<std::pair<cv::Point3d,cv::Point3d>> gridLines=grid.getLines();
        clearCanvas();
        prepareCamera();
        for(int i=0;i<gridLines.size();++i){
            std::pair<cv::Point3d,cv::Point3d> line=gridLines[i];
            drawLine3D(line.first,line.second);
        }
        cv::Mat drawnGrid=getDrawnImage(image.size());
        return drawnGrid+image;
    }

    virtual void setCameraLocationAndOrientation(cv::Point3d newCameraLocation,cv::Point3d newCameraOrientation){
        cameraLocation=newCameraLocation;
        cameraOrientation=newCameraOrientation;
    }

    virtual void moveCamera(cv::Point3d newCameraMoveVector){
        cameraLocation+=newCameraMoveVector;
    }

    virtual void rotateCamera(cv::Point3d newCameraRotationAnglesInDegrees){
        cameraOrientation=calculateOrientationVector(cameraOrientation,newCameraRotationAnglesInDegrees);
    }

    virtual cv::Point3d getCameraLocation(){
        return cameraLocation;
    }

    virtual cv::Point3d getCameraOrientation(){
        return cameraOrientation;
    }

    virtual void clearCanvas()=0;
    virtual void drawLine3D(cv::Point3d startPoint,cv::Point3d stopPoint)=0;
    virtual cv::Mat getDrawnImage(cv::Size imageSize)=0;
    virtual void prepareCamera()=0;

    class PerspectiveSettings{
    public:
        enum Configuration{FOVX,FOVY};

        PerspectiveSettings(){
            fov=0;
            aspectRatio=0;
        }

        void change(Configuration newConfiguration,double pfov,double paspectRatio){
            currentConfiguration=newConfiguration;
            fov=pfov;
            aspectRatio=paspectRatio;
        }

        void changeAspectRatio(double newAspectRatio){
            aspectRatio=newAspectRatio;
        }

        double getFovX(){
            if(currentConfiguration==FOVX){
                return fov;
            }else{
                return fov*aspectRatio;
            }
        }

        double getFovY(){
            if(currentConfiguration==FOVX){
                return fov/aspectRatio;
            }else{
                return fov;
            }
        }

        double getAspectRatio(){
            return aspectRatio;
        }

        Configuration getConfiguration(){
            return currentConfiguration;
        }

        QString toString(){
            double fovX;
            double fovY;
            if(currentConfiguration==FOVX){
                fovX=fov;
                fovY=fovX/aspectRatio;
            }else{
                fovY=fov;
                fovX=fovY*aspectRatio;
            }
            return QString("FOVX = %1 FOVY = %2").arg(fovX).arg(fovY);
        }
    private:
        Configuration currentConfiguration;
        double fov;
        double aspectRatio;
    };

private:
    static cv::Point3d round(cv::Point3d value){
        return cv::Point3d(round(value.x),round(value.y),round(value.z));
    }

    static double round(double doubleValue){
        return floor(doubleValue*1000.)/1000.;
    }
};

/*#ifndef  ABSTRACTGRIDDRAWER_H_CONSTANTS
#define ABSTRACTGRIDDRAWER_H_CONSTANTS
const cv::Point3i AbstractGridDrawer::defaultCameraLocation=cv::Point3i(0,0,0);
const cv::Point3d AbstractGridDrawer::defaultCameraOrientation=cv::Point3d(0,0,1);
#endif*/

#endif // ABSTRACTGRIDDRAWER_H
