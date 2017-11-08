#pragma once

#include "cocos2d.h"
#include "MK/Common/MKSingletonTemplate.h"

namespace SakataGintoki {
    class SimperMusicSys : public MinamiKotori::MKSingletonTemplate<SimperMusicSys>
    {
    public:


    protected:
        SimperMusicSys();
        virtual ~SimperMusicSys();
    };
}
