#include <boost/python.hpp>
#include <vector>
#include <iostream>
#include "structure.cpp"

using namespace std;
namespace py = boost::python;
using object = variant<int, float, string>;

//---

template <typename T>
inline
vector<T> list_to_vec(const py::object& iterable) {
  return vector<T> (py::stl_input_iterator<T> (iterable), py::stl_input_iterator<T> ());
}


template <class T>
inline
py::list vec_to_list(vector<T> vector) {
  typename std::vector<T>::iterator iter;
  py::list list;
  for (iter = vector.begin(); iter != vector.end(); ++iter)
    list.append(*iter);
  return list;
}

class BananaFrame {
private:

  BF* bf;

public:
  BananaFrame(const int axis0, const int axis1, const py::list types) {
    bf = new BF(axis0, axis1, list_to_vec<int>(types));
  }

  void fill_column(const int index, const py::list list) {
    if (bf->types[index] == 0)
      bf->fill_column<int>(index, list_to_vec<int>(list));
    else if (bf->types[index] == 1)
      bf->fill_column<float>(index, list_to_vec<float>(list));
    else if (bf->types[index] == 2)
      bf->fill_column<string>(index, list_to_vec<string>(list));
  }

  py::list const take_row(const int index) {
    vector<object> vec = bf->take_row(index);
    py::list list;

    for (int i = 0; i != bf->axis0; i++) {
      if (bf->types[i] == 0)
        list.append(get<int>(vec[i]));
      else if (bf->types[i] == 1)
        list.append(get<float>(vec[i]));
      else if (bf->types[i] == 2)
        list.append(get<string>(vec[i]));
    }

    return list;
  }

  py::list const take_serie(const int index) {
    if (bf->types[index] == 0)
      return vec_to_list<int>(bf->take_serie<int>(index));
    else if (bf->types[index] == 1)
      return vec_to_list<float>(bf->take_serie<float>(index));
    else
      return vec_to_list<string>(bf->take_serie<string>(index));
  }

  py::list const take_column(const int index) {
    if (bf->types[index] == 0)
      return vec_to_list<int>(bf->take_column<int>(index));
    else if (bf->types[index] == 1)
      return vec_to_list<float>(bf->take_column<float>(index));
    else
      return vec_to_list<string>(bf->take_column<string>(index));
  }

  py::list const take_serie_indexes(const int index) {
    vector<int> vec;

    if (bf->types[index] == 0)
      vec = bf->take_serie_indexes<int>(index);
    else if (bf->types[index] == 1)
      vec = bf->take_serie_indexes<float>(index);
    else
      vec = bf->take_serie_indexes<string>(index);

    return vec_to_list<int>(vec);
  }


  py::list const query(const int index, const py::list key) {
    vector<int> vec;

    if (bf->types[index] == 0)
      vec = bf->query<int>(index, list_to_vec<int>(key)[0]);
    else if (bf->types[index] == 1)
      vec = bf->query<float>(index, list_to_vec<float>(key)[0]);
    else if (bf->types[index] == 2)
      vec = bf->query<string>(index, list_to_vec<string>(key)[0]);

    return vec_to_list<int>(vec);
  }

  void slice(const py::list indexs) {
    bf->slice(list_to_vec<int>(indexs));
  }
};


BOOST_PYTHON_MODULE(integration) {
  py::class_<BananaFrame>("BananaFrame", py::init<int, int, py::list>())
    .def("fill_column", &BananaFrame::fill_column)
    .def("take_row", &BananaFrame::take_row)
    .def("take_column", &BananaFrame::take_column)
    .def("query", &BananaFrame::query)
    .def("take_serie", &BananaFrame::take_serie)
    .def("take_serie_indexes", &BananaFrame::take_serie_indexes)
    .def("slice", &BananaFrame::slice);
};
