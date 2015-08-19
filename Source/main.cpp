#include "mainwindow.h"
#include <QApplication>
#include <iostream>

//#define RUN_UNIT_TESTS

#ifdef RUN_UNIT_TESTS
    #include "abstractgriddrawerunittest.h"
#endif

#ifndef  ABSTRACTGRIDDRAWER_H_CONSTANTS
#define ABSTRACTGRIDDRAWER_H_CONSTANTS
const cv::Point3d AbstractGridDrawer::defaultCameraLocation=cv::Point3d(0,0,10);
const cv::Point3d AbstractGridDrawer::defaultCameraOrientation=cv::Point3d(0,0,1);
#endif

int main(int argc, char *argv[])
{
    #ifdef RUN_UNIT_TESTS
        try{
            AbstractGridDrawerUnitTest gridDrawerTester;
            std::cout<<"Launching tests ..."<<std::endl;
            bool testsOK=true;
            bool gridTest=gridDrawerTester.test();
            std::cout<<"AbstractGridDrawerUnitTest "<<(gridTest?" OK ":"Fail")<<std::endl;
            testsOK=testsOK && gridTest;
            std::cout<<"---------------------------------"<<std::endl;
            std::cout<<"------Tests             "<<(testsOK?" OK ":"Fail")<<"-----"<<std::endl;
            std::cout<<"---------------------------------"<<std::endl;
        }catch(std::exception &e){
            std::cerr<<"Runtime exception: "<<e.what()<<std::endl;
        }catch(...){
            std::cerr<<"Runtime exception! "<<std::endl;
        }
        return 0;
    #else
        QApplication a(argc, argv);
        int exitCode=0;

        MainWindow w;
        try{
            w.show();
            exitCode=a.exec();
        }catch(std::exception &e){
            std::cerr<<"Runtime exception: "<<e.what()<<std::endl;
        }catch(...){
            std::cerr<<"Runtime exception! "<<std::endl;
        }
        return exitCode;
    #endif
}
