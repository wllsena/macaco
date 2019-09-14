#include <vector>
#include <iostream>
#include <variant>
#include <stdlib.h>

// types int = 0, float = 1, string = 2
using namespace std;

//---

template <class T>
struct serie {
  vector<T>   cells;
  vector<int> indexes;
  int         size = 0;

  void add (T &key) {
    auto lower = cells.begin();
    auto upper = cells.end() - 1;
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower) {
      if (key < *mid)
        upper = mid;
      else
        lower = mid;

      mid = lower + (upper - lower) / 2;
    }

    mid += (cells.size() == 0 or key < *mid ? 0 : 1);
    indexes.insert(indexes.begin() + (mid - cells.begin()), size);
    cells.insert(mid, key);
    size++;
  }

  int search (T &key) {
    auto lower = cells.begin();
    auto upper = cells.end() - 1;
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower or key == *lower) {
      if (key == *mid)
        return *(indexes.begin() + (mid - cells.begin()));

      if (key < *mid)
        upper = mid;
      else
        lower = mid;

      mid = lower + (upper - lower) / 2;
    }

    return -1;
  }
};

//---

using object = variant<int, float, string>;
using pS_object = variant<serie<int> *, serie<float> *, serie<string> *>;

class BF {
protected:

  int                      size[2] = {0,0};
  vector<vector<object> *> pData;
  vector<pS_object>        pSeries;

public:
  BF(int axis0, int axis1, vector<int> types) {
    size[0] = axis0;
    size[1] = axis1;
    for (int i = 0; i != axis0; i++)
      pData.push_back(new vector<object>);

    for (int i = 0; i != axis1; i++) {
      pS_object s_obj;

      if (types[i] == 0)
        s_obj = new serie<int>;
      else if (types[i] == 1)
        s_obj = new serie<float>;
      else if (types[i] == 2)
        s_obj = new serie<string>;

      pSeries.push_back(s_obj);
    }
  }

  template <class T>
  void add_cell (int index, T value) {
    pData[get<serie<T> *>(pSeries[index])->size]->push_back(value);
    get<serie<T> *>(pSeries[index])->add(value);
  }

  template <class T>
  void fill_column (int index, vector<T> vec) {
    for (auto i = vec.begin(); i != vec.end(); i++)
      add_cell<T>(index, *i);
  }

  vector<object> take_row (int index) {
    return *pData[index];
  }

  template <class T>
  void query (int index, T key) {
    cout << get<serie<T> *>(pSeries[index])->search(key) << endl;
  }
};
