#include "NodeManager.h"
#include <iostream>
#include <typeinfo>

std::string tempo;

int NodeManager::addNode(int& id, std::string name, NodeType type) {
	int temp = 0;
	if (type >= INPUT_1 && type <= INPUT_8) {
		temp = id;
		nodes.push_back(new AndGate{temp, "Input"});
		for (int iter = 0; iter <= (type - INPUT_1); iter++)
			addOuputPin(temp, ++id, "I" + (std::to_string(iter)), 0);
	}

	else if (type == OUTPUT) {
		temp = id;
		nodes.push_back(new Output{temp, "Output"});
		addInputPin(temp, ++id, "", -1);
	}

	else if (type >= AND_GATE_2 && type <= AND_GATE_8) {
		temp = id;
		nodes.push_back(new AndGate{temp, "AND Gate"});
		for (int iter = 0; iter <= (type - AND_GATE_2 + 1); iter++)
			addInputPin(temp, ++id, "I" + (std::to_string(iter)), -1);
		addOuputPin(temp, ++id, "O", -1);
	}

	else if (type >= OR_GATE_2 && type <= OR_GATE_8) {
		temp = id;
		nodes.push_back(new OrGate{temp, "OR Gate"});
		for (int iter = 0; iter <= (type - OR_GATE_2 + 1); iter++) {
			addInputPin(temp, ++id, "I" + (std::to_string(iter)), -1);
		}
		addOuputPin(temp, ++id, "O", -1);
	}

	else if (type == NOT_GATE) {
		temp = id;
		nodes.push_back(new NotGate{temp, "NOT Gate"});
		addInputPin(temp, ++id, "A", -1);
		addOuputPin(temp, ++id, "A'", -1);
	}

	else if (type >= ENCODER8_3 && type <= ENCODER2_1) {
		temp = id;
		int outputno;
		int inputno;
		switch (type) {
		case ENCODER8_3:
			nodes.push_back(new Encoder{temp, "Encoder 8_3"});
			outputno = 3, inputno = 8;
			break;
		case ENCODER4_2:
			nodes.push_back(new Encoder{temp, "Encoder 4_2"});
			outputno = 2, inputno = 4;
			break;
		case ENCODER2_1:
			nodes.push_back(new Encoder{temp, "Encoder 2_1"});
			outputno = 1, inputno = 2;
			break;
		default:
			assert(0);
			break;
		}
		for (int iter = 0; iter < inputno; iter++)
			addInputPin(temp, ++id, "S" + (std::to_string(iter)), -1);
		for (int iter = 0; iter < outputno; iter++)
			addOuputPin(temp, ++id, "Y" + (std::to_string(iter)), -1);
	}

	else if (type >= DECODER3_8 && type <= DECODER1_2) {
		temp = id;
		int outputno;
		int inputno;
		switch (type) {
		case DECODER3_8:
			nodes.push_back(new Decoder{temp, "Decoder 3_8"});
			outputno = 8, inputno = 3;
			break;
		case DECODER2_4:
			nodes.push_back(new Decoder{temp, "Decoder 2_4"});
			outputno = 4, inputno = 2;
			break;
		case DECODER1_2:
			nodes.push_back(new Decoder{temp, "Decoder 1_2"});
			outputno = 2, inputno = 1;
			break;
		default:
			assert(0);
			break;
		}
		for (int iter = 0; iter < inputno; iter++)
			addInputPin(temp, ++id, "S" + (std::to_string(iter)), -1);
		for (int iter = 0; iter < outputno; iter++)
			addOuputPin(temp, ++id, "Y" + (std::to_string(iter)), -1);
	}

	else if (type >= MULTIPLEXER8_1 && type <= MULTIPLEXER2_1) {
		temp = id;
		int inputno;
		int selectorno;
		switch (type) {
		case MULTIPLEXER8_1:
			nodes.push_back(new Multiplexer{temp, "Multiplexer 8_1"});
			inputno = 8, selectorno = 3;
			break;
		case MULTIPLEXER4_1:
			nodes.push_back(new Multiplexer{temp, "Multiplexer 4_1"});
			inputno = 4, selectorno = 2;
			break;
		case MULTIPLEXER2_1:
			nodes.push_back(new Multiplexer{temp, "Multiplexer 2_1"});
			inputno = 2, selectorno = 1;
			break;
		default:
			assert(0);
			break;
		}
		for (int iter = 0; iter < inputno; iter++)
			addInputPin(temp, ++id, "D" + (std::to_string(iter)), -1);
		for (int iter = 0; iter < selectorno; iter++)
			addInputPin(temp, ++id, "S" + (std::to_string(iter)), -1);
		addOuputPin(temp, ++id, "Y", -1);
	}

	else if (type >= DEMULTIPLEXER1_8 && type <= DEMULTIPLEXER1_2) {
		temp = id;
		int outputno;
		int selectorno;
		switch (type) {
		case DEMULTIPLEXER1_8:
			nodes.push_back(new Demultiplexer{temp, "DeMultiplexer 1_8"});
			outputno = 8, selectorno = 3;
			break;
		case DEMULTIPLEXER1_4:
			nodes.push_back(new Demultiplexer{temp, "DeMultiplexer 1_4"});
			outputno = 4, selectorno = 2;
			break;
		case DEMULTIPLEXER1_2:
			nodes.push_back(new Demultiplexer{temp, "DeMultiplexer 1_2"});
			outputno = 2, selectorno = 1;
			break;
		default:
			assert(0);
			break;
		}
		addInputPin(temp, ++id, "D", -1);
		for (int iter = 0; iter < selectorno; iter++)
			addInputPin(temp, ++id, "S" + (std::to_string(iter)), -1);
		for (int iter = 0; iter < outputno; iter++)
			addOuputPin(temp, ++id, "Y" + (std::to_string(iter)), -1);
	} else {
		assert(0);
	}
	return 0;
}

