/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
