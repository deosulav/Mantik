#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <imnodes/imnodes.h>

struct in_pin {
	int pin_id;
	std::string pin_name;
};

struct out_pin {
	int pin_id;
	std::string pin_name;
	int value;
};

class node {
	const int m_unique_id;
	int m_logic_status;
	const std::string node_name;
	std::vector<in_pin> input_pins;
	std::vector<out_pin> output_pins;
	std::vector<node*> m_inputs;

	node(int id, std::string name) : m_unique_id(id), node_name(name) {}

	friend class node_manager;
};

#endif // NODE_H
