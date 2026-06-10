#ifndef NODE_MANAGER_H_
#define NODE_MANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "imgui/imgui.h"

#include "AddGate.h"
#include "Link.h"
#include "Node.h"
#include "NodeType.h"

class NodeManager {
  private:
	std::unordered_map<int, Node*> nodes;
	std::unordered_map<int, Link*> links;

  public:
	NodeType newNodeType;
	int addNode(int& id, NodeType type);
	int addLink(int& id, int startNode, int startPin, int endNode, int endPin);
	int removeLink(int const);

	int render();
	int copyover();
	int calculate();
};

#endif // NODE_MANAGER_H_
