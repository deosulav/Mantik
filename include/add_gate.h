#ifndef ADD_GATE_
#define ADD_GATE_

#include "node.h"

class input : public node {
  public:
	input (int id, std::string name)
		: node (id, name){ }

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
		if (this->input_pins[0].value == -1 || this->input_pins[1].value == -1) {
			this->output_pins[0].value = -1;
		} else
			this->output_pins[0].value = ( bool )this->input_pins[0].value * ( bool )this->input_pins[1].value;
        return 0;
    }
};

#endif
