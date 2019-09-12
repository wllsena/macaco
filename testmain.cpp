#include <vector>
#include <iostream>
#include <variant>
#include "structure.cpp"

using namespace std;
using object = variant<int, float, string>;

//---

int main () {

  vector<object> fuck1;
  fuck1.push_back(1);
  fuck1.push_back(2);
  fuck1.push_back(3);
  fuck1.push_back(4);
  fuck1.push_back(11);
  fuck1.push_back(6);
  fuck1.push_back(7);
  fuck1.push_back(8);
  fuck1.push_back(-20);

  vector<object> fuck2;
  fuck2.push_back(1.1f);
  fuck2.push_back(2.5f);
  fuck2.push_back(2.3f);
  fuck2.push_back(4.6f);
  fuck2.push_back(11.8f);
  fuck2.push_back(6.1f);
  fuck2.push_back(7.2f);
  fuck2.push_back(8.5f);
  fuck2.push_back(-20.5f);

  vector<object> fuck3;
  fuck3.push_back("assd");
  fuck3.push_back("2.3f");
  fuck3.push_back("3.5f");
  fuck3.push_back("bsad");
  fuck3.push_back("e");
  fuck3.push_back("aluno");
  fuck3.push_back("william");
  fuck3.push_back("fuck");
  fuck3.push_back("merda");

  dataframe df;
  df.add_serie(fuck1);
  df.add_serie(fuck2);
  df.add_serie(fuck3);
  df.print<int>(0);
  df.print<float>(1);
  df.print<string>(2);

  vector<string> vec = df.to_vec<string>(2);
  for (auto i = vec.begin(); i != vec.end(); i++) {
    cout << *i << " ";
  }
  cout << endl << endl;
}
