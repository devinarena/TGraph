
/**
 * @file builtins.h
 * @author Devin Arena
 * @brief Implementation for builtin functions e.g. sin, cos, etc and constants (PI, e).
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
  } else if (fname == "sqrt") {
    return &tg_sqrt;
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