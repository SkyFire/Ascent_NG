/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef _SP_GARBAGE_COLLECTOR_H
#define _SP_GARBAGE_COLLECTOR_H

//Reference based smart pointers
//Save while using hahsmaps etc for reference increasing.
//shared pointers will auto delete themselves but even with a purposed delete the pointer will be safe ;P

template <class T> class SharedPointer;
template <class T>
class PointerReference
{
        friend class SharedPointer<T>;
private:
        PointerReference(T* pT) : Count(0), my_pT(pT) { ASSERT(pT != 0); }
        ~PointerReference()         { ASSERT(Count == 0); delete my_pT; }

        unsigned GetRef()  { return ++Count; }
        unsigned FreeRef() {  ASSERT(Count!=0); return --Count; }

        T* const my_pT;
        unsigned Count;
};

template <class T>
class SharedPointer
{
public:
        SharedPointer()
        {
            m_Reference = 0;

        }
        SharedPointer(T* pT)
        {
            if(!pT)  { m_Reference = 0; return; }

            m_Reference = new PointerReference<T>(pT);
            m_Reference->GetRef();

        }

        SharedPointer(const SharedPointer<T>& rVar)
        {
                m_Reference = rVar.m_Reference;
                if (!Null())
                        m_Reference->GetRef();
        }

        ~SharedPointer()
        {
                UnBind();
        }

        T& operator*()
        {
            if (Null())
                    return NULL;
                      //  throw NulRefException();
                return *m_Reference->my_pT;
        }
        T* operator->()
        {
                if (Null())
                    return NULL;
                      //  throw NulRefException();
                return m_Reference->my_pT;
        }
        const T* operator->() const
        {
                if (Null())
                    return NULL;
                        //throw NulRefException();
                return m_Reference->my_pT;
        }

        SharedPointer<T>& operator=(const SharedPointer<T>& rVar)
        {
                if (!rVar.Null())
                        rVar.m_Reference->GetRef();
                UnBind();
                m_Reference = rVar.m_Reference;
                return *this;
        }

        bool operator==(const SharedPointer<T>& lhs)
        {
          
            return this->m_Reference == lhs.m_Reference;
        
        }
        
        bool operator!=(const SharedPointer<T>& lhs)
        {
                return !(*this == lhs);
        }

        bool Null() const {return m_Reference == 0; }
        void SetNull() { UnBind(); }

private:
        void UnBind()
        {
                if (!Null() && m_Reference->FreeRef() == 0)       
                        delete m_Reference;
                m_Reference = 0;
        }
        PointerReference<T>* m_Reference;
};

#endif
