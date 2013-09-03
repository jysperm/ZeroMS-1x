#include "OClient.h"

OClient::OClient():isLoged(0),clientver(0),conn(0)
{
    databuf=new QByteArray;
    ping();
}

OClient::~OClient()
{
    if(conn)
    {
        conn->abort();
        conn->disconnect();
        conn->deleteLater();
    }
    delete databuf;
}

