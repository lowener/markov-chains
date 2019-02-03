#pragma once

class Person
{
public:
   Person(int a, int b, int c, int d, int e)
      : sex(a), propre(b), alim(c), loisir(d),
        number(e)
   {}

   Person(const Person& p)
      : sex(p.sex), propre(p.propre), alim(p.alim), loisir(p.loisir),
        number(p.number)
   {}
   Person(Person&& p)
      : sex(p.sex), propre(p.propre), alim(p.alim), loisir(p.loisir),
        number(p.number)
   {}

   Person& operator=(Person p)
   {
      sex = p.sex;
      propre = p.propre;
      alim = p.alim;
      loisir = p.loisir;
      number = p.number;
      return *this;
   }


   int sex, propre, alim, loisir, number;
};
