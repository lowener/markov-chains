#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "appart.hh"
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

double rnd_double(double max = 1.0)
{
   static std::mt19937 rng;
   static bool init = true;
   if (init){
      int seed = std::random_device()();
      std::cerr << "Seed: " << seed << std::endl;
      rng.seed(seed);
      init = false;
   }
   std::uniform_real_distribution<> dist(0.0,max);
   return dist(rng);
}

std::vector<Appart> annealing(std::vector<Appart> listAppart)
{
   int cost = 0;
   for (const auto& a: listAppart)
      cost += a.cost();
   int appart_nb = listAppart.size();
   int round_blank = 0;
   float temp = appart_nb * 10;
   float decay = (float)(appart_nb * 1000 - 1) / (appart_nb * 1000);

   while (temp > 0.0000001 && round_blank < 500)
   {
      int rnd_appart1 = rnd(appart_nb - 1);
      int rnd_appart2 = rnd(appart_nb - 1);
      int rnd_person1 = rnd(5);
      int rnd_person2 = rnd(5);
      int delta_cost = - (listAppart[rnd_appart1].cost()+ listAppart[rnd_appart2].cost());
      std::swap(listAppart[rnd_appart1].listPerson[rnd_person1],
                listAppart[rnd_appart2].listPerson[rnd_person2]);
      delta_cost += listAppart[rnd_appart1].cost() + listAppart[rnd_appart2].cost();

      float proba = std::exp(- ((float)delta_cost) / (temp));

      double a_random_double = rnd_double();
      if (delta_cost < 0)
      {
         round_blank = 0;
         cost += delta_cost;
      }
      else
      {

         if (a_random_double < proba)
         {
            round_blank = 0;
            cost += delta_cost;
         }
         else
         {
            round_blank++;
            std::swap(listAppart[rnd_appart1].listPerson[rnd_person1],
                      listAppart[rnd_appart2].listPerson[rnd_person2]);

         }
      }
      temp = temp * decay;
/*      std::cerr << "Temp: " << temp << ", cost: " << cost << ", delta: "
                << delta_cost << ", blanks: " << round_blank
                <<  ", proba: " << proba
                << ", rnd: " << a_random_double << std::endl;*/
   }
   cost = 0;
   for (const auto& a: listAppart)
      cost += a.cost();
   std::cerr << "cost: " << cost << ' ' << decay << std::endl;

   return listAppart;
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

   // Fill appart
   std::vector<Appart> listAppart(matrix.size() / 6, 0);
   for (unsigned i = 0; i < listAppart.size(); ++i)
      listAppart[i].number_ = i;
   for (unsigned i = 0; i < matrix.size(); ++i)
      listAppart[i / 6].listPerson.push_back(matrix[i]);


   // Annealing
   std::vector<Appart> res = annealing(listAppart);
   for (const auto& appart: res)
   {
      for (int i = 0; i < 5; ++i)
         fout << appart.listPerson[i].number + 1 << " ";
      fout << appart.listPerson[5].number + 1 << std::endl;
   }
   return 0;
}
