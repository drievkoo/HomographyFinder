#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/core/core.hpp>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "homographyfinder.h"
#include "keyshortcutbinding.h"
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>
#include "griddraweropenglwidget.h"
#include "foundhomographydialog.h"
#include "perspectivesettingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    HomographyFinder *homographyFinder;
    enum keyAction{ROTATE_X_CLKWISE,ROTATE_X_COUNTER_CLKWISE,
                  ROTATE_Y_CLKWISE,ROTATE_Y_COUNTER_CLKWISE,
                  ROTATE_Z_CLKWISE,ROTATE_Z_COUNTER_CLKWISE,
                  TRANSLATE_X_POSITIVE,TRANSLATE_X_NEGATIVE,
                  TRANSLATE_Y_POSITIVE,TRANSLATE_Y_NEGATIVE,
                  TRANSLATE_Z_POSITIVE,TRANSLATE_Z_NEGATIVE,
                  DENSITY_MORE,DENSITY_LESS,
                  SHIFT_LINES_LEFT,SHIFT_LINES_RIGHT,
                  SHIFT_LINES_UP,SHIFT_LINES_DOWN};
    KeyShortcutBinding<keyAction> keyBindings;
    cv::Mat image;
    GridDrawerOpenGLWidget graphicsRender;
    bool emitKeyHit;
public:
    explicit MainWindow(QWidget *parent = 0):QMainWindow(parent),
    ui(new Ui::MainWindow),homographyFinder(NULL){
        ui->setupUi(this);
        bindDefaultKeyShortcuts();
        connect(ui->imageLabel,SIGNAL(mouseClick()),this,SLOT(mouseClick()));
        connect(ui->actionOpen_image,SIGNAL(triggered()),this,SLOT(openImage()));
        connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
        emitKeyHit=false;
        graphicsRender.resize(1000,1000);
    }

    ~MainWindow(){
        if(homographyFinder!=NULL)
            delete homographyFinder;
        delete ui;
    }

    void keyPressEvent(QKeyEvent *event){
        QMainWindow::keyPressEvent(event);
        //emitKeyHit=true;
    }

    void keyReleaseEvent(QKeyEvent *event){
        /*if(emitKeyHit){
            emitKeyHit=false;
            keyHit(*event);
        }*/
        if(event!=NULL)
           keyHit(*event);
        QMainWindow::keyReleaseEvent(event);
    }
private slots:
    void about(){
        QMessageBox aboutDialog;
        aboutDialog.setWindowTitle(QString("Homography Finder"));
        aboutDialog.setText(QString("<h2>HomographyFinder</h2><br>"
            "Interactive application for finding homography between image plane and "
            "custom 3D plane in image.<hr>"
            "Finding the homography is based on virtual grid that is shown to the user."
            " The user has to manually place the grid so that it lays on a plane in image, "
            "thus specifying two planes: custom 3D plane like wall, floor, ... and image plane "
            "itself. Homography gives relationship between these two planes.<hr>"
            "<i>Author: Marek Drevenak</i><br>"
            "<i>Version: 1.0.0</i><br>"
            "<i>2015</i>"));
        aboutDialog.exec();
    }

    void openImage(){
        QString filePath=QFileDialog::getOpenFileName(this,tr("Open PNG File"),"",
            tr("PNG Files (*.png)"));
        try{
            image=cv::imread(filePath.toStdString(),CV_LOAD_IMAGE_COLOR);
        }catch(cv::Exception &e){
            return;
        }
        graphicsRender.clearCanvas();
        homographyFinder=new HomographyFinder(&graphicsRender,&graphicsRender);
        homographyFinder->setCameraLocationAndOrientation(cv::Point3i(0,0,20),cv::Point3d(0,0,0));
        AbstractGridDrawer::PerspectiveSettings perspectiveSettings=graphicsRender.getPerspectiveSettings();
        perspectiveSettings.changeAspectRatio(double(image.size().width)/image.size().height);
        graphicsRender.setNewPerspective(perspectiveSettings,image.size());
        //update UI
        ui->loadedImagePathEdit->setText(filePath);
        ui->gridUnitSizeEdit->setText(QString("%1").arg(homographyFinder->getUnitSizeInPixels()));
        ui->perspectiveSettingsEdit->setText(graphicsRender.getPerspectiveSettings().toString());
        updateImage();
    }

    void mouseClick(){
        ui->statusBar->showMessage("Mouse click.");
    }

    void keyHit(QKeyEvent &event){
        if(homographyFinder==NULL)
            return;
        double rotationAngle=ui->angleDegreeSpinBox->value();
        double translationStepInUnits=ui->translationStepSpinBox->value();
        keyAction action;
        try{
            action=keyBindings.getKeyAction(&event);
        }catch(std::logic_error &e){
            return;
        }
        if(action==ROTATE_X_CLKWISE){
            ui->statusBar->showMessage("ROTATE_X_CLKWISE");
            homographyFinder->rotateCameraAroundXAxis(rotationAngle);
        }else if(action==ROTATE_X_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_X_COUNTER_CLKWISE");
            homographyFinder->rotateCameraAroundXAxis(-rotationAngle);
        }else if(action==ROTATE_Z_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Z_CLKWISE");
            homographyFinder->rotateCameraAroundZAxis(rotationAngle);
        }else if(action==ROTATE_Z_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Z_COUNTER_CLKWISE");
            homographyFinder->rotateCameraAroundZAxis(-rotationAngle);
        }else if(action==ROTATE_Y_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Y_CLKWISE");
            homographyFinder->rotateCameraAroundYAxis(rotationAngle);
        }else if(action==ROTATE_Y_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Y_COUNTER_CLKWISE");
            homographyFinder->rotateCameraAroundYAxis(-rotationAngle);
        }else if(action==TRANSLATE_X_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_X_POSITIVE");
            homographyFinder->translateCamera(cv::Point3f(translationStepInUnits,0,0));
        }else if(action==TRANSLATE_X_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_X_NEGATIVE");
            homographyFinder->translateCamera(cv::Point3f(-translationStepInUnits,0,0));
        }else if(action==TRANSLATE_Y_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_Y_POSITIVE");
            homographyFinder->translateCamera(cv::Point3f(0,translationStepInUnits,0));
        }else if(action==TRANSLATE_Y_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_Y_NEGATIVE");
            homographyFinder->translateCamera(cv::Point3f(0,-translationStepInUnits,0));
        }else if(action==TRANSLATE_Z_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_Z_POSITIVE");
            homographyFinder->translateCamera(cv::Point3f(0,0,translationStepInUnits));
        }else if(action==TRANSLATE_Z_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_Z_NEGATIVE");
            homographyFinder->translateCamera(cv::Point3f(0,0,-translationStepInUnits));
        }else if(action==DENSITY_MORE){
            ui->statusBar->showMessage("DENSITY_MORE");
            homographyFinder->moreDenseGrid();
        }else if(action==DENSITY_LESS){
            ui->statusBar->showMessage("DENSITY_LESS");
            homographyFinder->lessDenseGrid();
        }
        updateImage();
    }
    void on_getHomographyButton_clicked(){
        if(homographyFinder==NULL)
            return;
        /*QMessageBox aboutDialog;
        aboutDialog.setWindowTitle(QString("Homography"));
        aboutDialog.setText(QString("<h2>Homography</h2><br>"+
            homographyFinder->getCurrentHomographyMatrix().toString()));
        aboutDialog.exec();*/
        FoundHomographyDialog homographyResult(homographyFinder->getCurrentHomographyMatrix());
        homographyResult.setModal(true);
        homographyResult.exec();
    }

    void on_perspectiveSettingsButton_clicked(){
        if(homographyFinder==NULL)
            return;
        double aspectRatio=(double)(image.size().width)/(double)(image.size().height);
        PerspectiveSettingsDialog perspectiveSettings(graphicsRender.getPerspectiveSettings(),
            aspectRatio);
        perspectiveSettings.setModal(true);
        perspectiveSettings.exec();
        if(perspectiveSettings.operationCanceled())
            return;
        graphicsRender.setNewPerspective(perspectiveSettings.getPerspectiveSettings(),image.size());
        ui->perspectiveSettingsEdit->setText(graphicsRender.getPerspectiveSettings().toString());
    }

