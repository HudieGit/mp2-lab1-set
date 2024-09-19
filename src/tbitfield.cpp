// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    BitLen = len;
    if (len <= 0) {
        throw "incorrect length";// проверка на отрицательность 
    }
    MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);  //формула
    pMem = new TELEM[MemLen](); //создание памяти
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    this->pMem = new TELEM[MemLen]();
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }

}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int m = n % (sizeof(TELEM) * 8);
    TELEM Mask = 1; // создание маски с младшим битом 1
    Mask <<= m; // двигаем бит на нужную позицию
    return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {  // проверка на отрицательность и существование
        throw "incorrect index";
    }

    int index = GetMemIndex(n);  // грубо говоря объединяем с маской, где в нужном месте есть единица
    TELEM Mask = GetMemMask(n);
    pMem[index] |= Mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {  // проверка на отрицательность и существование
        throw "incorrect index";
    }

    int index = GetMemIndex(n); // аналогичная ситуация как с установкой, только необходимо инвентировать маску(т.к операция противоположная)
    TELEM Mask = GetMemMask(n);
    pMem[index] ^= Mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {  // проверка на отрицательность и существование
        throw "incorrect index";
    }

    TELEM Mask = GetMemMask(n);
    int index = GetMemIndex(n);
    Mask &= pMem[index];
    if (Mask) {
        return 1;
    }
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        this->BitLen = bf.BitLen;
        this->MemLen = bf.MemLen;
        this->pMem = new TELEM[MemLen]();
        for (int i = 0; i < MemLen; i++) {
            this->pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }

    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int MaxLen = BitLen;
    if (BitLen < bf.BitLen) MaxLen = bf.BitLen;

    TBitField result(MaxLen);

    for (int i = 0; i < result.MemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int MaxLen = BitLen;
    if (BitLen < bf.BitLen) MaxLen = bf.BitLen;

    TBitField result(MaxLen);

    for (int i = 0; i < result.MemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField inverse(BitLen);
    for (int i = 0; i < inverse.MemLen; i++) {
        inverse.pMem[i] = ~pMem[i];
    }

    if (BitLen % (sizeof(TELEM) * 8)) {
        TELEM Mask = (1 << (BitLen % (sizeof(TELEM) * 8))) - 1;
        inverse.pMem[MemLen - 1] &= Mask;
    }

    return inverse;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++) {
        bool bit;
        istr >> bit;
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}

