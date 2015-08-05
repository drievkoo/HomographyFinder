#ifndef CONTROLLABLELABEL
#define CONTROLLABLELABEL

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>

class ControllableLabel : public QLabel
{
    Q_OBJECT
    QMouseEvent *mouseEvent;
    bool emitMouseClick;
    QKeyEvent *keyEvent;
    bool emitKeyHit;
public:
    explicit ControllableLabel(QWidget *parent =0):QLabel(parent),mouseEvent(0),keyEvent(0){
        setMouseTracking(true);
        emitMouseClick=false;
        emitKeyHit=false;
    }

    void mousePressEvent(QMouseEvent *event){
        mouseEvent=event;
        emitMouseClick=true;
        emit mousePressed();
    }

    void mouseMoveEvent(QMouseEvent *event){
        mouseEvent=event;
        if(emitMouseClick)
            emitMouseClick=false;
        emit mouseMove();
    }

    void mouseReleaseEvent(QMouseEvent *event){
        mouseEvent=event;
        emit mouseRelease();
        if(emitMouseClick){
            emitMouseClick=false;
            emit mouseClick();
        }
    }

    void keyPressEvent(QKeyEvent *event){
        keyEvent=event;
        emitKeyHit=true;
        emit keyPressed();
    }

    void keyReleaseEvent(QKeyEvent *event){
        keyEvent=event;
        emit keyReleased();
        if(emitKeyHit){
            emitKeyHit=false;
            emit keyHit();
        }
    }

    QMouseEvent *getMouseEvent(){
        return mouseEvent;
    }

    QKeyEvent *getKeyEvent(){
        return keyEvent;
    }
signals:
    void mousePressed();
    void mouseMove();
    void mouseRelease();
    void mouseClick();

    void keyPressed();
    void keyReleased();
    void keyHit();
};

#endif // CONTROLLABLELABEL

