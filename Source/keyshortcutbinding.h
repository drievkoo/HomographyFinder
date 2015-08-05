#ifndef KEYSHORTCUTBINDING
#define KEYSHORTCUTBINDING

#include <QKeyEvent>
#include <QChar>
#include <map>

template<class ENUM>
class KeyShortcutBinding{
    std::map<QChar,ENUM> bindings;
public:
    KeyShortcutBinding(){

    }

    ~KeyShortcutBinding(){

    }

    void add(ENUM action,QString key){
        bindings[key.trimmed().at(0)]=action;
    }

    void add(ENUM action,Qt::KeyboardModifier modifiers,int key){
        throw std::exception("Not implemented yet.");
    }

    ENUM getKeyAction(QKeyEvent *event){
        if(event==NULL)
            throw std::invalid_argument("No key event to find bind action.");
        QChar key=event->text().at(0);
        if(bindings.find(key)==bindings.end())
            throw std::out_of_range("Key not found.");
        return bindings[key];
    }

private:
};

#endif // KEYSHORTCUTBINDING

