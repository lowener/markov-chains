#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "apt.hh"
#include "person.hh"
#include <cmath>


int rnd(int max)
{
   static std::mt19937 rng;
   static bool init = true;
   if (init){
      int seed = std::random_device()();
      std::cerr << "Seed: " << seed << std::endl;
      rng.seed(seed);
      init = false;
   }
   std::uniform_int_distribution<std::mt19937::result_type> dist6(0,max);
   return dist6(rng);
}


std::vector<apt> annealing(std::vector<apt> listApt, float temp=500000, float decay=0.999)
{
   int cost = 0;
   for (const auto& a: listApt)
      cost += a.cost();
   int apt_nb = listApt.size();
   int round_blank = 0;

   while (temp > 5 && round_blank < 500)
   {
      int rnd_apt1 = rnd(apt_nb - 1);
      int rnd_apt2 = rnd(apt_nb - 1);
      int rnd_person1 = rnd(5);
      int rnd_person2 = rnd(5);
      int delta_cost = listApt[rnd_apt1].cost()+ listApt[rnd_apt2].cost();
      std::swap(listApt[rnd_apt1].listPerson[rnd_person1],
                listApt[rnd_apt2].listPerson[rnd_person2]);
      delta_cost -= listApt[rnd_apt1].cost()+ listApt[rnd_apt2].cost();
      double proba = std::exp(- ((float)delta_cost) / temp);

      if (delta_cost < 0)
      {
         round_blank = 0;
         cost += delta_cost;
      }
      else
      {
         std::uniform_real_distribution<double> unif(0, 1);
         std::default_random_engine re;
         double a_random_double = unif(re);

         if (a_random_double > proba)
         {
            round_blank = 0;
            cost += delta_cost;
         }
         else
         {
            round_blank++;
            std::swap(listApt[rnd_apt1].listPerson[rnd_person1],
                      listApt[rnd_apt2].listPerson[rnd_person2]);

         }
      }
      temp = temp * decay;
      std::cerr << "Temp: " << temp << ", cost: " << cost << ", delta: "
                << delta_cost << ", blanks: " << round_blank << std::endl;
   }

   return listApt;
}

int main(int argc, char** argv)
{
   if (argc != 3)
   {
      std::cout << "Invalid number of arguments" << std::endl;
      return 1;
   }
   char* filein = argv[1];
   char* fileout = argv[2];

   std::ifstream fin(filein);
   std::ofstream fout(fileout);
   std::string line;
   std::vector<Person> matrix;

   int nb = 0;
   while (std::getline(fin, line))
   {
      int a, b, c, d;
      a = line[0] - '0';
      b = line[2] - '0';
      c = line[4] - '0';
      d = line[6] - '0';
      matrix.emplace_back(a, b, c, d, nb);
      nb++;
   }

   // Fill apt
   std::vector<apt> listApt(matrix.size() / 6, 0);
   for (unsigned i = 0; i < listApt.size(); ++i)
      listApt[i].number_ = i;
   for (unsigned i = 0; i < matrix.size(); ++i)
      listApt[i / 6].listPerson.push_back(matrix[i]);


   // Annealing
   std::vector<apt> res = annealing(listApt);
   for (const auto& appart: res)
   {
      for (int i = 0; i < 5; ++i)
         fout << appart.listPerson[i].number + 1 << " ";
      fout << appart.listPerson[5].number + 1 << std::endl;
   }
   return 0;
}
