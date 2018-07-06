#pragma once

#include "embeddedwindow.h"

//this class is the publicly facable invocation of SurfaceItem
//it is responsible for launching a plasmoid over our DBus interface and handling relevant
//plasmoid specific things

class EmbeddedPlasmoidLoader: public SurfaceItem
{
    Q_OBJECT
public:
    EmbeddedPlasmoidLoader(QQuickItem *parent = nullptr);

private:
};
