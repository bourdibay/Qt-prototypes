
#include <QHBoxLayout>

#include "Area/Area.h"
#include "Area/SideAreaTabs.h"

Area::Area(QWidget *areaWidget, QWidget *parent)
: QWidget(parent)
,
    // TODO: initializer list with VS2013
    //    _sideArea(),
    _areaWidget(areaWidget)
, _splitterVertical(new QSplitter())
, _splitterHorizontal(new QSplitter())
, _containerMidHori(new QWidget()) {
    Q_ASSERT(areaWidget != nullptr);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    layout->addWidget(_splitterHorizontal);

    _splitterHorizontal->setOrientation(Qt::Orientation::Horizontal);
    _splitterVertical->setOrientation(Qt::Orientation::Vertical);

    _sideArea[LEFT_SIDE] = new SideAreaTabs(this);
    _sideArea[RIGHT_SIDE] = new SideAreaTabs(this);
    _sideArea[BOTTOM_SIDE] = new SideArea(this);

    _splitterHorizontal->addWidget(_sideArea[LEFT_SIDE]);
    _splitterHorizontal->addWidget(_splitterVertical);
    _splitterHorizontal->addWidget(_sideArea[RIGHT_SIDE]);

    _splitterVertical->addWidget(_areaWidget);
    _splitterVertical->addWidget(_sideArea[BOTTOM_SIDE]);
}
