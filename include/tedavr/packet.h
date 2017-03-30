#pragma once

#include <inttypes.h>
#include <string.h>

template <size_t size> class Packet {
public:
	template <typename p0_t, size_t p0_s = sizeof(p0_t)> Packet<size> &operator << (p0_t const &p0);
	template <typename p0_t, size_t p0_s = sizeof(p0_t)> Packet<size> &operator >> (p0_t &p0);
	uint8_t buf[size];
	size_t offset = 0;
};

template <size_t size> template <typename p0_t, size_t p0_s> Packet<size> &Packet<size>::operator << (p0_t const &p0) {
	memcpy(buf + offset, &p0, p0_s);
	offset += p0_s;
	return(*this);
}

template <size_t size> template <typename p0_t, size_t p0_s> Packet<size> &Packet<size>::operator >> (p0_t &p0) {
	memcpy(&p0, buf + offset, p0_s);
	offset += p0_s;
	return(*this);
}
