#pragma once
#include <iostream>
#include <vector>
#include <string>

#define IID_IUnknown 0
#define IID_IMath 1
#define IID_IExpression 2
#define E_NOINTERFACE "Error"
#define S_OK "OK"
#define E_OUTOFMEMORY "Memory out"

class IUnknown
{
public:
    virtual std::string QueryInterface(int riid, void** ppv) = 0;
    virtual long AddRef() = 0;
    virtual long Release() = 0;
};

class IClassFactory : public IUnknown
{
public:
    virtual std::string CreateInstance(int riid, void** ppv) = 0;
    virtual std::string LockServer(bool fLock) = 0;
};

class MathClassFactory : public IClassFactory
{
protected:
    // Счетчик обращений к экземпляру ClassFactory
    int m_lRef;
public:
    MathClassFactory() : m_lRef(0) {};

    // реализация интерфейса IUnknown 
    virtual std::string QueryInterface(int riid, void** ppv) { std::string str = " "; return str; };
    virtual long AddRef();
    virtual long Release();

    // реализация интерфеса  IClassFactory
    virtual std::string CreateInstance( int riid, void** ppv) override;
    virtual std::string LockServer(bool fLock) override { std::string str = " "; return str; }
};

class IMath : public IUnknown
{
public:
    virtual long Add(long Op1, long Op2) = 0;
    virtual long Subtract(long Op1, long Op2) = 0;
    virtual long Multiply(long Op1, long Op2) = 0;
    virtual long Divide(long Op1, long Op2) = 0;
};

class IExpression : public IUnknown
{
public:
   // virtual void SetExpression(string strExpression) = 0;
    virtual bool Validate() = 0;
    virtual bool Evaluate() = 0;
};

class Math : public IMath, public IExpression
{
public:
    std::string QueryInterface(int riid, void** ppv) override;
    long   AddRef() override;
    long   Release() override;

    long Add(long Op1, long Op2) override;
    long Subtract(long Op1, long Op2) override;
    long Multiply(long Op1, long Op2) override;
    long Divide(long Op1, long Op2) override;

   // void SetExpression(string strExpression) override {};
    bool Validate() override { return false; }
    bool Evaluate() override { return false; }
    // Реализация 
private:
    // Новая переменная-член класса добавлена для подсчета
    // обращений извне к интерфейсу объекта
    int m_lRef;

public:
    Math();
};