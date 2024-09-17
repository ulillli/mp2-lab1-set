// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField) 
{
    MaxPower = s.GetMaxPower();
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    TBitField tmp(BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (this->MaxPower == s.GetMaxPower()) {
        return BitField == s.BitField;
    }
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this==s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet tmp(BitField|s.BitField);
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem-1 < MaxPower) {
        TSet tmp(BitField);
        tmp.InsElem(Elem);
        return tmp;
    }
    else throw "Index out of range\n";
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem-1 < MaxPower) {
        TSet tmp(BitField);
        tmp.DelElem(Elem);
        return tmp;
    }
    else throw "Index out of range\n";
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(this->BitField&s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    for (int i = 0; i <s.GetMaxPower(); i++) {
        int tmp; 
        istr >> tmp;
        if (tmp)
         s.InsElem(i);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{" ;
    for (int i = 0; i <s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << i << ",";
        }
    }
    ostr <<  "}\n";
    return ostr;
}
