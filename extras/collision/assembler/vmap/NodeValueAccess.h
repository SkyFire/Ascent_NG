/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _NODEVALUEACCESS_H
#define _NODEVALUEACCESS_H

namespace VMAP
{
    /**
    This is a helper Class to get access to SubModels or triangles when analyzing the BSP-Tree.
    */

    template<class TNode, class TValue> class NodeValueAccess
    {
        private:
            TNode const* iNodeArray;
            TValue const* iValueArray;

        public:
            inline NodeValueAccess() : iNodeArray(NULL), iValueArray(NULL) {}

            inline NodeValueAccess(TNode const* pNodeArray, TValue const* pValueArray) : iNodeArray(pNodeArray), iValueArray(pValueArray) {}
            inline TNode const* getNodePtr() const { return(iNodeArray); }
            inline TValue const* getValuePtr() const { return(iValueArray); }

            inline TNode const& getNode(unsigned int pPos) const { return(iNodeArray[pPos]); }
            inline void setNode(const TNode& pNode, unsigned int pPos) { iNodeArray[pPos] = pNode; }

            inline TValue const& getValue(unsigned int pPos) const { return(iValueArray[pPos]); }
            inline void setValue(const TValue& pValue, unsigned int pPos) { iValueArray[pPos] = pValue; }
    };
}
#endif
