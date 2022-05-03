#include "Header.h"

using namespace std;

void InterlockedDecrement(int& value)
{
    --value;
}

void InterlockedIncrement(int& value)
{
    ++value;
}

bool FAILED(std::string error)
{
    if (error == S_OK)
        return false;
    else
        return true;
}

string MathClassFactory::CreateInstance(int riid, void** ppvObj)
{
    Math* pMath;
    std::string hr;

    // Инициализация возвращаемого значения
    // на случай возникновения проблем
    *ppvObj = NULL;

    // Создание нового экземпляра объекта Math 
    pMath = new Math;
    if (pMath == 0)
        return(E_OUTOFMEMORY);

    // Получить у созданного объекта указатель
    // на запрашиваемый интерфейс
    hr = pMath->QueryInterface(riid, ppvObj);
    if (FAILED(hr))
        delete pMath;

    return hr;
}

Math::Math()
{
    m_lRef = 0;
}
// Реализация функции IUnknown::AddRef
long Math::AddRef()
{
    InterlockedIncrement(m_lRef);
    return m_lRef;
}

long MathClassFactory::AddRef()
{
    InterlockedIncrement(m_lRef);
    return m_lRef;
}
std::string Math::QueryInterface(int riid, void** ppv)
{
    switch (riid)
    {
    case IID_IUnknown:
        break;
    case IID_IMath:
        // Множественное наследование требует явного приведения типов
        *ppv = (IMath*)this;
        break;
    case IID_IExpression:
        // Множественное наследование требует явного приведения типов
        *ppv = (IExpression*)this;
        break;
    default:
        return(E_NOINTERFACE);
    }

    // Возвращаем указатель нового интерфейса
    // и таким образом вызываем AddRef для нового указателя
    AddRef();
    return(S_OK);
    
}

// Реализация функии IUnknown:Release
long Math::Release()
{
    InterlockedDecrement(m_lRef);

    // когда значение счетчика обращений
    // становится равным нулю, объект удаляет сам себя
    if (m_lRef == 0)
    {
        delete this;
        // нельзя вернуть m_lRef, поскольку его уже не существует
        return 0;
    }
    else
        return m_lRef;
}
long MathClassFactory::Release()
{
    InterlockedDecrement(m_lRef);

    // когда значение счетчика обращений
    // становится равным нулю, объект удаляет сам себя
    if (m_lRef == 0)
    {
        delete this;
        // нельзя вернуть m_lRef, поскольку его уже не существует
        return 0;
    }
    else
        return m_lRef;
}


long Math::Add(long Op1, long Op2)
{
    return Op1 + Op2;
}

long Math::Subtract(long Op1, long Op2)
{
    return Op1 - Op2;
}
long Math::Multiply(long Op1, long Op2) {
    return Op1 * Op2;
}
long Math::Divide(long Op1, long Op2)
{
    return Op1 / Op2;
}



int main() {

    MathClassFactory* pCF = new MathClassFactory;
    pCF->AddRef();

    std::string hr;
    // Используя интерфейс фабрики классов, создаем экземпляр 
    // компонента Math
    IMath* pMath = NULL;
    hr = pCF->CreateInstance(IID_IMath, (void**)&pMath);

    // Освободить интерфейс фабрики классов
    pCF->Release();

    long result;
    result = pMath->Multiply(100, 8);
    cout << "100 * 8 is " << result << endl;
    result = pMath->Subtract(1000, 333);
    cout << "1000 - 333 is " << result << endl;

    pMath->Release();

    return 0;
}