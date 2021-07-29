#include "node_manager.h"

int
node_manager::add_node (int& node_id, std::string name, node_types n_typ) {
    int temp = 0;
	switch (n_typ) {
    case AND_GATE:
		temp = node_id;
        nodes.push_back(new and_gate{temp, name});
		add_input_pins(temp, ++node_id, "A");
		add_input_pins(temp, ++node_id, "B");
		add_output_pins(temp, ++node_id, "A.B");
        break;
    case OR_GATE:
        break;
    case NOT_GATE:
        break;
    default:
        assert(0);
        break;
    }
    return 0;
}

int
node_manager::add_input_pins (int& node_id, int pin_id, std::string pin_name) {
    for(node* n: nodes) {
        if(n->m_unique_id == node_id) {
            n->input_pins.push_back({pin_id, pin_name});
            break;
        }
    }
    return 0;
}

int
node_manager::add_output_pins (int& node_id, int pin_id, std::string pin_name) {
    for(node* n: nodes) {
        if(n->m_unique_id == node_id) {
            n->output_pins.push_back({pin_id, pin_name});
            break;
        }
    }
    return 0;
}

int
node_manager::add_link (int& m_id, int st_node, int st_pin, int ed_node, int ed_pin) {
    links.push_back({m_id, st_node, st_pin, ed_node, ed_pin});

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
    for(int n = 0; n < links.size(); ++n) {
        if(links[n].m_unique_id == l_id) {
            // bring the last element to the index to remove
            // ie overwrite the useless index
            // and pop the last one
            links[n] = links[links.size() - 1];
            links.pop_back();
            break;
        }
    }
    return 0;
}

int
node_manager::render ( ) {
    for(node* n: nodes) {
        ImNodes::BeginNode(n->m_unique_id);
        ImNodes::BeginNodeTitleBar();
        ImGui::Text("%s", n->node_name.c_str());
        ImNodes::EndNodeTitleBar();
        for(in_pin& pin: n->input_pins) {
            ImNodes::BeginInputAttribute(pin.pin_id);
            ImGui::ColorButton("", {0.5, 1, 0.25, 1}, 0, {15, 15});
            ImGui::SameLine();
            ImGui::Text("%s",pin.pin_name.c_str());
            ImNodes::EndInputAttribute();
        }
        ImGui::NewLine();
        for(out_pin& pin: n->output_pins) {
            ImNodes::BeginOutputAttribute(pin.pin_id);
            ImGui::Text("%s", pin.pin_name.c_str());
            ImGui::SameLine();
            ImGui::ColorButton("", {0.5, 1, 0.25, 1}, 0, {15, 15});
            ImNodes::EndOutputAttribute();
        }
        ImNodes::EndNode();
    }
    for(link& l : links) {
        ImNodes::Link(l.m_unique_id, l.start_pin, l.end_pin);
    }
    return 0;
}
