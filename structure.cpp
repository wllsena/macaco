#include <vector>
#include <iostream>
#include <variant>

using namespace std;
using object = variant<int, float, string>;

//---

struct cell {
  object value;
  int    index;
  cell (object v, int i): value(v), index(i) {}
};

//---

struct serie {
  vector<cell>   cells;
  vector<object> data;
  int            size = 0;

  void add (object &key) {
    auto lower = cells.begin();
    auto upper = cells.end();
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower and mid != upper) {
      if (key < mid->value)
        upper = mid;
      else
        lower = mid;
      mid = lower + (upper - lower) / 2;
    }

    cells.insert(mid + (size == 0 or key < mid->value ? 0 : 1), cell(key, size));
    data.push_back(key);
    size++;
  }

  void adds (vector<object> &vec) {
    for (auto i = vec.begin(); i != vec.end(); i++)
      add(*i);
  }

  template <class T>
  void print () {
    for (auto i = cells.begin(); i != cells.end(); i++) {
      cout << get<T>(i->value) << "_" << i->index << " ";
    }
    cout << endl;
  }

  serie (vector<object> &vec) {
    adds(vec);
  };
};

//---

class dataframe {
protected:

  vector<serie*> pSeries;
  int            size = 0;

public:

  void add_serie (vector<object> &vec) {
    pSeries.push_back(new serie(vec));
    size++;
  }

  template <class T>
  vector<T> to_vec (const int &index) {
    vector<T> vec;
    for (auto i = pSeries[index]->data.begin(); i != pSeries[index]->data.end(); i++) {
      vec.push_back(get<T>(*i));
    }
    return vec;
  }

  template <class T>
  void print (const int &index) {
    pSeries[index]->print<T>();
  }
};
