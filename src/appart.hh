#pragma once

#include <vector>
#include <numeric>
#include "person.hh"
#include <algorithm>

class Appart
{
public:
   Appart(int nb)
      : number_(nb)
   {
      listPerson.reserve(6);
   };

   float cost() const
   {
      int sex_balance = 0;
      int sex_weights[3] = {1, -1, 0};

      int propre_nb[3] = {0, 0, 0};
      int propre_add = 0;

      int alim_nb[3] = {0, 0, 0};
      int loisir_nb[3] = {0, 0, 0};
      unsigned nb_person = listPerson.size();

      for (const auto& person: listPerson)
      {
         sex_balance += sex_weights[person.sex - 1];

         propre_nb[person.propre - 1]++;

         if ((person.propre == 2) ^ (number_ % 10 == 7))
            propre_add++;

         alim_nb[person.propre - 1]++;
         loisir_nb[person.propre - 1]++;
      }

      sex_balance = std::abs(sex_balance);
      int propre_cost = nb_person - *std::max_element(propre_nb, propre_nb + 3);
      int alim_cost = nb_person - *std::max_element(alim_nb, alim_nb + 3);
      int loisir_cost = nb_person - *std::max_element(loisir_nb, loisir_nb + 3);

      return sex_balance + propre_cost + propre_add + alim_cost + loisir_cost;
   }

   std::vector<Person> listPerson;
   int number_;
};
