#ifndef PERSPECTIVESETTINGSDIALOG_H
#define PERSPECTIVESETTINGSDIALOG_H

#include <QDialog>
#include "ui_perspectivesettingsdialog.h"
#include "abstractgriddrawer.h"

namespace Ui {
class PerspectiveSettingsDialog;
}

class PerspectiveSettingsDialog : public QDialog
{
    Q_OBJECT
    Ui::PerspectiveSettingsDialog *ui;
    AbstractGridDrawer::PerspectiveSettings perspectiveSettings;
    bool operationOK;
    double imageAspectRatio;
public:
    explicit PerspectiveSettingsDialog(AbstractGridDrawer::PerspectiveSettings currentSettings,double
    pImageAspectRatio,QWidget *parent = 0):QDialog(parent), ui(new Ui::PerspectiveSettingsDialog){
        ui->setupUi(this);
        perspectiveSettings=currentSettings;
        imageAspectRatio=pImageAspectRatio;
        operationOK=false;
        ui->aspectRatioEdit->setText(QString::number(perspectiveSettings.getAspectRatio(),'f',3));
        ui->fovXEdit->setText(QString::number(perspectiveSettings.getFovX(),'f',3));
        ui->fovYEdit->setText(QString::number(perspectiveSettings.getFovY(),'f',3));
    }

    ~PerspectiveSettingsDialog(){
        delete ui;
    }

    AbstractGridDrawer::PerspectiveSettings getPerspectiveSettings(){
        return perspectiveSettings;
    }

    bool operationCanceled(){
        return !operationOK;
    }

private slots:
    void on_OKButton_clicked(){
        bool ok;
        double fovX=ui->fovXEdit->text().toDouble(&ok);
        if(!ok)
            return;
        double fovY=ui->fovYEdit->text().toDouble(&ok);
        if(!ok)
            return;
        operationOK=true;
        double aspectRatio=fovX/fovY;
        perspectiveSettings.change(AbstractGridDrawer::PerspectiveSettings::Configuration::FOVX,
            fovX,aspectRatio);
        close();
    }

    void on_cancelButton_clicked(){
        close();
    }

    void on_calculateFOVButton_clicked(){
        bool ok;
        double aspectRatio=ui->aspectRatioEdit->text().toDouble(&ok);
        if(!ok)
            return;
        if(ui->fovYEdit->text().isEmpty()&&!ui->fovXEdit->text().isEmpty()){
            double fov=ui->fovXEdit->text().toDouble(&ok);
            if(ok)
                ui->fovYEdit->setText(QString::number(fov/aspectRatio,'f',3));
        }else if(ui->fovXEdit->text().isEmpty()&&!ui->fovYEdit->text().isEmpty()){
            double fov=ui->fovYEdit->text().toDouble(&ok);
            if(ok)
                ui->fovXEdit->setText(QString::number(fov*aspectRatio,'f',3));
        }
    }

    void on_useImageAspectRatioButton_clicked(){
        ui->aspectRatioEdit->setText(QString::number(imageAspectRatio,'f',3));
    }

private:
};

#endif // PERSPECTIVESETTINGSDIALOG_H
