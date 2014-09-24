#pragma once

#include <vector>
#include <stack>

#include "kernelisation/types.h"

bool branch(Graph g, int k, bool doKern, std::vector<std::stack<GraphModifierType, std::vector<GraphModifierType>>>& stacks);