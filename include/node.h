#ifndef NODE_H
#define NODE_H

#include <imnodes/imnodes.h>
#include <string>
#include <vector>

struct InPin {
	int id;
	std::string name;
	int value;
	int counter = 0;
};

struct OutPin {
	int id;
	std::string name;
	int value;
	std::vector<InPin*> connectedInputs;
};

class Node {
	const int uniqueId;
	const std::string name;

  public:
	std::vector<InPin> inputPins;

  protected:
	std::vector<OutPin> outputPins;
	std::vector<Node*> mInputs; // subject to removal

	virtual void calculate() = 0;
	friend class NodeManager;

	Node(int id, std::string name)
		: uniqueId(id)
		, name(name) {}
};

#endif // NODE_H
