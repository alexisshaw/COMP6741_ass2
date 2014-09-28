#pragma once

#include <vector>
#include <stack>

#include "kernelisation/types.h"

bool  branch(Graph g, int k, bool doKern, vector<stack<GraphModifier, vector<GraphModifier>>>& stacks);