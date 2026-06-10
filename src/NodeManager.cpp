#include "NodeManager.h"

ImVec4 getPinColor(int value) {
	if (value == 0)
		return {0.992f, 0.266f, 0.423f, 1.0f};
	if (value == 1)
		return {0.5f, 1.0f, 0.25f, 1.0f};
	return {0.36f, 0.36f, 0.36f, 1.0f};
}

int NodeManager::addNode(int& id, NodeType type) {
	int outputno;
	int inputno;
	int selectorno;
	int temp   = id;
	Node* node = nullptr;
	switch (type) {
	case INPUT_1:
	case INPUT_2:
	case INPUT_3:
	case INPUT_4:
	case INPUT_5:
	case INPUT_6:
	case INPUT_7:
	case INPUT_8: {
		node = new Input{id, "Input"};
		for (int iter = 0; iter <= (type - INPUT_1); iter++)
			node->outputPins.emplace_back(OutPin{++id, "I" + (std::to_string(iter)), 0});
		break;
	}
	case OUTPUT: {
		node = new Output{id, "Output"};
		node->inputPins.emplace_back(InPin{++id, "", -1});
		break;
	}
	case AND_GATE_2:
	case AND_GATE_3:
	case AND_GATE_4:
	case AND_GATE_5:
	case AND_GATE_6:
	case AND_GATE_7:
	case AND_GATE_8: {
		node = new AndGate{id, "AND Gate"};
		for (int iter = 0; iter <= (type - AND_GATE_2 + 1); iter++)
			node->inputPins.emplace_back(InPin{++id, "I" + (std::to_string(iter)), -1});
		node->outputPins.emplace_back(OutPin{++id, "O", -1});
		break;
	}
	case OR_GATE_2:
	case OR_GATE_3:
	case OR_GATE_4:
	case OR_GATE_5:
	case OR_GATE_6:
	case OR_GATE_7:
	case OR_GATE_8: {
		node = new OrGate{id, "OR Gate"};
		for (int iter = 0; iter <= (type - OR_GATE_2 + 1); iter++)
			node->inputPins.emplace_back(InPin{++id, "I" + (std::to_string(iter)), -1});
		node->outputPins.emplace_back(OutPin{++id, "O", -1});
		break;
	}
	case NOT_GATE: {
		node = new NotGate{id, "NOT Gate"};
		node->inputPins.emplace_back(InPin{++id, "A", -1});
		node->outputPins.emplace_back(OutPin{++id, "A'", -1});
		break;
	}
	case ENCODER8_3:
		node	 = new Encoder{id, "Encoder 8_3"};
		outputno = 3, inputno = 8;
	case ENCODER4_2:
		if (!node) {
			node	 = new Encoder{id, "Encoder 4_2"};
			outputno = 2, inputno = 4;
		}
	case ENCODER2_1:
		if (!node) {
			node	 = new Encoder{id, "Encoder 2_1"};
			outputno = 1, inputno = 2;
		}
		for (int iter = 0; iter < inputno; iter++)
			node->inputPins.emplace_back(InPin{++id, "S" + (std::to_string(iter)), -1});
		for (int iter = 0; iter < outputno; iter++)
			node->outputPins.emplace_back(OutPin{++id, "Y" + (std::to_string(iter)), -1});
		break;
	case DECODER3_8:
		node	 = new Decoder{id, "Decoder 3_8"};
		outputno = 8, inputno = 3;
	case DECODER2_4:
		if (!node) {
			node	 = new Decoder{id, "Decoder 2_4"};
			outputno = 4, inputno = 2;
		}
	case DECODER1_2:
		if (!node) {
			node	 = new Decoder{id, "Decoder 1_2"};
			outputno = 2, inputno = 1;
		}
		for (int iter = 0; iter < inputno; iter++)
			node->inputPins.emplace_back(InPin{++id, "S" + (std::to_string(iter)), -1});
		for (int iter = 0; iter < outputno; iter++)
			node->outputPins.emplace_back(OutPin{++id, "Y" + (std::to_string(iter)), -1});
		break;
	case MULTIPLEXER8_1:
		node	= new Multiplexer{id, "Multiplexer 8_1"};
		inputno = 8, selectorno = 3;
	case MULTIPLEXER4_1:
		if (!node) {
			node	= new Multiplexer{id, "Multiplexer 4_1"};
			inputno = 4, selectorno = 2;
		}
	case MULTIPLEXER2_1:
		if (!node) {
			node	= new Multiplexer{id, "Multiplexer 2_1"};
			inputno = 2, selectorno = 1;
		}
		for (int iter = 0; iter < inputno; iter++)
			node->inputPins.emplace_back(InPin{++id, "D" + (std::to_string(iter)), -1});
		for (int iter = 0; iter < selectorno; iter++)
			node->inputPins.emplace_back(InPin{++id, "S" + (std::to_string(iter)), -1});
		node->outputPins.emplace_back(OutPin{++id, "Y", -1});
		break;
	case DEMULTIPLEXER1_8:
		node	 = new Demultiplexer{id, "DeMultiplexer 1_8"};
		outputno = 8, selectorno = 3;
	case DEMULTIPLEXER1_4:
		if (!node) {
			node	 = new Demultiplexer{id, "DeMultiplexer 1_4"};
			outputno = 4, selectorno = 2;
		}
	case DEMULTIPLEXER1_2:
		if (!node) {
			node	 = new Demultiplexer{id, "DeMultiplexer 1_2"};
			outputno = 2, selectorno = 1;
		}
		node->inputPins.emplace_back(InPin{++id, "D", -1});
		for (int iter = 0; iter < selectorno; iter++)
			node->inputPins.emplace_back(InPin{++id, "S" + (std::to_string(iter)), -1});
		for (int iter = 0; iter < outputno; iter++)
			node->outputPins.emplace_back(OutPin{++id, "Y" + (std::to_string(iter)), -1});
		break;
	default:
		break;
	}
	if (node) {
		this->nodes[temp] = node;
	}
	return 0;
}

