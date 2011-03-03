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

#ifndef _CALLBACK_H
#define _CALLBACK_H


class CallbackBase {
public:
	virtual void execute() {};
	virtual ~CallbackBase() {};
};

class CallbackFP {
	typedef void(*callback)();
	callback mycallback;
public:
	CallbackFP(callback cb) : mycallback(cb) {}
	void operator()();
	void execute();
	void set(callback cb) { mycallback = cb; }
	CallbackFP* Create();
};

template < class Class >
class CallbackP0 : public CallbackBase
{
public:

	typedef void (Class::*Method)();
	CallbackP0(Class* _class_instance, Method _method)
	{
	   _obj = _class_instance;
	   _func = _method;
	};
	void operator()() { return (_obj->*_func)(); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
};

template < class Class, typename P1 >
class CallbackP1 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1);
	CallbackP1(Class* _class_instance, Method _method, P1 p1)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	};

	void operator()() { return (_obj->*_func)(_p1); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
};

template < class Class, typename P1, typename P2 >
class CallbackP2 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2);
	CallbackP2(Class* _class_instance, Method _method, P1 p1, P2 p2)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
};

template < class Class, typename P1, typename P2, typename P3 >
class CallbackP3 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2, P3);
	CallbackP3(Class* _class_instance, Method _method, P1 p1, P2 p2, P3 p3)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	   _p3 = p3;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2, _p3); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
	P3 _p3;
};

template < class Class, typename P1, typename P2, typename P3, typename P4 >
class CallbackP4 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2, P3, P4);
	CallbackP4(Class* _class_instance, Method _method, P1 p1, P2 p2, P3 p3, P4 p4)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	   _p3 = p3;
	   _p4 = p4;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2, _p3, _p4); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
	P3 _p3;
	P4 _p4;
};

template < class Class, typename P1, typename P2, typename P3, typename P4, typename P5 >
class CallbackP5 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2, P3, P4, P5);
	CallbackP5(Class* _class_instance, Method _method, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	   _p3 = p3;
	   _p4 = p4;
	   _p5 = p5;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2, _p3, _p4, _p5); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
	P3 _p3;
	P4 _p4;
	P5 _p5;
};

class QueryResult;
struct AsyncQueryResult;
#include <vector>
typedef std::vector<AsyncQueryResult> QueryResultVector;

class SQLCallbackBase
{
public:
	virtual ~SQLCallbackBase();
	virtual void run(QueryResultVector & result) = 0;
};

template<class T>
class SQLClassCallbackP0 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p);
	T * base;
	SCMethod method;
public:
	SQLClassCallbackP0(T* instance, SCMethod imethod) : SQLCallbackBase(), base(instance), method(imethod) {}
	~SQLClassCallbackP0() {}
	void run(QueryResultVector & data) { (base->*method)(data); }
};

template<class T, typename P1>
class SQLClassCallbackP1 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1);
	T * base;
	SCMethod method;
	P1 par1;
public:
	SQLClassCallbackP1(T* instance, SCMethod imethod, P1 p1) : SQLCallbackBase(), base(instance), method(imethod), par1(p1) {}
	~SQLClassCallbackP1() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1); }
};

template<class T, typename P1, typename P2>
class SQLClassCallbackP2 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
public:
	SQLClassCallbackP2(T* instance, SCMethod imethod, P1 p1, P2 p2) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2) {}
	~SQLClassCallbackP2() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2); }
};

template<class T, typename P1, typename P2, typename P3>
class SQLClassCallbackP3 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2, P3 p3);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
	P3 par3;
public:
	SQLClassCallbackP3(T* instance, SCMethod imethod, P1 p1, P2 p2, P3 p3) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2), par3(p3) {}
	~SQLClassCallbackP3();
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2, par3); }
};

template<class T, typename P1, typename P2, typename P3, typename P4>
class SQLClassCallbackP4 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2, P3 p3, P4 p4);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
	P3 par3;
	P4 par4;
public:
	SQLClassCallbackP4(T* instance, SCMethod imethod, P1 p1, P2 p2, P3 p3, P4 p4) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2), par3(p3), par4(p4) {}
	~SQLClassCallbackP4() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2, par3, par4); }
};

class SQLFunctionCallbackP0 : public SQLCallbackBase
{
	typedef void(*SCMethod)(QueryResult*);
	SCMethod method;
public:
	SQLFunctionCallbackP0(SCMethod m) : SQLCallbackBase(), method(m) {}
	~SQLFunctionCallbackP0();
	void run(QueryResult * data) { method(data); }
};

template<typename T1>
class SQLFunctionCallbackP1 : public SQLCallbackBase
{
	typedef void(*SCMethod)(QueryResult*, T1 p1);
	SCMethod method;
	T1 p1;
public:
	SQLFunctionCallbackP1(SCMethod m, T1 par1) : SQLCallbackBase(), method(m), p1(par1) {}
	~SQLFunctionCallbackP1();
	void run(QueryResult * data) { method(data, p1); }
};

#endif
