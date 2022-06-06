#pragma once

#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>

namespace otfcc::type {

using uint8_t = std::uint8_t;
using int8_t = std::int8_t;
using uint16_t = std::uint16_t;
using int16_t = std::int16_t;

struct uint24_t {
	uint32_t _value;

	constexpr uint24_t() = default;
	constexpr uint24_t(const uint24_t &) = default;
	constexpr uint24_t(uint32_t v) : _value(v & 0xffffff) {}

	constexpr uint24_t &operator=(const uint24_t &) = default;
	constexpr uint24_t &operator=(uint32_t v) {
		_value = v & 0xffffff;
		return *this;
	}

	constexpr operator uint32_t() const { return _value & 0xffffff; }

	constexpr std::strong_ordering
	operator<=>(const uint24_t &) const = default;
};

using uint32_t = std::uint32_t;
using int32_t = std::int32_t;

struct fixed_t {
	int32_t _value;

	constexpr fixed_t() = default;
	constexpr fixed_t(const fixed_t &) = default;
	constexpr fixed_t(std::floating_point auto v)
	    : _value(std::round(v * 0x10000)) {}

	constexpr fixed_t &operator=(const fixed_t &) = default;
	constexpr fixed_t &operator=(std::floating_point auto v) {
		_value = v * 0x10000;
		return *this;
	}

	template <std::floating_point T> constexpr operator T() const {
		return _value / T(0x10000);
	}

	constexpr std::strong_ordering operator<=>(const fixed_t &) const = default;
};

using fword_t = std::int16_t;
using ufword_t = std::uint16_t;

struct f2dot14_t {
	int16_t _value;

	constexpr f2dot14_t() = default;
	constexpr f2dot14_t(const f2dot14_t &) = default;
	constexpr f2dot14_t(std::floating_point auto v)
	    : _value(std::round(v * 0x4000)) {}

	constexpr f2dot14_t &operator=(const f2dot14_t &) = default;
	constexpr f2dot14_t &operator=(std::floating_point auto v) {
		_value = v * 0x4000;
		return *this;
	}

	template <std::floating_point T> constexpr operator T() const {
		return _value / T(0x4000);
	}

	constexpr std::strong_ordering
	operator<=>(const f2dot14_t &) const = default;
};

using longdatetime_t = std::int64_t;

struct tag_t {
	uint8_t _value[4];

	constexpr tag_t() : _value{' ', ' ', ' ', ' '} {};
	constexpr tag_t(const tag_t &) = default;
	constexpr tag_t(const char (&)[1]) : tag_t() {}
	constexpr tag_t(const char (&v)[2])
	    : _value{uint8_t(v[0]), ' ', ' ', ' '} {}
	constexpr tag_t(const char (&v)[3])
	    : _value{uint8_t(v[0]), uint8_t(v[1]), ' ', ' '} {}
	constexpr tag_t(const char (&v)[4])
	    : _value{uint8_t(v[0]), uint8_t(v[1]), uint8_t(v[2]), ' '} {}
	constexpr tag_t(const char (&v)[5])
	    : _value{uint8_t(v[0]), uint8_t(v[1]), uint8_t(v[2]), uint8_t(v[3])} {}

	constexpr tag_t &operator=(const tag_t &) = default;
	template <size_t N>
	constexpr tag_t &operator=(const char (&v)[N]) requires(N >= 1 && N <= 5) {
		return operator=(tag_t{v});
	};

	operator std::string() const { return {_value, _value + 4}; }

	constexpr std::strong_ordering operator<=>(const tag_t &) const = default;
};

inline std::ostream &operator<<(std::ostream &s, const tag_t &t) {
	return s << std::string(t);
}

using offset16_t = std::uint16_t;
using offset24_t = uint24_t;
using offset32_t = std::uint32_t;

struct version16dot16_t {
	uint32_t _value;

	static constexpr uint32_t convert_value(std::floating_point auto v) {
		uint32_t fixed = std::round(v * 10000);
		return ((fixed / 10000) << 16) | ((fixed % 10000 / 1000) << 12) |
		       ((fixed % 1000 / 100) << 8) | ((fixed % 100 / 10) << 4) |
		       (fixed % 10);
	}

	constexpr version16dot16_t() = default;
	constexpr version16dot16_t(const version16dot16_t &) = default;
	constexpr version16dot16_t(std::floating_point auto v)
	    : _value(convert_value(v)) {}

	template <std::floating_point T> constexpr operator T() const {
		uint32_t fixed = ((_value & 0xffff0000) >> 16) * 10000 +
		                 ((_value & 0xf000) >> 12) * 1000 +
		                 ((_value & 0xf00) >> 8) * 100 +
		                 ((_value & 0xf0) >> 4) * 10 + (_value & 0xf);
		return fixed / T(10000);
	}
};

} // namespace otfcc::type
