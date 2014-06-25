
#ifndef __AREA_H__
#define __AREA_H__

#include <QWidget>

#include <QSplitter>

#include "Area/SideArea.h"

class Area : public QWidget {
public:
    enum SidePosition { LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE };

    explicit Area(QWidget *areaWidget, QWidget *parent = nullptr);
    virtual ~Area() {}

    SideArea *getSideArea(const SidePosition position) const {
        return _sideArea[position];
    }

private:
    SideArea *_sideArea[3];
    QWidget *_areaWidget;

    QSplitter *_splitterVertical;
    QSplitter *_splitterHorizontal;
    QWidget *_containerMidHori;
};

#endif
