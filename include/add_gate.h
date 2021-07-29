#ifndef ADD_GATE_
#define ADD_GATE_

#include "node.h"

class input : public node {
  public:
	input (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		return 0;
	}
};

class and_gate: public node {
public:
	and_gate(int id, std::string name)
        :node(id, name)
    {
    }

	int calculate() {
        return 0;
    }
};

#endif
