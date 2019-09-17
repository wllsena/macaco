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

  const void import(const vector<T> cel, const vector<int> ind, const int dep) {
    cells   = cel;
    indexes = ind;
    depth   = dep;
  }

  const void add (const T &key) {
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

  const void cut_pieces(const vector<int> &indexs) {
    vector<int> vec(depth);
    vector<T>   pc_cells (depth);
    vector<int> pc_indexes (depth, -1);

    for (int i = 0; i != depth; i++)
      vec[indexes[i]] = i;
    indexes = vec;

    for (int i = 0; i < indexs.size(); i++) {
      pc_cells[indexes[indexs[i]]] = cells[indexes[indexs[i]]];
      pc_indexes[indexes[indexs[i]]] = i;
    }

    cells.clear();
    indexes.clear();

    for (int i = 0; i < depth; i++) {
      if (pc_indexes[i] != -1) {
        cells.push_back(pc_cells[i]);
        indexes.push_back(pc_indexes[i]);
      }
    }

    depth = indexes.size();
  }

  const vector<int> search (const T &key) {
    vector<int> vec;
    auto lower = cells.begin();
    auto upper = cells.end();
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower or key == *lower) {
      if (key == *mid) {
        vec.push_back(*(indexes.begin() + (mid - cells.begin())));

        if (mid != cells.begin())
          for (lower = mid - 1; lower + 1 > cells.begin() and *lower == key; lower--)
            vec.push_back(*(indexes.begin() + (lower - cells.begin())));

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

    return vec;
  }

  const vector<int> between (const T &min, const T &max) {
    vector<int> vec;
    auto lower = cells.begin();
    auto upper = cells.end();
    auto mid   = lower + (upper - lower) / 2;

    while (mid != lower or (min <= *mid and *mid <= max)) {
      if (min <= *mid and *mid <= max) {
        vec.push_back(*(indexes.begin() + (mid - cells.begin())));

        if (mid != cells.begin())
          for (lower = mid - 1; lower + 1 > cells.begin() and min <= *lower; lower--)
            vec.push_back(*(indexes.begin() + (lower - cells.begin())));

        if (mid != cells.end() - 1)
          for (upper = mid + 1; cells.end() > upper and *upper <= max; upper++)
            vec.push_back(*(indexes.begin() + (upper - cells.begin())));

        return vec;
      }

      if (max < *mid)
        upper = mid;
      else
        lower = mid;

      mid = lower + (upper - lower) / 2;
    }

    return vec;
  }
};

//---

using object = variant<int, float, string>;
using pS_object = variant<serie<int> *, serie<float> *, serie<string> *>;

struct BF {
  int                      axis0, axis1;
  vector<int>              types;
  vector<vector<object> *> pData;
  vector<pS_object>        pSeries;

  BF (const int &ax0, const int &ax1, const vector<int> &tps) : axis0(ax0), axis1(ax1), types(tps) {
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

  const void import (const vector<vector<object> *> &new_pData, const vector<pS_object> &new_pSeries) {
    for (int i = 0; i != axis0; i++)
      *pData[i] = *new_pData[i];

    for (int i = 0; i != axis1; i++)
      if (types[i] == 0) {
        get<serie<int> *>(pSeries[i])->import(get<serie<int> *> (new_pSeries[i])->cells,
                                              get<serie<int> *> (new_pSeries[i])->indexes, axis0);

      }else if (types[i] == 1) {
        get<serie<float> *>(pSeries[i])->import(get<serie<float> *> (new_pSeries[i])->cells,
                                                get<serie<float> *> (new_pSeries[i])->indexes, axis0);

      }else if (types[i] == 2) {
        get<serie<string> *>(pSeries[i])->import(get<serie<string> *> (new_pSeries[i])->cells,
                                                 get<serie<string> *> (new_pSeries[i])->indexes, axis0);
      }
  }

  template <class T>
  const void add_cell (const int &index, const T &value) {
    pData[get<serie<T> *>(pSeries[index])->depth]->push_back(value);
    get<serie<T> *>(pSeries[index])->add(value);
  }

  template <class T>
  const void fill_column (const int &index, const vector<T> &vec) {
    for (auto i = vec.begin(); i != vec.end(); i++)
      add_cell<T>(index, *i);
  }

  template <class T>
  const void add_column (const int &type, const vector<T> &vec) {
    pSeries.push_back(new serie<T>);
    types.push_back(type);
    fill_column(axis1, vec);
    axis1++;
  }

  template <class T>
  const vector<int> query (const int &index, const T &key) {
    return get<serie<T> *>(pSeries[index])->search(key);
  }

  template <class T>
  const vector<int> between (const int &index, const T &min, const T &max) {
    return get<serie<T> *>(pSeries[index])->between(min, max);
  }

  template <class T>
  const vector<T> take_serie (const int &index) {
    return get<serie<T> *>(pSeries[index])->cells;
  }

  template <class T>
  const vector<int> take_serie_indexes (const int &index) {
    return get<serie<T> *>(pSeries[index])->indexes;
  }

  template <class T>
  const vector<T> take_column (const int &index) {
    vector<T> vec;
    for (auto d = pData.begin(); d != pData.end(); d++)
      vec.push_back(get<T>((**d)[index]));
    return vec;
  }

  const vector<object> take_row (const int &index) {
    return *pData[index];
  }

  const void slice (const vector<int> &indexs) {
    vector<vector<object> *> new_pData(indexs.size());

    for (int i = 0; i != indexs.size(); i++)
      new_pData[i] = pData[indexs[i]];

    pData.clear();
    pData = new_pData;

    for (int i = 0; i < axis1; i++) {
      if (types[i] == 0)
        get<serie<int> *>(pSeries[i])->cut_pieces(indexs);
      else if (types[i] == 1)
        get<serie<float> *>(pSeries[i])->cut_pieces(indexs);
      else if (types[i] == 2)
        get<serie<string> *>(pSeries[i])->cut_pieces(indexs);
    }

    axis0 = indexs.size();
  }
};
