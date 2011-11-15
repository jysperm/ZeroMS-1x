#ifndef OCLIENT_H
#define OCLIENT_H

#include "OCore_global.h"
#include "ocommand.h"

// ZeroMS Namespace {
OMS_NAMESPACE_BEGIN

class OCORESHARED_EXPORT Client
{
public:
    Client();

    bool connect(/* Params */);
    bool disconnect(/* Params */);
    bool send(Command m/*, Params */);
};

OMS_NAMESPACE_END
// ZeroMS Namesapce }

#endif // OCLIENT_H
