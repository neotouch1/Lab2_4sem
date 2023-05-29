#pragma once
#include <string>

struct Variable {
  union data {
    int i;
    double d;
  };
  data val;
  bool isInt;
  std::string name;

  Variable() = default;
  Variable(double dd, std::string _name = "none") :isInt(0) { val.d = dd; name = _name; }
  Variable(int ii, std::string _name = "none") :isInt(1) { val.i = ii; name = _name; }
  Variable(std::string s, std::string _name = "none") {
    name = _name;
    if (s.find('.') != std::string::npos) {
      isInt = 0; val.d = stod(s);
    }
    else {
      isInt = 1; val.i = stoi(s);
    }
  }
  Variable operator+(const Variable& v) {
    if (isInt && v.isInt)
      return Variable(val.i + v.val.i);
    if (isInt) // int + double = double
      return Variable(double(val.i) + v.val.d);
    return Variable(val.d + double(v.val.i));
  }
  Variable operator-() {
    if (isInt)
      return Variable(-val.i);
    return Variable(-val.d);
  }
  Variable operator-(const Variable& v) {
    if (isInt && v.isInt)
      return Variable(val.i - v.val.i);
    if (isInt) // int - float = float
      return Variable(double(val.i) - v.val.d);
    return Variable(val.d - double(v.val.i));
  }
  Variable operator*(const Variable& v) {
    if (isInt && v.isInt)
      return Variable(val.i * v.val.i);
    if (isInt) // int * float = float
      return Variable(double(val.i) * v.val.d);
    return Variable(val.d * double(v.val.i));
  }
  Variable operator/(const Variable& v) {
    if (isInt && v.isInt) // int div int = int
      return Variable(val.i / v.val.i);
    throw - 1;
  }
  Variable operator%(const Variable& v) {
    if (isInt && v.isInt) // int % int = int
      return Variable(val.i % v.val.i);
    throw - 1;
  }
  Variable operator==(const Variable& v)
  {
    if (isInt && v.isInt)
      return Variable(int(val.i == v.val.i));
    if (!isInt && !v.isInt)
      return Variable(int(val.d == v.val.d));
    if(isInt)
      return Variable(int(val.i == v.val.d));
    return Variable(int(val.d == v.val.i));
  }
  Variable operator!=(const Variable& v)
  {
    if (isInt && v.isInt)
      return Variable(int(val.i != v.val.i));
    if (!isInt && !v.isInt)
      return Variable(int(val.d != v.val.d));
    if (isInt)
      return Variable(int(val.i != v.val.d));
    return Variable(int(val.d != v.val.i));
  }
  Variable operator>(const Variable& v)
  {
    if (isInt && v.isInt)
      return Variable(int(val.i > v.val.i));
    if (!isInt && !v.isInt)
      return Variable(int(val.d > v.val.d));
    if (isInt)
      return Variable(int(val.i > v.val.d));
    return Variable(int(val.d > v.val.i));
  }
  Variable operator<(const Variable& v)
  {
    if (isInt && v.isInt)
      return Variable(int(val.i < v.val.i));
    if (!isInt && !v.isInt)
      return Variable(int(val.d < v.val.d));
    if (isInt)
      return Variable(int(val.i < v.val.d));
    return Variable(int(val.d < v.val.i));
  }

  friend std::ostream& operator <<(std::ostream& os, const Variable& v) {
    if (v.isInt)
      os << v.val.i;
    else
      os << v.val.d;
    return os;
  }
  friend std::istream& operator>>(std::istream& is, Variable& v) {
    std::string s; is >> s;
    try { int i = stoi(s); v.isInt = 1; v.val.i = i; }
    catch (...) { v.isInt = 0; v.val.d = stod(s); }
    return is;
  }
};
