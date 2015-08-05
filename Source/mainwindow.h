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
        homographyFinder=new HomographyFinder(image.size());
        //update UI
        ui->loadedImagePathEdit->setText(filePath);
        ui->gridUnitSizeEdit->setText(QString("%1").arg(homographyFinder->getUnitSizeInPixels()));
        updateImage();
    }

    void mouseClick(){
        ui->statusBar->showMessage("Mouse click.");
    }

    void keyHit(QKeyEvent &event){
        if(homographyFinder==NULL)
            return;
        double rotationAngleInRadians=(ui->angleDegreeSpinBox->value()*CV_PI)/180.;
        double translationStepInUnits=ui->translationStepSpinBox->value();
        double linesShiftStep=ui->gridLinesShiftStepSpinBox->value();
        keyAction action;
        try{
            action=keyBindings.getKeyAction(&event);
        }catch(std::logic_error &e){
            return;
        }
        if(action==ROTATE_X_CLKWISE){
            ui->statusBar->showMessage("ROTATE_X_CLKWISE");
            homographyFinder->rotateGridAroundZAxis(rotationAngleInRadians);
        }else if(action==ROTATE_X_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_X_COUNTER_CLKWISE");
            homographyFinder->rotateGridAroundZAxis(-rotationAngleInRadians);
        }else if(action==ROTATE_Z_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Z_CLKWISE");
            homographyFinder->rotateGridAroundXAxis(rotationAngleInRadians);
        }else if(action==ROTATE_Z_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Z_COUNTER_CLKWISE");
            homographyFinder->rotateGridAroundXAxis(-rotationAngleInRadians);
        }else if(action==ROTATE_Y_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Y_CLKWISE");
            homographyFinder->rotateGridAroundYAxis(rotationAngleInRadians);
        }else if(action==ROTATE_Y_COUNTER_CLKWISE){
            ui->statusBar->showMessage("ROTATE_Y_COUNTER_CLKWISE");
            homographyFinder->rotateGridAroundYAxis(-rotationAngleInRadians);
        }else if(action==TRANSLATE_X_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_X_POSITIVE");
            homographyFinder->translate(cv::Point3f(translationStepInUnits,0,0));
        }else if(action==TRANSLATE_X_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_X_NEGATIVE");
            homographyFinder->translate(cv::Point3f(-translationStepInUnits,0,0));
        }else if(action==TRANSLATE_Y_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_Y_POSITIVE");
            homographyFinder->translate(cv::Point3f(0,translationStepInUnits,0));
        }else if(action==TRANSLATE_Y_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_Y_NEGATIVE");
            homographyFinder->translate(cv::Point3f(0,-translationStepInUnits,0));
        }else if(action==TRANSLATE_Z_POSITIVE){
            ui->statusBar->showMessage("TRANSLATE_Z_POSITIVE");
            homographyFinder->translate(cv::Point3f(0,0,translationStepInUnits));
        }else if(action==TRANSLATE_Z_NEGATIVE){
            ui->statusBar->showMessage("TRANSLATE_Z_NEGATIVE");
            homographyFinder->translate(cv::Point3f(0,0,-translationStepInUnits));
        }else if(action==DENSITY_MORE){
            ui->statusBar->showMessage("DENSITY_MORE");
            homographyFinder->moreDenseGrid();
        }else if(action==DENSITY_LESS){
            ui->statusBar->showMessage("DENSITY_LESS");
            homographyFinder->lessDenseGrid();
        }else if(action==SHIFT_LINES_LEFT){
            ui->statusBar->showMessage("SHIFT_LINES_LEFT");
            homographyFinder->shiftGridLeft(linesShiftStep);
        }else if(action==SHIFT_LINES_RIGHT){
            ui->statusBar->showMessage("SHIFT_LINES_RIGHT");
            homographyFinder->shiftGridRight(linesShiftStep);
        }else if(action==SHIFT_LINES_UP){
            ui->statusBar->showMessage("SHIFT_LINES_UP");
            homographyFinder->shiftGridUp(linesShiftStep);
        }else if(action==SHIFT_LINES_DOWN){
            ui->statusBar->showMessage("SHIFT_LINES_DOWN");
            homographyFinder->shiftGridDown(linesShiftStep);
        }
        updateImage();
    }
    void on_getHomographyButton_clicked(){

    }

private:
    void bindDefaultKeyShortcuts(){
        keyBindings.add(ROTATE_X_CLKWISE,"a");
        keyBindings.add(ROTATE_X_COUNTER_CLKWISE,"q");
        keyBindings.add(ROTATE_Y_CLKWISE,"s");
        keyBindings.add(ROTATE_Y_COUNTER_CLKWISE,"w");
        keyBindings.add(ROTATE_Z_CLKWISE,"d");
        keyBindings.add(ROTATE_Z_COUNTER_CLKWISE,"e");

        keyBindings.add(TRANSLATE_X_POSITIVE,"u");
        keyBindings.add(TRANSLATE_X_NEGATIVE,"j");
        keyBindings.add(TRANSLATE_Y_POSITIVE,"i");
        keyBindings.add(TRANSLATE_Y_NEGATIVE,"k");
        keyBindings.add(TRANSLATE_Z_POSITIVE,"o");
        keyBindings.add(TRANSLATE_Z_NEGATIVE,"l");

        keyBindings.add(DENSITY_MORE,"m");
        keyBindings.add(DENSITY_LESS,"n");

        keyBindings.add(SHIFT_LINES_LEFT,"f");
        keyBindings.add(SHIFT_LINES_RIGHT,"h");
        keyBindings.add(SHIFT_LINES_UP,"t");
        keyBindings.add(SHIFT_LINES_DOWN,"g");
    }

    void updateImage(){
        cv::Mat imageWithGrid=image.clone();
        homographyFinder->drawVirtualGridOn(imageWithGrid);
        float fx=ui->imageLabel->frameSize().width()/float(imageWithGrid.cols);
        float fy=ui->imageLabel->frameSize().height()/float(imageWithGrid.rows);
        float f=cv::min(fx,fy);
        cv::resize(imageWithGrid,imageWithGrid,cv::Size(),f,f);
        ui->imageLabel->setPixmap(makeQPixmap(imageWithGrid));
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
