#ifndef NODE_MANAGER_H_
#define NODE_MANAGER_H_

#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "AddGate.h"
#include "Link.h"
#include "Node.h"
#include "NodeType.h"

class NodeManager {
  private:
	std::vector<Node*> nodes;
	std::vector<Link> links;

  public:
	int addNode(int& id, std::string name, NodeType type);
	int addInputPin(int& nodeId, int id, std::string name, int value);
	int addOuputPin(int& nodeId, int id, std::string name, int value);
	int addLink(int& id, int startNode, int startPin, int endNode, int endPin);
	int removeLink(int const);

	int render();
	int copyover();
	int calculate();
};

#endif // NODE_MANAGER_H_
