#ifndef LINK_H
#define LINK_H

class Link {
	int uniqueId;
	int startNode;
	int startPin;
	int endNode;
	int endPin;

	Link(int id, int startNode, int startPin, int endNode, int endPin)
		: uniqueId(id)
		, startNode(startNode)
		, startPin(startPin)
		, endNode(endNode)
		, endPin(endPin) {}

	Link& operator=(const Link& other) noexcept;

	friend class NodeManager;
};

#endif // LINK_H
