#ifndef NODE_TYPES_H
#define NODE_TYPES_H

enum node_types {
	NOT_ADDING = 0,
	INPUT=1,
	AND_GATE,
	OR_GATE,
	NOT_GATE,
	MULTIPLEXER8_1,


	MAX_GATE_TYPES // useful for for loops
};


#endif // NODE_TYPES_H