int NodeManager::addInputPin(int& nodeId, int id, std::string name, int value) {
	for (Node* n : nodes) {
		if (n->uniqueId == nodeId) {
			n->inputPins.push_back({id, name, value});
			break;
		}
	}
	return 0;
}

int NodeManager::addOuputPin(int& nodeId, int id, std::string name, int value) {
	for (Node* n : nodes) {
		if (n->uniqueId == nodeId) {
			n->outputPins.push_back({id, name, value});
			break;
		}
	}
	return 0;
}

int NodeManager::addLink(int& id, int startNode, int startPin, int endNode, int endPin) {
	for (Node* n : nodes) {
		if (n->uniqueId == startNode) {
			for (OutPin& oPin : n->outputPins) {
				if (oPin.id == startPin) {	 // identify start/output pin
					for (Node* nk : nodes) { // reiterate to find end/input pins
						if (nk->uniqueId == endNode) {
							for (InPin& iPin : nk->inputPins) {
								if (iPin.id == endPin) // checking id
									if (iPin.counter == 0) {
										oPin.connectedInputs.push_back(&iPin), iPin.counter++;
										links.push_back({id, startNode, startPin, endNode, endPin});
									}
							}
						}
					}
				}
			}
		}
	}
	Node* temp;
	for (Node* n : nodes) {
		if (n->uniqueId == startNode) {
			temp = n;
		}
	}
	for (Node* n : nodes) {
		if (n->uniqueId == endNode) {
			n->mInputs.push_back(temp);
		}
	}
	return 0;
}

int NodeManager::removeLink(int id) {
	InPin* temp;
	for (int n = 0; n < links.size(); ++n) {
		if (links[n].uniqueId == id) {
			// bring the last element to the index to remove
			// ie overwrite the useless index
			// and pop the last one


			// link remove garda connected input hataune out pin bata //code for that starts here
			for (Node* no : nodes) {
				if (no->uniqueId == links[n].endNode) {
					for (InPin& pin : no->inputPins) {
						if (pin.id == links[n].endPin) {
							pin.value = -1; // default condition
							if (pin.counter > 0)
								pin.counter = 0;
							temp = &pin;
							for (Node* nod : nodes) {
								for (OutPin& opin : nod->outputPins) { // similar logic like above
									for (int i = 0; i < opin.connectedInputs.size(); i++) {
										if (opin.connectedInputs[i] == temp) {
											opin.connectedInputs[i] =
												opin.connectedInputs[opin.connectedInputs.size() - 1];
											opin.connectedInputs.pop_back();
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

int NodeManager::render() {
	ImVec4 color = {0.36f, 0.36f, 0.36f, 1.0f};
	bool iflag	 = false; // flag to check if input pins/out pins exist
	bool oflag	 = false;
	for (Node* n : nodes) {

		ImNodes::BeginNode(n->uniqueId);
		ImNodes::BeginNodeTitleBar();
		ImGui::Text("%s", n->name.c_str());
		ImNodes::EndNodeTitleBar();
		for (InPin& pin : n->inputPins) {
			if (pin.value == 0) // checking value and render proper color
				color = {0.992f, 0.266f, 0.423f, 1.0f};
			if (pin.value == 1)
				color = {0.5f, 1.0f, 0.25f, 1.0f};
			if (pin.value == -1)
				color = {0.36f, 0.36f, 0.36f, 1.0f};
			ImNodes::BeginInputAttribute(pin.id);
			ImGui::ColorButton("1", color, ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImGui::SameLine();
			ImGui::Text("%s", pin.name.c_str());
			ImNodes::EndInputAttribute();
			iflag = true;
		}
		if (iflag == true)
			ImGui::NewLine();
		for (OutPin& pin : n->outputPins) {
			if (pin.value == 0)
				color = {0.992f, 0.266f, 0.423f, 1.0f};
			if (pin.value == 1)
				color = {0.5f, 1.0f, 0.25f, 1.0f};
			if (pin.value == -1)
				color = {0.36f, 0.36f, 0.36f, 1.0f};
			ImNodes::BeginOutputAttribute(pin.id);
			if (iflag == false) {						 // if input pin doesn't exist i.e for input node
				if (ImGui::Button("", {15, 15})) {		 // create a button and check if its pressed
					pin.value = ( int )!bool(pin.value); // flip 0 and 1 //could use review as I'm unsure how safe it is
				}
				ImGui::SameLine();
			}
			ImGui::Indent();
			ImGui::Text("%s", pin.name.c_str());
			ImGui::SameLine();
			ImGui::ColorButton("", color, ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImNodes::EndOutputAttribute();
			oflag = true;
		}
		if (oflag == false) {
			ImGui::Indent();
			if (n->inputPins[0].value == -1)
				ImGui::Button("", {30, 30});
			else
				ImGui::Button((std::to_string(n->inputPins[0].value)).c_str(), {30, 30});
		}
		ImNodes::EndNode();
		iflag = false;
		oflag = false;
	}
	for (Link& l : links) {
		ImNodes::Link(l.uniqueId, l.startPin, l.endPin);
	}
	return 0;
}

int NodeManager::copyover() {
	for (Node* n : nodes) {
		for (OutPin& opin : n->outputPins) {
			for (int i = 0; i < opin.connectedInputs.size(); i++) { // pretty basic
				opin.connectedInputs[i]->value = opin.value;
			}
		}
	}
	return 0;
}

int NodeManager::calculate() {
	for (Node* n : nodes) {
		if (n->name !=
			"Input") { // input ko calculation block bolauda vertex out of bound jaanxa tesaile, not quite sure why
			n->calculate();
		}
	}
	return 0;
}