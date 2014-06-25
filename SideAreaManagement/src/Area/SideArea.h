
#ifndef __SIDE_AREA_H__
#define __SIDE_AREA_H__

#include <algorithm>
#include <vector>
#include <QWidget>
#include <QStackedWidget>

#include "Area/SideAreaComponent.h"

class SideArea : public QWidget {
public:
    SideArea(QWidget *parent = nullptr);
    virtual ~SideArea() {}

    virtual bool addComponent(SideAreaComponent *component);
    virtual void insertComponent(SideAreaComponent *before,
                                 SideAreaComponent *component);
    virtual bool takeComponent(SideAreaComponent *component);

    virtual SideAreaComponent *getCurrentComponent() const {
        QWidget *currentWidget = _stackComponents->currentWidget();
        return currentWidget ? dynamic_cast<SideAreaComponent *>(currentWidget)
                             : nullptr;
    }
    virtual void setCurrentComponent(SideAreaComponent *component);

    SideAreaComponent *getComponent(QString const &key) const;
    int getIndex(SideAreaComponent *component) const {
        return _components.indexOf(component);
    }
    SideAreaComponent *getComponent(const int index) const {
        return (index >= 0 && index < _components.length())
                   ? _components.at(index)
                   : nullptr;
    }
    int getNumberComponents() const { return _components.length(); }

protected:
    QStackedWidget *_stackComponents;
    QVector<SideAreaComponent *> _components;
};

#endif
