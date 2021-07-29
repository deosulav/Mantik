#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <imnodes/imnodes.h>

struct in_pin {
	int pin_id;
	std::string pin_name;
	int			value;
	int			counter = 0;
};

struct out_pin {
	int pin_id;
	std::string pin_name;
	int value;
	std::vector<in_pin*> connected_inputs;
};

class node {
	const int m_unique_id;
	const std::string node_name;
protected:
	std::vector<in_pin> input_pins;
	std::vector<out_pin> output_pins;
	std::vector<node*> m_inputs; //subject to removal

	virtual int calculate() = 0;
	friend class node_manager;

	node(int id, std::string name) : m_unique_id(id), node_name(name) {}
};

#endif // NODE_H
