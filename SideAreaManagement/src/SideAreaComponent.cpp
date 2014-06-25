
#include <QLabel>

#include "Area/SideAreaComponent.h"

SideAreaComponent::SideAreaComponent(QString const &key, QString const &name,
                                     QWidget *parent)
: QWidget(parent), _layout(new QGridLayout), _key(key), _name(name), _icon() {
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);

    // TEST
    QLabel *label = new QLabel("hej!");
    _layout->addWidget(label);
}

SideAreaComponent::SideAreaComponent(QString const &key, QString const &name,
                                     QIcon const &icon, QWidget *parent)
: QWidget(parent)
, _layout(new QGridLayout)
, _key(key)
, _name(name)
, _icon(icon) {
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);

    // TEST
    QLabel *label = new QLabel("hej!");
    _layout->addWidget(label);
}

SideAreaComponent::SideAreaComponent(SideAreaComponent const &copy)
: QWidget()
, _layout(new QGridLayout)
, _key(copy.getKey())
, _name(copy.getName())
, _icon(copy.getIcon()) {
    _layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_layout);
}
