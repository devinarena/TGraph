
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
  return std::sqrt(x);
}

static BuiltinFunc resolveFunction(std::string fname) {
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

#endif