int NodeManager::addLink(int& id, int startNode, int startPin, int endNode, int endPin) {
	Node* start = this->nodes[startNode];
	Node* end	= this->nodes[endNode];
	auto oPin	= std::find_if(start->outputPins.begin(), start->outputPins.end(), [startPin](const OutPin& pin) {
		return pin.id == startPin;
	});
	auto iPin	= std::find_if(end->inputPins.begin(), end->inputPins.end(), [endPin](const InPin& pin) {
		return pin.id == endPin;
	});
	if (iPin->counter == 0) {
		oPin->connectedInputs.push_back(iPin.base()), iPin->counter++;
		this->links[id] = new Link{id, startNode, startPin, endNode, endPin};
	}
	return 0;
}

int NodeManager::removeLink(int id) {
	InPin* temp;
	Link* link	   = this->links[id];
	Node* end	   = this->nodes[link->endNode];
	Node* start	   = this->nodes[link->startNode];
	int endPin	   = link->endPin;
	int startPin   = link->startPin;
	auto iEndPin   = std::find_if(end->inputPins.begin(), end->inputPins.end(), [endPin](const InPin& pin) {
		return pin.id == endPin;
	});
	iEndPin->value = -1;
	if (iEndPin->counter > 0)
		iEndPin->counter = 0;
	auto oStartPin = std::find_if(start->outputPins.begin(), start->outputPins.end(), [startPin](const OutPin& pin) {
		return pin.id == startPin;
	});
	oStartPin->connectedInputs.erase(
		std::remove_if(
			oStartPin->connectedInputs.begin(),
			oStartPin->connectedInputs.end(),
			[endPin](InPin* pin) {
				return pin->id == endPin;
			}),
		oStartPin->connectedInputs.end());
	this->links.erase(id);
	delete link;
	return 0;
}

int NodeManager::render() {
	for (const auto& [key, node] : nodes) {

		ImNodes::BeginNode(node->uniqueId);
		ImNodes::BeginNodeTitleBar();
		ImGui::Text("%s", node->name.c_str());
		ImNodes::EndNodeTitleBar();
		for (InPin& pin : node->inputPins) {
			ImNodes::BeginInputAttribute(pin.id);
			ImGui::ColorButton("1", getPinColor(pin.value), ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImGui::SameLine();
			ImGui::Text("%s", pin.name.c_str());
			ImNodes::EndInputAttribute();
		}
		if (node->inputPins.size())
			ImGui::NewLine();
		for (OutPin& pin : node->outputPins) {
			ImNodes::BeginOutputAttribute(pin.id);
			if (!node->inputPins.size()) {			   // if input pin doesn't exist i.e for input node
				if (ImGui::Button("##xx", {15, 15})) { // create a button and check if its pressed
					pin.value = ( int )!bool(pin.value);
				}
				ImGui::SameLine();
			}
			ImGui::Indent();
			ImGui::Text("%s", pin.name.c_str());
			ImGui::SameLine();
			ImGui::ColorButton("", getPinColor(pin.value), ImGuiColorEditFlags_NoTooltip, {15, 15});
			ImNodes::EndOutputAttribute();
		}
		if (!node->outputPins.size()) {
			ImGui::Indent();
			if (node->inputPins[0].value == -1)
				ImGui::Button("", {30, 30});
			else
				ImGui::Button((std::to_string(node->inputPins[0].value)).c_str(), {30, 30});
		}
		ImNodes::EndNode();
	}
	for (auto const& [key, l] : this->links) {
		ImNodes::Link(l->uniqueId, l->startPin, l->endPin);
	}
	return 0;
}

int NodeManager::copyover() {
	for (const auto& [id, node] : nodes) {
		for (OutPin& oPin : node->outputPins) {
			for (int i = 0; i < oPin.connectedInputs.size(); i++) {
				oPin.connectedInputs[i]->value = oPin.value;
			}
		}
	}
	return 0;
}

int NodeManager::calculate() {
	for (const auto& [id, node] : nodes) {
		node->calculate();
	}
	return 0;
}