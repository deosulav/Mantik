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

class or_gate : public node {
  public:
	or_gate (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		if (this->input_pins[0].value == -1 || this->input_pins[1].value == -1) {
			this->output_pins[0].value = -1;
		} else
			this->output_pins[0].value = (bool)(( bool )this->input_pins[0].value + ( bool )this->input_pins[1].value);
		return 0;
	}
};

class not_gate : public node {
  public:
	not_gate (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		if (this->input_pins[0].value == -1 ) {
			this->output_pins[0].value = -1;
		} else
			this->output_pins[0].value =(int)!( bool )this->input_pins[0].value ;
		return 0;
	}
};

class multiplexer8_1 : public node {
  public:
	multiplexer8_1 (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		if (this->input_pins[0].value == -1 || this->input_pins[1].value == -1 || this->input_pins[2].value == -1 ||\
			this->input_pins[3].value == -1 || this->input_pins[4].value == -1 || this->input_pins[5].value == -1 ||\
			this->input_pins[6].value == -1 || this->input_pins[7].value == -1 || this->input_pins[8].value == -1 ||\
			this->input_pins[9].value == -1 || this->input_pins[10].value == -1) {
			this->output_pins[0].value = -1;
		} else {
			this->output_pins[0].value = ( int )( 
			!( bool )this->input_pins[10].value * !( bool )this->input_pins[9].value * !( bool )this->input_pins[8].value * ( bool )this->input_pins[0].value +
			!( bool )this->input_pins[10].value * !( bool )this->input_pins[9].value *  ( bool )this->input_pins[8].value * ( bool )this->input_pins[1].value +
			!( bool )this->input_pins[10].value *  ( bool )this->input_pins[9].value * !( bool )this->input_pins[8].value * ( bool )this->input_pins[2].value +
			!( bool )this->input_pins[10].value *  ( bool )this->input_pins[9].value *  ( bool )this->input_pins[8].value * ( bool )this->input_pins[3].value +
			 ( bool )this->input_pins[10].value * !( bool )this->input_pins[9].value * !( bool )this->input_pins[8].value * ( bool )this->input_pins[4].value +
			 ( bool )this->input_pins[10].value * !( bool )this->input_pins[9].value *  ( bool )this->input_pins[8].value * ( bool )this->input_pins[5].value +
			 ( bool )this->input_pins[10].value *  ( bool )this->input_pins[9].value * !( bool )this->input_pins[8].value * ( bool )this->input_pins[6].value +
			 ( bool )this->input_pins[10].value *  ( bool )this->input_pins[9].value *  ( bool )this->input_pins[8].value * ( bool )this->input_pins[7].value);
		}
		return 0;
	}
};
#endif
