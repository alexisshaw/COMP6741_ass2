#include "types.h"
#include <stack>

using namespace std;

namespace GraphReduction {
	//! Performs kernelisation on (G,k) as per RamanS08 and returns (false,0) if it is a NO instance, otherwise (true,m) where there have been m graph modifications
	pair<bool, unsigned> kernelize(Graph& g, int k, stack<GraphModifier>& modifier_stack);

	//! Undoes kernelisation by popping off the top m modifications and restoring the graph.
	void unkernelize(Graph& g, stack<GraphModifier>& modifier_stack, unsigned m);
}