#ifndef NODE_MANAGER_H_
#define NODE_MANAGER_H_

#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "link.h"
#include "node.h"
#include "node_types.h"
#include "add_gate.h"

class node_manager {
private:
	std::vector<node*> nodes;
	std::vector<link> links;

public:
	int
	add_node (int& node_id, std::string name, node_types n_typ);

	int
	add_input_pins (int& node_id, int pin_id, std::string pin_name);

	int
	add_output_pins (int& node_id, int pin_id, std::string pin_name);

	int
	add_link (int& m_id,
			int st_node,
			int st_pin,
			int ed_node,
		    int ed_pin
			  );

	int
	remove_link (int const);

	int
	render ( );
};

#endif // NODE_MANAGER_H_
