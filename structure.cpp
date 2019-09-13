#include <vector>
#include <iostream>
#include <variant>
#include <stdlib.h>

using namespace std;

//---

template <class T>
struct cell {
  T   value;
  int index;
  cell (T v, int i): value(v), index(i) {}
};

//---

template <class T>
struct serie {
  vector<cell<T> > cells;
  vector<T>        data;

  void add (T &key) {
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

    cells.insert(mid + (data.size() == 0 or key < mid->value ? 0 : 1), cell(key, data.size()));
    data.push_back(key);
  }
};

//---

using serie_T = variant<serie<int> *, serie<float> *, serie<string>*>;

class BF {
protected:

  vector<serie_T> pSeries;

public:


  template <class T>
  void add_cell (int index, T value) {
    get<serie<T> *>(pSeries[index])->add(value);
  }

  template <class T>
  void add_column (vector<T> vec) {
    pSeries.push_back(new serie<T>);

    for (auto i = vec.begin(); i != vec.end(); i++)
      add_cell<T>(pSeries.size() - 1, *i);
  }

  void add_row (vector<string> types, vector<char *> vec) {
    for (int i = 0; i != vec.size(); i++) {
      if (types[i] == "string")
        add_cell<string>(i, vec[i]);
      else if (types[i] == "int")
        add_cell<int>(i, atoi(vec[i]));
      else
        add_cell<float>(i, atof(vec[i]));
    }
  }

  template <class T>
  vector<T> to_vec (int index) {
    vector<T> vec;
    for (auto i = get<serie<T> *>(pSeries[index])->data.begin();
         i != get<serie<T> *>(pSeries[index])->data.end(); i++)
      vec.push_back(*i);
    return vec;
  }
};
