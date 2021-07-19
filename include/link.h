#ifndef LINK_H
#define LINK_H

class link {
	int m_unique_id;
	int start_node;
	int start_pin;
	int end_node;
	int end_pin;

	link(int m_id, int st_node, int st_pin, int ed_node, int ed_pin)
		: m_unique_id(m_id)
		, start_node(st_node)
		, start_pin(st_pin)
		, end_node(ed_node)
		, end_pin(ed_pin)
	{}

	link& operator=(const link& other) noexcept;

	friend class node_manager;
};

#endif // LINK_H
