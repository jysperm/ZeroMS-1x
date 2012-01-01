#ifndef OROBOT_H
#define OROBOT_H

#include "oclientcore.h"

class ORobot : public OClientCore
{
    Q_OBJECT
public:
    explicit ORobot();
    void init();
    void msgLoginOk(OPacket &packet);
public slots:
    void MSG(QString user,QString view,QString msg);
};

#endif // OROBOT_H
