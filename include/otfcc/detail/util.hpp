#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "otfcc/detail/type.hpp"

namespace otfcc::util {

struct buffer : std::string {

	struct reserved_offset16 {
		type::offset16_t *poffset;
		std::string data;
	};

	struct reserved_offset32 {
		type::offset32_t *poffset;
		std::string data;
	};

	std::vector<reserved_offset16> _pending16;
	std::vector<reserved_offset32> _pending32;
	size_t _current;

	buffer() = default;
	buffer(const std::string &v) : std::string(v) {}
	buffer(std::string &&v) : std::string(std::move(v)) {}
	buffer(const buffer &) = default;
	buffer(buffer &&) = default;
	~buffer() = default;

	buffer &operator=(const buffer &) = default;
	buffer &operator=(buffer &&) = default;

	void finish();
};

struct buffer_view : std::string_view {

	size_t _current;

	buffer_view() = default;
	buffer_view(const buffer_view &) = default;
	buffer_view(const std::string_view &v) : std::string_view(v) {}
	buffer_view(const std::string &v) : std::string_view(v) {}

	buffer_view &operator++() {
		_current++;
		return *this;
	}
	buffer_view &operator+=(size_t n) {
		_current += n;
		return *this;
	}

	buffer_view &operator>>(type::uint8_t &o) {
		o = (*this)[_current];
		_current++;
		return *this;
	}
	buffer_view &operator>>(type::int8_t &o) {
		o = (*this)[_current];
		_current++;
		return *this;
	}
	buffer_view &operator>>(type::uint16_t &o) {
		uint8_t b0, b1;
		(*this) >> b0 >> b1;
		o = b0 << 8 | b1;
		return *this;
	}
	buffer_view &operator>>(type::int16_t &o) {
		uint8_t b0, b1;
		(*this) >> b0 >> b1;
		o = b0 << 8 | b1;
		return *this;
	}
	buffer_view &operator>>(type::uint24_t &o) {
		uint8_t b0, b1, b2;
		(*this) >> b0 >> b1 >> b2;
		o = b0 << 16 | b1 << 8 | b2;
		return *this;
	}
	buffer_view &operator>>(type::uint32_t &o) {
		uint8_t b0, b1, b2, b3;
		(*this) >> b0 >> b1 >> b2 >> b3;
		o = b0 << 24 | b1 << 16 | b2 << 8 | b3;
		return *this;
	}
	buffer_view &operator>>(type::int32_t &o) {
		uint8_t b0, b1, b2, b3;
		(*this) >> b0 >> b1 >> b2 >> b3;
		o = b0 << 24 | b1 << 16 | b2 << 8 | b3;
		return *this;
	}
	buffer_view &operator>>(type::fixed_t &o) { return *this >> o._value; }
	buffer_view &operator>>(type::f2dot14_t &o) { return *this >> o._value; }
	buffer_view &operator>>(type::tag_t &o) {
		o._value[0] = (*this)[_current];
		o._value[1] = (*this)[_current + 1];
		o._value[2] = (*this)[_current + 2];
		o._value[3] = (*this)[_current + 3];
		_current += 4;
		return *this;
	}
	buffer_view &operator>>(type::version16dot16_t &o) {
		return *this >> o._value;
	}

	std::string read_offset16() {
		type::offset16_t offset;
		type::uint16_t length;
		(*this) >> offset >> length;
		return {begin() + offset, begin() + offset + length};
	}
	std::string read_offset32() {
		type::offset32_t offset;
		type::uint32_t length;
		(*this) >> offset >> length;
		return {begin() + offset, begin() + offset + length};
	}
};

} // namespace otfcc::util
