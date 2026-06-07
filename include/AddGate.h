#ifndef ADD_GATE_
#define ADD_GATE_

#include "Node.h"

static int mapValues(bool s0, bool s1, bool s2) {
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
	return 7;
}

static bool checkValid(Node* node) {
	for (InPin& iPin : node->inputPins) {
		if (iPin.value == -1) {
			return false;
		}
	}
	return true;
}

class Input : public Node {
  public:
	Input(int id, std::string name)
		: Node(id, name) {}

	void calculate() {}
};

class Output : public Node {
  public:
	Output(int id, std::string name)
		: Node(id, name) {}

	void calculate() {}
};

class AndGate : public Node {
  public:
	AndGate(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			this->outputPins[0].value = -1;
			return;
		}
		bool temp = true;
		for (InPin& iPin : this->inputPins) {
			temp = temp * ( bool )iPin.value;
		}
		this->outputPins[0].value = temp;
	}
};

class OrGate : public Node {
  public:
	OrGate(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			this->outputPins[0].value = -1;
			return;
		}
		bool temp = false;
		for (InPin& iPin : this->inputPins) {
			temp = temp + ( bool )iPin.value;
		}
		this->outputPins[0].value = temp;
	}
};

class NotGate : public Node {
  public:
	NotGate(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			this->outputPins[0].value = -1;
			return;
		}
		this->outputPins[0].value = ( int )!( bool )this->inputPins[0].value;
	}
};

class Encoder : public Node {
  public:
	Encoder(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			for (OutPin& oPin : this->outputPins) {
				oPin.value = -1;
			}
			return;
		}
		if (this->inputPins.size() == 8) {
			this->outputPins[0].value = ( int )( bool )(this->inputPins[7].value + this->inputPins[5].value +
														this->inputPins[3].value + this->inputPins[1].value);
			this->outputPins[1].value = ( int )( bool )(this->inputPins[7].value + this->inputPins[6].value +
														this->inputPins[3].value + this->inputPins[2].value);
			this->outputPins[2].value = ( int )( bool )(this->inputPins[7].value + this->inputPins[6].value +
														this->inputPins[5].value + this->inputPins[4].value);
		}
		if (this->inputPins.size() == 4) {
			this->outputPins[0].value = ( int )( bool )(this->inputPins[3].value + this->inputPins[1].value);
			this->outputPins[1].value = ( int )( bool )(this->inputPins[3].value + this->inputPins[2].value);
		}
		if (this->inputPins.size() == 2) {
			this->outputPins[0].value = this->inputPins[1].value;
		}
	}
};

class Decoder : public Node {
  public:
	Decoder(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			for (OutPin& oPin : this->outputPins) {
				oPin.value = -1;
			}
			return;
		}

		for (OutPin& oPin : this->outputPins) {
			oPin.value = 0;
		}
		if (this->inputPins.size() == 3) {
			this->outputPins[mapValues(this->inputPins[0].value, this->inputPins[1].value, this->inputPins[2].value)]
				.value = 1;
		}
		if (this->inputPins.size() == 2) {
			this->outputPins[mapValues(this->inputPins[0].value, this->inputPins[1].value, 0)].value = 1;
		}
		if (this->inputPins.size() == 1) {
			this->outputPins[mapValues(this->inputPins[0].value, 0, 0)].value = 1;
		}
	}
};

class Multiplexer : public Node {
  public:
	Multiplexer(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			this->outputPins[0].value = -1;
			return;
		}

		if (this->inputPins.size() == 11) {
			this->outputPins[0].value =
				this->inputPins[mapValues(
									this->inputPins[8].value, this->inputPins[9].value, this->inputPins[10].value)]
					.value;
		}
		if (this->inputPins.size() == 6) {
			this->outputPins[0].value =
				this->inputPins[mapValues(this->inputPins[4].value, this->inputPins[5].value, 0)].value;
		}
		if (this->inputPins.size() == 3) {
			this->outputPins[0].value = this->inputPins[mapValues(this->inputPins[2].value, 0, 0)].value;
		}
	}
};

class Demultiplexer : public Node {
  public:
	Demultiplexer(int id, std::string name)
		: Node(id, name) {}

	void calculate() {
		if (!checkValid(this)) {
			for (OutPin& oPin : this->outputPins) {
				oPin.value = -1;
			}
			return;
		}

		for (OutPin& oPin : this->outputPins) {
			oPin.value = 0;
		}
		if (this->inputPins.size() == 4) {
			this->outputPins[mapValues(this->inputPins[1].value, this->inputPins[2].value, this->inputPins[3].value)]
				.value = this->inputPins[0].value;
		}
		if (this->inputPins.size() == 3) {
			this->outputPins[mapValues(this->inputPins[1].value, this->inputPins[2].value, 0)].value =
				this->inputPins[0].value;
		}
		if (this->inputPins.size() == 2) {
			this->outputPins[mapValues(this->inputPins[1].value, 0, 0)].value = this->inputPins[0].value;
		}
	}
};
#endif
