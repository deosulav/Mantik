#include "node_manager.h"
#include<typeinfo>
#include<iostream>

std::string tempo;

int
node_manager::add_node (int& node_id, std::string name, node_types n_typ) {
    int temp = 0;
	if (n_typ>=INPUT_1 && n_typ<=INPUT_8){
		temp = node_id;
		nodes.push_back (new and_gate{temp, "Input"});
		for ( int iter=0;iter<=(n_typ-INPUT_1);iter++)
			add_output_pins (temp, ++node_id, "I", 0);
	}
	else if (n_typ >= AND_GATE_2 && n_typ <= AND_GATE_8) {
		temp = node_id;
		nodes.push_back (new and_gate{temp, "AND Gate"});
		for (int iter = 0; iter <= (n_typ-AND_GATE_2+1); iter++)
			add_input_pins (temp, ++node_id, "I", -1);
		add_output_pins(temp, ++node_id, "O",-1);
	}
	else if (n_typ >= OR_GATE_2 && n_typ <= OR_GATE_8) {
		temp = node_id;
		nodes.push_back (new or_gate{temp, "OR Gate"});
		for (int iter = 0; iter <= (n_typ - OR_GATE_2 + 1); iter++) {
			add_input_pins (temp, ++node_id, "I", -1);
		}
		add_output_pins (temp, ++node_id, "O", -1);
	}
	else if (n_typ==NOT_GATE){
		temp = node_id;
		nodes.push_back (new not_gate{temp, "NOT Gate"});
		add_input_pins (temp, ++node_id, "A", -1);
		add_output_pins (temp, ++node_id, "A'", -1);
	} 
	else if (n_typ >= MULTIPLEXER8_1 && n_typ <= MULTIPLEXER2_1) {
		temp = node_id;
		int inputno;
		int selectorno;
		switch (n_typ) {
		case MULTIPLEXER8_1:
			nodes.push_back (new multiplexer{temp, "Multiplexer 8_1"});
			inputno = 8, selectorno = 3;
			break;
		case MULTIPLEXER4_1:
			nodes.push_back (new multiplexer{temp, "Multiplexer 4_1"});
			inputno = 4, selectorno = 2;
			break;
		case MULTIPLEXER2_1:
			nodes.push_back (new multiplexer{temp, "Multiplexer 2_1"});
			inputno = 2, selectorno = 1;
			break;
		default:
			assert (0);
			break;
		}
		for (int iter = 0; iter < inputno; iter++)
			add_input_pins (temp, ++node_id, "D", -1);
		for (int iter = 0; iter < selectorno; iter++)
			add_input_pins (temp, ++node_id, "S", -1);
		add_output_pins (temp, ++node_id, "Y", -1);
	} 
	else if (n_typ >= DEMULTIPLEXER1_8 && n_typ <= DEMULTIPLEXER1_2) {
		temp = node_id;
		int outputno;
		int selectorno;
		switch (n_typ) {
		case DEMULTIPLEXER1_8:
			nodes.push_back (new demultiplexer{temp, "DeMultiplexer 1_8"});
			outputno = 8, selectorno = 3;
			break;
		case DEMULTIPLEXER1_4:
			nodes.push_back (new demultiplexer{temp, "DeMultiplexer 1_4"});
			outputno = 4, selectorno = 2;
			break;
		case DEMULTIPLEXER1_2:
			nodes.push_back (new demultiplexer{temp, "DeMultiplexer 1_2"});
			outputno = 2, selectorno = 1;
			break;
		default:
			assert (0);
			break;
		}
		add_input_pins (temp, ++node_id, "D", -1);
		for (int iter = 0; iter < selectorno; iter++)
			add_input_pins (temp, ++node_id, "S", -1);
		for (int iter = 0; iter < outputno; iter++)
			add_output_pins (temp, ++node_id, "Y", -1);
	} 
	else if (n_typ >= DECODER3_8 && n_typ <= DECODER1_2) {
		temp = node_id;
		int outputno;
		int inputno;
		switch (n_typ) {
		case DECODER3_8:
			nodes.push_back (new decoder{temp, "Decoder 3_8"});
			outputno = 8, inputno = 3;
			break;
		case DECODER2_4:
			nodes.push_back (new decoder{temp, "Decoder 2_4"});
			outputno = 4, inputno = 2;
			break;
		case DECODER1_2:
			nodes.push_back (new decoder{temp, "Decoder 1_2"});
			outputno = 2, inputno = 1;
			break;
		default:
			assert (0);
			break;
		}
		for (int iter = 0; iter < inputno; iter++)
			add_input_pins (temp, ++node_id, "S", -1);
		for (int iter = 0; iter < outputno; iter++)
			add_output_pins (temp, ++node_id, "Y", -1);
	} 
	else {
		assert (0);
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
		if (flag==true) ImGui::NewLine ( );
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
					pin.value = ( int )!bool (pin.value); // flip 0 and 1 //could use review as I'm unsure how safe it is
				}
				ImGui::SameLine ( );
			}
			ImGui::Indent ( );
			ImGui::Text ("%s", pin.pin_name.c_str ( ));
			ImGui::SameLine ( );
			ImGui::ColorButton ("", color, ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImNodes::EndOutputAttribute ( );
		}
		ImNodes::EndNode ( );
			flag = false; // formality sake
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