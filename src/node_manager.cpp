#include "node_manager.h"
#include<typeinfo>
#include<iostream>
int
node_manager::add_node (int& node_id, std::string name, node_types n_typ) {
    int temp = 0;
	switch (n_typ) {
	case INPUT_1: case INPUT_2: case INPUT_3: case INPUT_4: case INPUT_5: case INPUT_6: case INPUT_7: case INPUT_8 :
		temp = node_id;
		nodes.push_back (new and_gate{temp, "Input"});
		for ( int iter=1;iter<=n_typ;iter++)
			add_output_pins (temp, ++node_id, "I", 0);
		break;
    case AND_GATE:
		temp = node_id;
		nodes.push_back (new and_gate{temp, "AND Gate"});
		add_input_pins(temp, ++node_id, "A",-1);
		add_input_pins(temp, ++node_id, "B",-1);
		add_output_pins(temp, ++node_id, "A.B",-1);
        break;
    case OR_GATE:
		temp = node_id;
		nodes.push_back (new or_gate{temp, "OR Gate"});
		add_input_pins (temp, ++node_id, "A", -1);
		add_input_pins (temp, ++node_id, "B", -1);
		add_output_pins (temp, ++node_id, "A+B", -1);
        break;
    case NOT_GATE:
		temp = node_id;
		nodes.push_back (new not_gate{temp, "NOT Gate"});
		add_input_pins (temp, ++node_id, "A", -1);
		add_output_pins (temp, ++node_id, "A'", -1);
        break;
	case MULTIPLEXER8_1:
		temp = node_id;
		nodes.push_back (new multiplexer8_1{temp, "Multiplexer 8_1"});
		add_input_pins (temp, ++node_id, "D0", -1);
		add_input_pins (temp, ++node_id, "D1", -1);
		add_input_pins (temp, ++node_id, "D2", -1);
		add_input_pins (temp, ++node_id, "D3", -1);
		add_input_pins (temp, ++node_id, "D4", -1);
		add_input_pins (temp, ++node_id, "D5", -1);
		add_input_pins (temp, ++node_id, "D6", -1);
		add_input_pins (temp, ++node_id, "D7", -1);
		add_input_pins (temp, ++node_id, "S0", -1);
		add_input_pins (temp, ++node_id, "S1", -1);
		add_input_pins (temp, ++node_id, "S2", -1);
		add_output_pins (temp, ++node_id, "Y", -1);
		break;
    default:
        assert(0);
        break;
    }
    return 0;
}

int
node_manager::add_input_pins (int& node_id, int pin_id, std::string pin_name,int value) {
    for(node* n: nodes) {
        if(n->m_unique_id == node_id) {
            n->input_pins.push_back({pin_id, pin_name,value});
            break;
        }
    }
    return 0;
}

int
node_manager::add_output_pins (int& node_id, int pin_id, std::string pin_name,int value) {
    for(node* n: nodes) {
        if(n->m_unique_id == node_id) {
            n->output_pins.push_back({pin_id, pin_name,value});
            break;
        }
    }
    return 0;
}

int
node_manager::add_link (int& m_id, int st_node, int st_pin, int ed_node, int ed_pin) {
	for (node* n : nodes) {
		if (n->m_unique_id == st_node) {
			for (out_pin& opin : n->output_pins) {
				if (opin.pin_id == st_pin) { // identify start/output pin
					for (node* nk : nodes) { // reiterate to find end/input pins
						if (nk->m_unique_id == ed_node) {
							for (in_pin& ipin : nk->input_pins) {
								if (ipin.pin_id == ed_pin) // checking id
									if (ipin.counter == 0) {
										opin.connected_inputs.push_back (&ipin), ipin.counter++;
										links.push_back ({m_id, st_node, st_pin, ed_node, ed_pin});
									}
							}
						}
					}
				}
			}
		}
	}
    node* temp;
    for(node* n: nodes) {
        if(n->m_unique_id == st_node) {
            temp = n;
        }
    }
    for(node* n: nodes) {
        if(n->m_unique_id == ed_node) {
            n->m_inputs.push_back(temp);
        }
    }
    return 0;
}

