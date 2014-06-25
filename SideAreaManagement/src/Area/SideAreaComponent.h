
#ifndef __SIDE_AREA_COMPONENT_H__
#define __SIDE_AREA_COMPONENT_H__

#include <QWidget>
#include <QGridLayout>
#include <QIcon>
#include <QString>

class SideAreaComponent : public QWidget {
public:
    SideAreaComponent(QString const &key, QString const &name,
                      QWidget *parent = nullptr);
    SideAreaComponent(QString const &key, QString const &name,
                      QIcon const &icon, QWidget *parent = nullptr);
    SideAreaComponent(SideAreaComponent const &copy);

    virtual ~SideAreaComponent() {}

    QString const &getName() const { return _name; }
    QIcon const &getIcon() const { return _icon; }
    QString const &getKey() const { return _key; }

protected:
    QGridLayout *_layout;

private:
    const QString _key;
    const QString _name;
    const QIcon _icon;

    SideAreaComponent &operator=(SideAreaComponent const &copy);
};

#endif
