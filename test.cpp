#include <boost/python.hpp>
#include <vector>
#include <iostream>
#include <variant>
#include "structure.cpp"

using namespace std;
namespace py = boost::python;
using object = variant<int, float, string>;

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
  for (iter = vector.begin(); iter != vector.end(); ++iter) {
    list.append(*iter);
  }
  return list;
}


char const *greet() {
  return "hello, world";
}

py::list const vec(const py::list lin) {
  vector<int> vec = list_to_vec<int>(lin);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  py::list l = vec_to_list<int>(vec);

  return l;
}

struct World {
  void set(string msg) { this->msg = msg; }
  string greet() { return msg; }
  string msg;
};

struct Var
{
  Var(std::string name) : name(name), value() {}
  std::string const name;
  float value;
};

class FilePos { /*...*/ };

FilePos     operator+(FilePos, int);
FilePos     operator+(int, FilePos);
int         operator-(FilePos, FilePos);
FilePos     operator-(FilePos, int);
FilePos&    operator+=(FilePos&, int);
FilePos&    operator-=(FilePos&, int);
bool        operator<(FilePos, FilePos);


BOOST_PYTHON_MODULE(test) {
  py::def("greet", &greet);
  py::def("vec", &vec);

  py::class_<World>("World")
    .def("greet", &World::greet)
    .def("set", &World::set);

  py::class_<Var>("Var", py::init<std::string>())
    .def_readonly("name", &Var::name)
    .def_readwrite("value", &Var::value);

  py::class_<FilePos>("FilePos")
    .def(py::self + int())          // __add__
    .def(int() + py::self)          // __radd__
    .def(py::self - py::self)           // __sub__
    .def(py::self - int())          // __sub__
    .def(py::self += int())         // __iadd__
    .def(py::self -= py::other<int>())
    .def(py::self < py::self);          // __lt__
}