int
node_manager::remove_link (int l_id) {
	in_pin* temp;
    for(int n = 0; n < links.size(); ++n) {
        if(links[n].m_unique_id == l_id) {
            // bring the last element to the index to remove
            // ie overwrite the useless index
            // and pop the last one

            
			// link remove garda connected input hataune out pin bata //code for that starts here
			for (node* no : nodes) {
				if (no->m_unique_id == links[n].end_node) {
					for (in_pin& pin : no->input_pins) {
						if (pin.pin_id == links[n].end_pin) {
							pin.value = -1; // default condition
							if (pin.counter > 0)
								pin.counter = 0;
							temp = &pin;
							for (node* nod : nodes) {
								for (out_pin& opin : nod->output_pins) { // similar logic like above
									for (int i = 0; i < opin.connected_inputs.size ( ); i++) {
										if (opin.connected_inputs[i] == temp) {
											opin.connected_inputs[i] =
												opin.connected_inputs[opin.connected_inputs.size ( ) - 1];
											opin.connected_inputs.pop_back ( );
											break; // I'm not sure which loops it breaks but meh as long as it works
										}
									}
								}
							}
						}
					}
				}
			}
			// ends here

            links[n] = links[links.size() - 1];
            links.pop_back();
            break;
        }
    }
    return 0;
}

int
node_manager::render ( ) {
	ImVec4 color = {0.36f, 0.36f, 0.36f, 1.0f};
	bool   flag	 = false; // flag to check if input pins/out pins exist
	for (node* n : nodes) {

		ImNodes::BeginNode (n->m_unique_id);
		ImNodes::BeginNodeTitleBar ( );
		ImGui::Text ("%s", n->node_name.c_str ( ));
		ImNodes::EndNodeTitleBar ( );
		for (in_pin& pin : n->input_pins) {
			if (pin.value == 0) // checking value and render proper color
				color = {0.5f, 1.0f, 0.25f, 1.0f};
			if (pin.value == 1)
				color = {0.992f, 0.266f, 0.423f, 1.0f};
			if (pin.value == -1)
				color = {0.36f, 0.36f, 0.36f, 1.0f};
			ImNodes::BeginInputAttribute (pin.pin_id);
			ImGui::ColorButton ("1", color, ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImGui::SameLine ( );
			ImGui::Text ("%s", pin.pin_name.c_str ( ));
			ImNodes::EndInputAttribute ( );
			flag = true;
		}
		ImGui::NewLine ( );
		for (out_pin& pin : n->output_pins) {
			if (pin.value == 0)
				color = {0.5f, 1.0f, 0.25f, 1.0f};
			if (pin.value == 1)
				color = {0.992f, 0.266f, 0.423f, 1.0f};
			if (pin.value == -1)
				color = {0.36f, 0.36f, 0.36f, 1.0f};
			ImNodes::BeginOutputAttribute (pin.pin_id);
			if (flag == false) {						  // if input pin doesn't exist i.e for input node
				if (ImGui::Button ("", {15, 15})) {		  // create a button and check if its pressed
					pin.value = ( int )!bool (pin.value); // flip 0 and 1 //could use review as I'm unsure how safe it
														  // is
				}
			}
			ImGui::SameLine ( );
			flag = false; // formality sake
			ImGui::Indent ( );
			ImGui::Text ("%s", pin.pin_name.c_str ( ));
			ImGui::SameLine ( );
			ImGui::ColorButton ("", color, ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImNodes::EndOutputAttribute ( );
		}
		ImNodes::EndNode ( );
	}
	for (link& l : links) {
		ImNodes::Link (l.m_unique_id, l.start_pin, l.end_pin);
	}
    return 0;
}

int
node_manager::copyover ( ) {
	for (node* n : nodes) {
		for (out_pin& opin : n->output_pins) {
			for (int i = 0; i < opin.connected_inputs.size ( ); i++) { // pretty basic
				opin.connected_inputs[i]->value = opin.value;
			}
		}
	}
	return 0;
}

int
node_manager::calculate ( ) {
	for (node* n : nodes) {
		if (n->node_name != "Input") { //input ko calculation block bolauda vertex out of bound jaanxa tesaile, not quite sure why
			n->calculate ( );
		}
	}
	return 0;
}