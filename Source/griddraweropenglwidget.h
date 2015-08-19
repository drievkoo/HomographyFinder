#ifndef GRIDDRAWEROPENGLWIDGET
#define GRIDDRAWEROPENGLWIDGET

#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include <opencv2/core/core.hpp>
#include "virtualgrid.h"
#include "abstractgriddrawer.h"
#include "iprojection.h"
#include <array>

class GridDrawerOpenGLWidget : public AbstractGridDrawer, public IProjection, public QGLWidget
{
private:
    AbstractGridDrawer::PerspectiveSettings perspectiveSettings;
public:
    GridDrawerOpenGLWidget(QWidget *parent = 0):QGLWidget(parent){
        //setBaseSize(QSize(900,900));
        //setMinimumSize(900,900);
        setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
        perspectiveSettings.change(AbstractGridDrawer::PerspectiveSettings::Configuration::FOVY,
            70.,3);
    }

    ~GridDrawerOpenGLWidget(){

    }

    virtual void clearCanvas(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    virtual void drawLine3D(cv::Point3d startPoint,cv::Point3d stopPoint){
        glDisable(GL_LIGHTING);
        qglColor(Qt::white);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex3f(startPoint.x,startPoint.y,startPoint.z);
        glVertex3f(stopPoint.x,stopPoint.y,stopPoint.z);
        glEnd();
        qglColor(Qt::green);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3f(startPoint.x,startPoint.y,startPoint.z);
        glVertex3f(stopPoint.x,stopPoint.y,stopPoint.z);
        glEnd();
        glEnable(GL_LIGHTING);
    }

    virtual cv::Mat getDrawnImage(cv::Size imageSize){
        cv::Mat image(imageSize,CV_8UC3,cv::Scalar(0,0,255));
        glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
        glPixelStorei(GL_PACK_ROW_LENGTH, image.step/image.elemSize());
        glReadPixels(0, 0, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        cv::Mat flipped;
        cv::flip(image, flipped, 0);
        return flipped;
    }

    void initializeGL(){
        qglClearColor(Qt::black);
        glShadeModel(GL_FLAT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void resizeGL(int pwidth, int pheight){
        glViewport(0, 0, pwidth, pheight);
        //glScissor(0, 0, pwidth, pheight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float front=1;
        float back=1000;

        /*float fov=perspectiveSettings.getFov();
        float aspectRatio=perspectiveSettings.getAspectRatio();
        float tangent = tanf(fov/2 * CV_PI/180.0);   // tangent of half fovY
        float height,width;
        if(perspectiveSettings.getConfiguration()==AbstractGridDrawer::PerspectiveSettings::
        Configuration::FOVY){
            height= front * tangent;           // half height of near plane
            width = height * aspectRatio;       // half width of near plane
        }else{
            width= front * tangent;           // half height of near plane
            height = width / aspectRatio;       // half width of near plane
        }*/

        double DegToRad=CV_PI/180.;
        double height= front * tan(perspectiveSettings.getFovY()/2*DegToRad);           // half height of near plane
        double width = front * tan(perspectiveSettings.getFovX()/2*DegToRad);       // half width of near plane

        // params: left, right, bottom, top, near, far
        glFrustum(-width, width, -height, height, front, back);

        //glFrustum(-pwidth/2, pwidth/2, -pheight/2, pheight/2, front, back);

        glMatrixMode(GL_MODELVIEW);
    }

    void changePerspectiveFovX(double fovX,double aspectRatio,cv::Size imageSize){
        perspectiveSettings.change(AbstractGridDrawer::PerspectiveSettings::Configuration::FOVX,
            fovX,aspectRatio);
        resizeGL(imageSize.width,imageSize.height);
    }

    void changePerspectiveFovY(double fovY,double aspectRatio,cv::Size imageSize){
        perspectiveSettings.change(AbstractGridDrawer::PerspectiveSettings::Configuration::FOVY,
            fovY,aspectRatio);
        resizeGL(imageSize.width,imageSize.height);
    }

    AbstractGridDrawer::PerspectiveSettings getPerspectiveSettings(){
        return perspectiveSettings;
    }

    void setNewPerspective(AbstractGridDrawer::PerspectiveSettings newPerspectiveSettings,
    cv::Size imageSize){
        clearCanvas();
        perspectiveSettings=newPerspectiveSettings;
        resizeGL(imageSize.width,imageSize.height);
    }

    void paintGL(){
        //clearCanvas();
    }

    virtual cv::Point2d project(cv::Point3d pointInWorld,cv::Point3d pcameraOrientation,
    cv::Point3d pcameraLocation){
        std::array<GLdouble, 16> projection;
        std::array<GLdouble, 16> modelview;
        std::array<GLdouble, 3>  screen_coords;
        GLint viewport[4];

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-pcameraLocation.x,-pcameraLocation.y,-pcameraLocation.z);
        glRotatef(-pcameraOrientation.z, 0.0, 0.0, 1.0);
        glRotatef(-pcameraOrientation.y, 0.0, 1.0, 0.0);
        glRotatef(-pcameraOrientation.x, 1.0, 0.0, 0.0);

        glGetDoublev(GL_PROJECTION_MATRIX, projection.data());
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview.data());
        glGetIntegerv(GL_VIEWPORT, viewport);

        gluProject(pointInWorld.x, pointInWorld.y, pointInWorld.z,
            modelview.data(), projection.data(),viewport,
            screen_coords.data(), screen_coords.data() + 1, screen_coords.data() + 2);
        return cv::Point2d(screen_coords[0],screen_coords[1]);
    }

    void prepareCamera(){
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-cameraLocation.x,-cameraLocation.y,-cameraLocation.z);
        glRotatef(-cameraOrientation.z, 0.0, 0.0, 1.0);
        glRotatef(-cameraOrientation.y, 0.0, 1.0, 0.0);
        glRotatef(-cameraOrientation.x, 1.0, 0.0, 0.0);

    }

private:
    /*class Matrix4{

    };

    void setFrustum(float fovY, float aspectRatio, float front, float back)
    {
        float tangent = tanf(fovY/2 * CV_PI/180.);   // tangent of half fovY
        float height = front * tangent;           // half height of near plane
        float width = height * aspectRatio;       // half width of near plane

        // params: left, right, bottom, top, near, far
        setFrustum(-width, width, -height, height, front, back);
    }

    void setFrustum(float l, float r, float b, float t, float n, float f)
    {
        matrixProjection.identity();
        matrixProjection[0]  =  2 * n / (r - l);
        matrixProjection[2]  =  (r + l) / (r - l);
        matrixProjection[5]  =  2 * n / (t - b);
        matrixProjection[6]  =  (t + b) / (t - b);
        matrixProjection[10] = -(f + n) / (f - n);
        matrixProjection[11] = -(2 * f * n) / (f - n);
        matrixProjection[14] = -1;
        matrixProjection[15] =  0;
    }*/
};

#endif // GRIDDRAWEROPENGLWIDGET

