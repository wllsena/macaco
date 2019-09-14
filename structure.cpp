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
  int         depth = 0;

  void add (T &key) {
    auto lower = cells.begin();
    auto upper = cells.end();
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower) {
      if (key < *mid)
        upper = mid;
      else
        lower = mid;

      mid = lower + (upper - lower) / 2;
    }

    mid += (depth == 0 or key < *mid ? 0 : 1);
    indexes.insert(indexes.begin() + (mid - cells.begin()), depth);
    cells.insert(mid, key);
    depth++;
  }

  void organize_indexes () {
    vector<int> vec(depth);
    for (int i = 0; i != depth; i++)
      vec[indexes[i]] = i;
    indexes = vec;
  }

  void cut_pieces(vector<int> indexs) {
    vector<T>   pc_cells (depth);
    vector<int> pc_indexes (depth, -1);

    organize_indexes();

    for (int i = 0; i < indexs.size(); i++) {
      pc_cells[indexes[indexs[i]]] = cells[indexes[indexs[i]]];
      pc_indexes[indexes[indexs[i]]] = i;
    }

    cells   = vector<T> ();
    indexes = vector<int> ();

    for (int i = 0; i < depth; i++) {
      if (pc_indexes[i] != -1) {
        cells.push_back(pc_cells[i]);
        indexes.push_back(pc_indexes[i]);
      }
    }

    depth = indexes.size();
  }

  vector<int> search (T &key) {
    auto lower = cells.begin();
    auto upper = cells.end();
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower or key == *lower) {
      if (key == *mid) {
        vector<int> vec;

        if (mid != cells.begin())
          for (lower = mid - 1; lower + 1 > cells.begin() and *lower == key; lower--)
            vec.push_back(*(indexes.begin() + (lower - cells.begin())));

        vec.push_back(*(indexes.begin() + (mid - cells.begin())));

        if (mid != cells.end() - 1)
          for (upper = mid + 1; cells.end() > upper and *upper == key; upper++)
            vec.push_back(*(indexes.begin() + (upper - cells.begin())));

        return vec;
      }

      if (key < *mid)
        upper = mid;
      else
        lower = mid;

      mid = lower + (upper - lower) / 2;
    }

    return vector<int> ();
  }
};

//---

using object = variant<int, float, string>;
using pS_object = variant<serie<int> *, serie<float> *, serie<string> *>;

class BF {
protected:

  vector<vector<object> *> pData;
  vector<pS_object>        pSeries;

public:

  int         axis0, axis1;
  vector<int> types;

  BF(int ax0, int ax1, vector<int> tps) : axis0(ax0), axis1(ax1), types(tps) {
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
    pData[get<serie<T> *>(pSeries[index])->depth]->push_back(value);
    get<serie<T> *>(pSeries[index])->add(value);
  }

  template <class T>
  void fill_column (int index, vector<T> vec) {
    for (auto i = vec.begin(); i != vec.end(); i++)
      add_cell<T>(index, *i);
  }

  template <class T>
  vector<int> query (int index, T key) {
    return get<serie<T> *>(pSeries[index])->search(key);
  }

  template <class T>
  vector<T> take_serie (int index) {
    return get<serie<T> *>(pSeries[index])->cells;
  }

  template <class T>
  vector<int> take_serie_indexes (int index) {
    return get<serie<T> *>(pSeries[index])->indexes;
  }

  template <class T>
  vector<T> take_column (int index) {
    vector<T> vec;
    for (auto d = pData.begin(); d != pData.end(); d++)
      vec.push_back(get<T>((**d)[index]));
    return vec;
  }

  vector<object> take_row (int index) {
    return *pData[index];
  }

  void slice (vector<int> indexs) {
    vector<vector<object> *> new_pData;
    for (auto ii = indexs.begin(); ii != indexs.end(); ii++)
      new_pData.push_back(pData[*ii]);
    pData = new_pData;

    vector<pS_object> new_pSeries = pSeries;
    for (int i = 0; i < axis1; i++) {
      if (types[i] == 0)
        get<serie<int> *>(new_pSeries[i])->cut_pieces(indexs);
      else if (types[i] == 1)
        get<serie<float> *>(new_pSeries[i])->cut_pieces(indexs);
      else if (types[i] == 2)
        get<serie<string> *>(new_pSeries[i])->cut_pieces(indexs);
    }

    axis0 = indexs.size();
  }
};
