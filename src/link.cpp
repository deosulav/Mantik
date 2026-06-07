#include "Link.h"

Link& Link::operator=(const Link& other) noexcept {
	uniqueId  = other.uniqueId;
	startNode = other.startNode;
	startPin  = other.startPin;
	endNode	  = other.endNode;
	endPin	  = other.endPin;
	return *this;
}