private:
    void bindDefaultKeyShortcuts(){
        keyBindings.add(ROTATE_X_CLKWISE,"q");
        keyBindings.add(ROTATE_X_COUNTER_CLKWISE,"a");
        keyBindings.add(ROTATE_Y_CLKWISE,"w");
        keyBindings.add(ROTATE_Y_COUNTER_CLKWISE,"s");
        keyBindings.add(ROTATE_Z_CLKWISE,"e");
        keyBindings.add(ROTATE_Z_COUNTER_CLKWISE,"d");

        keyBindings.add(TRANSLATE_X_POSITIVE,"f");
        keyBindings.add(TRANSLATE_X_NEGATIVE,"h");
        keyBindings.add(TRANSLATE_Y_POSITIVE,"g");
        keyBindings.add(TRANSLATE_Y_NEGATIVE,"t");
        keyBindings.add(TRANSLATE_Z_POSITIVE,"5");
        keyBindings.add(TRANSLATE_Z_NEGATIVE,"b");

        keyBindings.add(DENSITY_MORE,"m");
        keyBindings.add(DENSITY_LESS,"l");
    }

    void updateImage(){
        cv::Mat imageWithGrid=image.clone();
        imageWithGrid=homographyFinder->drawVirtualGridOn(imageWithGrid);
        float fx=ui->imageLabel->frameSize().width()/float(imageWithGrid.cols);
        float fy=ui->imageLabel->frameSize().height()/float(imageWithGrid.rows);
        float f=cv::min(fx,fy);
        cv::resize(imageWithGrid,imageWithGrid,cv::Size(),f,f);
        ui->imageLabel->setPixmap(makeQPixmap(imageWithGrid));

        //std::vector<cv::Point3d> gridCorners3D=homographyFinder->getGrid().getGridCornerPoints();
        //cv::Point2d origin=graphicsRender.project(gridCorners3D[0],homographyFinder->getCameraOrientation(),
        //    homographyFinder->getCameraLocation());
        //imageWithGrid=image.clone();
        //cv::circle(imageWithGrid,cv::Point2f(origin.x,image.size().height-origin.y),5,cv::Scalar(0,0,255));
        //cv::imshow("found",imageWithGrid);
    }

    QPixmap makeQPixmap(cv::Mat frame){
        cv::Mat cvtFrame;
        cv::cvtColor(frame,cvtFrame,CV_BGR2RGB);
        frame=cvtFrame;
        QImage imgFrame((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        return QPixmap::fromImage(imgFrame);
    }
};

#endif // MAINWINDOW_H
