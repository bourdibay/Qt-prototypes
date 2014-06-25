
#ifndef __SIDE_AREA_TABS_H__
#define __SIDE_AREA_TABS_H__

#include <QTabBar>

#include "Area/SideArea.h"

// TODO: if we enable move tabs, we have to update index via insertComponent
class SideAreaTabs : public SideArea {
public:
    SideAreaTabs(QWidget *parent = nullptr);
    virtual ~SideAreaTabs() {}

    virtual bool addComponent(SideAreaComponent *component);
    virtual void insertComponent(SideAreaComponent *before,
                                 SideAreaComponent *component) override;
    virtual bool takeComponent(SideAreaComponent *component) override;
    virtual void setCurrentComponent(SideAreaComponent *component) override;

private:
    QTabBar *_tabbar;
};

#endif
