/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef _MANAGEDMODELCONTAINER_H
#define _MANAGEDMODELCONTAINER_H

#include "ModelContainer.h"

//=======================================================
/**
This is a ModelContainer with reference count information.
*/

namespace VMAP
{
    //=======================================================

    class ManagedModelContainer :
    public ModelContainer
    {
        private:
            int refCount;
        public:
            ManagedModelContainer(void) ;
            ~ManagedModelContainer(void);

            void incRefCount() { ++refCount; }
            void decRefCount() { --refCount; if(refCount < 0) refCount = 0; }
            int getRefCount() { return refCount; }
    };

    //=======================================================
}
#endif
