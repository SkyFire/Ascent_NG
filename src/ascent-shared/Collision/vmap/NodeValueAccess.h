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
