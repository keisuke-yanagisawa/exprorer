#include <set>

#include "problem.hpp"
#include "state.hpp"

#ifndef BEASLEY1990_HPP_
#define BEASLEY1990_HPP_

state primal_dual(const problem& pr);
state primal_dual(const problem& pr, state& st);

#endif
