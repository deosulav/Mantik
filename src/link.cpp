#include "link.h"

link& link::operator= (const link& other) noexcept {
	m_unique_id = other.m_unique_id;
	start_node = other.start_node;
	start_pin = other.start_pin;
	end_node = other.end_node;
	end_pin = other.end_pin;
	return *this;
}
