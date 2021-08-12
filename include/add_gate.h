#ifndef ADD_GATE_
#define ADD_GATE_

#include "node.h"
static int mapvalues ( bool s0 , bool s1 , bool s2) {
	if (s0 == 0 && s1 == 0 && s2 == 0)
		return 0;
	if (s0 == 1 && s1 == 0 && s2 == 0)
		return 1;
	if (s0 == 0 && s1 == 1 && s2 == 0)
		return 2;
	if (s0 == 1 && s1 == 1 && s2 == 0)
		return 3;
	if (s0 == 0 && s1 == 0 && s2 == 1)
		return 4;
	if (s0 == 1 && s1 == 0 && s2 == 1)
		return 5;
	if (s0 == 0 && s1 == 1 && s2 == 1)
		return 6;
	if (s0 == 1 && s1 == 1 && s2 == 1)
		return 7;
}
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
		bool temp = true;
		 bool checkinvalid = false;
		 for (in_pin& ipin : this->input_pins) {
			 if (ipin.value == -1) {
				 checkinvalid = true;
				 break;
			 }
		 }
		if (checkinvalid) {
			this->output_pins[0].value = -1;
		} else {
			for (in_pin& ipin : this->input_pins) {
				temp = temp * (bool) ipin.value;
			}
			this->output_pins[0].value = temp;
		}
        return 0;
    }
};

class or_gate : public node {
  public:
	or_gate (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		bool temp		  = false;
		bool checkinvalid = false;
		for (in_pin& ipin : this->input_pins) {
			if (ipin.value == -1) {
				checkinvalid = true;
				break;
			}
		}
		if (checkinvalid) {
			this->output_pins[0].value = -1;
		} else {
			for (in_pin& ipin : this->input_pins) {
				temp = temp + ( bool )ipin.value;
			}
			this->output_pins[0].value = temp;
		}
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

class multiplexer : public node {
  public:
	multiplexer (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		bool temp		  = false;
		bool checkinvalid = false;
		for (in_pin& ipin : this->input_pins) {
			if (ipin.value == -1) {
				checkinvalid = true;
				break;
			}
		}
		if (checkinvalid) {
			this->output_pins[0].value = -1;
		} else {
			if (this->input_pins.size ( ) == 11) {
				this->output_pins[0].value=
					this->input_pins[mapvalues (this->input_pins[8].value, this->input_pins[9].value, this->input_pins[10].value )].value;
			}
			if (this->input_pins.size ( ) == 6) {
				this->output_pins[0].value=
					this->input_pins[mapvalues (this->input_pins[4].value, this->input_pins[5].value, 0)].value;
			}
			if (this->input_pins.size ( ) == 3) {
				this->output_pins[0].value=
					this->input_pins[mapvalues (this->input_pins[2].value, 0, 0)].value;
			}
		}
		return 0;
	}
};
class demultiplexer : public node {
  public:
	demultiplexer (int id, std::string name)
		: node (id, name) {}

	int
	calculate ( ) {
		bool temp		  = false;
		bool checkinvalid = false;
		for (in_pin& ipin : this->input_pins) {
			if (ipin.value == -1) {
				checkinvalid = true;
				break;
			}
		}
		if (checkinvalid) {
			for (out_pin& opin : this->output_pins) {
				opin.value = -1;
			}
		} else {
			for (out_pin& opin : this->output_pins) {
				opin.value = 0;
			}
			if (this->input_pins.size ( ) == 4) {
				this->output_pins[mapvalues(this->input_pins[1].value, this->input_pins[2].value, this->input_pins[3].value)].value =
					this->input_pins[0].value;
			}
			if (this->input_pins.size ( ) == 3) {
				this->output_pins[mapvalues (this->input_pins[1].value, this->input_pins[2].value, 0)].value =
					this->input_pins[0].value;
			}
			if (this->input_pins.size ( ) == 2) {
				this->output_pins[mapvalues (this->input_pins[1].value, 0 , 0)].value =
					this->input_pins[0].value;
			}
		}
		return 0;
	}
};
#endif
