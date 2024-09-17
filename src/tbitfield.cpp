// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len >0){
            MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
            pMem = new TELEM[MemLen];
            BitLen = len;
            for (int i = 0; i < MemLen; i++) {
                pMem[i] = 0;
            }
    }
    else {
        throw "Not correct len\n";
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
   BitLen = bf.GetLength();
   MemLen = bf.MemLen;
   pMem = new TELEM[MemLen];
   for (int i = 0; i <MemLen; i++) {
       pMem[i] = bf.pMem[i];
       //std::cout << pMem[i] << " " << bf.pMem[i] << std::endl;
   }
   //*pMem = (~(0))&(*bf.pMem); 
}

TBitField::~TBitField()
{
    if (pMem != nullptr) {
        delete [] pMem;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < BitLen) {
        return n/(sizeof(TELEM)*8);
    }
    else throw "Index out of range\n";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
   TELEM tmp=0;
    if ((n < BitLen)&&(0<=n)) {
        tmp = (1 << sizeof(TELEM)*8-n%(sizeof(TELEM) * 8) - 1);

    }
    else throw "Index out of range\n";
    return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
   pMem[GetMemIndex(n)] |= (GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (GetBit(n) == 1) {
        pMem[GetMemIndex(n)] &= (~GetMemMask(n));
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  return ((pMem[GetMemIndex(n)])&(GetMemMask(n)))!=0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (&bf != this) {
        delete [] pMem;
        BitLen = bf.GetLength();
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
 
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int flag = 1;
    int minLength = 0, maxLength;
    if (this->BitLen >= bf.GetLength()) {
        maxLength = this->BitLen;
        minLength = bf.GetLength();
        for (int i = 0; i < minLength; i++) {
            if (this->GetBit(i) != bf.GetBit(i)) {
                flag = 0;
                break;
            }
        }
        for (int i = minLength; i < maxLength; i++) {
            if (0!= this->GetBit(i)) {
                flag = 0;
                break;
            }
        }
    }
    else {
        maxLength = bf.GetLength();
        minLength = this->BitLen;
        for (int i = 0; i < minLength; i++) {
            if (this->GetBit(i) != bf.GetBit(i)) {
                flag = 0;
                break;
            }
        }
        for (int i = minLength; i < maxLength; i++) {
            if (0 != bf.GetBit(i)) {
                flag = 0;
                break;
            }
        }
    }
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int flag = 0;
    int minLength = 0, maxLength;
    if (this->BitLen >= bf.GetLength()) {
        maxLength = this->BitLen;
        minLength = bf.GetLength();
        flag = 1;
    }
    else {
        maxLength = bf.GetLength();
        minLength = this->BitLen;
        flag = 2;
    }
    TBitField tmp(maxLength);
    for (int i = 0; i < minLength; i++) {
        if ((this->GetBit(i) || bf.GetBit(i)) == 1) tmp.SetBit(i);
    }
    if (flag == 1) {
        for (int i = minLength; i < maxLength; i++) {
            if (this->GetBit(i)) tmp.SetBit(i);
        }
    }
    else {
        for (int i = minLength; i < maxLength; i++) {
            if (bf.GetBit(i)) tmp.SetBit(i);
        }
    }

    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
   int minLength = 0, maxLength;
   if (this->BitLen >= bf.GetLength()) {
       maxLength = this->BitLen;
       minLength = bf.GetLength();
   }
   else {
       maxLength = bf.GetLength();
       minLength = this->BitLen;
   }
   TBitField tmp(maxLength);
   for (int i = 0; i < minLength; i++) {
        if ((this->GetBit(i) && bf.GetBit(i)) == 1) tmp.SetBit(i);
    }
   for (int i = minLength; i < maxLength; i++) {
       tmp.ClrBit(i);
   }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
    for (int i = 0; i < tmp.GetLength(); i++) {
        if (this->GetBit(i) == 1) {
            tmp.ClrBit(i);
        }
        else tmp.SetBit(i);
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        int tmp = 0;
        istr >> tmp;
        if (tmp == 1) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
        /*if (i % 8==0) {
            ostr << std::endl;
        }*/
    }
    ostr << std::endl;
    return ostr;
}
