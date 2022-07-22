
/**
 * @file builtins.h
 * @author Devin Arena
 * @brief Implementation for builtin functions e.g. sin, cos, etc.
 * @since 7/21/2022
 **/

#ifndef TGRAPH_BUILTINS_H
#define TGRAPH_BUILTINS_H

#include <cmath>
#include <string>

typedef double (*BuiltinFunc)(double);

static double tg_sin(double angle) {
  return std::sin(angle);
}

static double tg_cos(double angle) {
  return std::cos(angle);
}

static double tg_tan(double angle) {
  return std::tan(angle);
}

static double tg_sqrt(double x) {
  if (x < 0)
    return INT_MIN;
  return std::sqrt(x);
}

static inline BuiltinFunc resolveFunction(std::string fname) {
  if (fname == "sin") {
    return &tg_sin;
  } else if (fname == "cos") {
    return &tg_cos;
  } else if (fname == "tan") {
    return &tg_tan;
  } else if (fname == "sqrt") {
    return &tg_sqrt;
  } else {
    return nullptr;
  }
}

static inline double resolveConstant(std::string cname) {
  if (cname == "pi") {
    return M_PI;
  } else if (cname == "e") {
    return M_E;
  } else {
    return 0;
  }
}

#endif