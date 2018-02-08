#ifndef MK_SHOP_ITEM_H
#define MK_SHOP_ITEM_H

// Include MK
#include "../Common/MKMacros.h"

NS_MK_BEGIN

class MKShopItem
{
public:
    const mkString m_Name;
    const mkU32 m_Cost;

    MKShopItem(const mkString& _name, mkU32 _cost)
        : m_Name(_name), m_Cost(_cost)
    {}
    virtual ~MKShopItem() {}
};

NS_MK_END

#endif // MK_SHOP_ITEM_H