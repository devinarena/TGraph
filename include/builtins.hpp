
/**
 * @file builtins.h
 * @author Devin Arena
 * @brief Implementation for builtin functions e.g. sin, cos, etc and constants
 *(PI, e).
 * @since 7/21/2022
 **/

#ifndef TGRAPH_BUILTINS_H
#define TGRAPH_BUILTINS_H

#include <cmath>
#include <string>

/**
 * @brief Define the builtin functions.
 */
typedef double (*BuiltinFunc)(double);

/**
 * @brief Builtin sin function, simply wraps over C++ sin.
 *
 * @param angle the angle to take the sin of.
 * @return double the sin of the angle.
 */
static double tg_sin(double angle) {
  return std::sin(angle);
}

/**
 * @brief Builtin cos function, simply wraps over C++ cos.
 *
 * @param angle the angle to take the cos of.
 * @return double the cos of the angle.
 */
static double tg_cos(double angle) {
  return std::cos(angle);
}

/**
 * @brief Builtin tan function, simply wraps over C++ tan.
 *
 * @param angle the angle to take the tan of.
 * @return double the tan of the angle.
 */
static double tg_tan(double angle) {
  return std::tan(angle);
}

/**
 * @brief Builtin sec function, simply wraps over C++ cos.
 *
 * @param angle the angle to take the tan of.
 * @return double the tan of the angle.
 */
static double tg_sec(double angle) {
  double cos = std::cos(angle);
  if (cos == 0)
    return INT_MIN;
  return 1 / cos;
}

/**
 * @brief Builtin sec function, simply wraps over C++ cos.
 *
 * @param angle the angle to take the tan of.
 * @return double the tan of the angle.
 */
static double tg_csc(double angle) {
  double sin = std::sin(angle);
  if (sin == 0)
    return INT_MIN;
  return 1 / sin;
}

/**
 * @brief Builtin cot function, simply wraps over C++ tan.
 *
 * @param angle the angle to take the tan of.
 * @return double the tan of the angle.
 */
static double tg_cot(double angle) {
  double tan = std::tan(angle);
  if (tan == 0)
    return INT_MIN;
  return 1 / tan;
}

/**
 * @brief Builtin sqrt function, simply wraps over C++ sqrt.
 *
 * @param angle the angle to take the sqrt of.
 * @return double the sqrt of the angle.
 */
static double tg_sqrt(double x) {
  if (x < 0)
    return INT_MIN;
  return std::sqrt(x);
}

/**
 * @brief Builtin natural logarithm, uses change of base formula.
 *
 * @param x the number to take the natural logarithm of
 * @return double the natural logarithm
 */
static double tg_ln(double x) {
  if (x <= 0)
    return INT_MIN;
  return std::log(x) / std::log(M_E);
}

/**
 * @brief Resolves the builtin function name to the function pointer.
 *
 * @param fname the name of the function.
 * @return BuiltinFunc the function pointer.
 */
static inline BuiltinFunc resolveFunction(std::string fname) {
  if (fname == "sin") {
    return &tg_sin;
  } else if (fname == "cos") {
    return &tg_cos;
  } else if (fname == "tan") {
    return &tg_tan;
  } else if (fname == "sec") {
    return &tg_sec;
  } else if (fname == "csc") {
    return &tg_csc;
  } else if (fname == "cot") {
    return &tg_cot;
  } else if (fname == "sqrt") {
    return &tg_sqrt;
  } else if (fname == "ln") {
    return &tg_ln;
  } else {
    return nullptr;
  }
}

/**
 * @brief Resolves the constant name to the constant value.
 *
 * @param cname the name of the constant.
 * @return double the value of the constant.
 */
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