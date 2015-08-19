#ifndef FOUNDHOMOGRAPHYDIALOG_H
#define FOUNDHOMOGRAPHYDIALOG_H

#include <QDialog>
#include "ui_foundhomographydialog.h"
#include "homography.h"
#include <QFileDialog>
#include <QTextStream>

namespace Ui {
class FoundHomographyDialog;
}

class FoundHomographyDialog : public QDialog
{
    Q_OBJECT
    Ui::FoundHomographyDialog *ui;
    Homography homography;
public:
    explicit FoundHomographyDialog(Homography phomography,QWidget *parent = 0):
    QDialog(parent),homography(phomography),
    ui(new Ui::FoundHomographyDialog){
        ui->setupUi(this);
        ui->homographyResult->setPlainText(homography.toString());
    }

    ~FoundHomographyDialog(){
        delete ui;
    }

private slots:
    void on_saceAsButton_clicked(){
        QString filePath=QFileDialog::getSaveFileName(this,tr("Save as ..."),".",
            tr("TXT Files (*.txt)"));
        QFile fileToWrite(filePath);
        if(!fileToWrite.open(QIODevice::WriteOnly|QIODevice::Text))
            return;
        QTextStream fileContentStream(&fileToWrite);
        fileContentStream<<homography.toString()<<endl;
        close();
    }

    void on_okButton_clicked(){
        close();
    }

private:

};

#endif // FOUNDHOMOGRAPHYDIALOG_H
