#ifndef ADD_GATE_
#define ADD_GATE_

#include "node.h"

class and_gate: public node {
public:
	and_gate(int id, std::string name);

	int calculate();
};

#endif
