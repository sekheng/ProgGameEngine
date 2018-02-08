#ifndef MKBACKGROUND_ITEM_H
#define MKBACKGROUND_ITEM_H

// Include MK
#include "../Common/MKMacros.h"
#include "MKShopItem.h"

NS_MK_BEGIN

class MKShopItem_Background : public MKShopItem
{
public:
    enum BackgroundLayers
    {
        STATIC = 0,
        REAR,
        MIDDLE,
        FRONT,

        NUM_BACKGROUND_LAYERS,
    };

protected:
    mkString m_BackgroundFiles[NUM_BACKGROUND_LAYERS];

public:
    // Constructor(s) & Destructor
    MKShopItem_Background(const mkString& _name, mkU32 _cost)
        : MKShopItem(_name, _cost)
    {}
    virtual ~MKShopItem_Background() {}

    void SetBackgroundFile(BackgroundLayers _layer, const mkString& _filePath) { m_BackgroundFiles[_layer] = _filePath; }
    const mkString& GetBackgroundFile(BackgroundLayers _layer) { return m_BackgroundFiles[_layer]; }
};

NS_MK_END

#endif // MKBACKGROUND_ITEM_H