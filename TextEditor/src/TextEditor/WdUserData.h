
#ifndef __WD_USER_DATA_H__
#define __WD_USER_DATA_H__

#include <QMap>
#include <QString>
#include <QVariant>
#include <QTextBlockUserData>

class WdUserData : public QTextBlockUserData
{
public:
    WdUserData(QString const &prop, QVariant const &value = QVariant()) 
        : QTextBlockUserData(), properties()
    {
        properties.insert(prop, value);
    }

    virtual ~WdUserData() {}

    QMap<QString, QVariant> properties;
};

#endif
