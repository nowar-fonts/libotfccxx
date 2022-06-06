#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include "exception.hpp"
#include "otfcc/detail/util.hpp"
#include "type.hpp"

namespace otfcc::sfnt {

struct table_directory {

	static constexpr uint32_t version_ttf = 0x00010000;
	static constexpr uint32_t version_otto = 0x4F54544F;

	static uint32_t checksum(const std::string &raw_data) {
		// pad to four-byte aligned offset with zero
		const std::size_t expected_length = (raw_data.length() + 3) & ~3;
		const std::size_t num_uint32s = expected_length / 4;
		std::vector<uint32_t> buffer(num_uint32s);
		std::memcpy(buffer.data(), raw_data.data(), raw_data.length());

		return std::accumulate(buffer.cbegin(), buffer.cend(), uint32_t(0));
	}

	std::map<type::tag_t, std::string> table_records;

	constexpr uint32_t sfnt_version() const {
		bool glyf = table_records.contains("glyf");
		bool cff = table_records.contains("CFF ");
		bool cff2 = table_records.contains("CFF2");

		if (!(glyf || cff || cff2) || (glyf + cff + cff2 > 1))
			throw exception::glyph_format_error(
			    "OpenType font should contain exactly 1 of `glyf`, `CFF ` or "
			    "`CFF2` table.");
		return glyf ? version_ttf : version_otto;
	}

	constexpr uint16_t num_tables() const { return table_records.size(); }
	constexpr uint16_t search_range() const {
		return (1 << entry_selector()) * 16;
	}
	constexpr uint16_t entry_selector() const {
		return std::floor(std::log2(num_tables()));
	}
	constexpr uint16_t range_shift() const {
		return num_tables() * 16 - search_range();
	}
};

struct ttc_header {
	uint16_t major_version;
	uint16_t minor_version;
	std::vector<table_directory> table_directories;

	static constexpr uint32_t tag_null = 0;
	static constexpr uint32_t tag_dsig = 0x44534947;

	constexpr uint32_t num_fonts() const { return table_directories.size(); }
	constexpr uint32_t dsig_tag() const { return tag_null; }
	constexpr uint32_t dsig_length() const { return 0; }
	constexpr uint32_t dsig_offset() const { return 0; }
};

inline bool is_ttc(const std::string &raw_file) {
	return raw_file.length() > 16 && raw_file[0] == 't' && raw_file[1] == 't' &&
	       raw_file[2] == 'c' && raw_file[3] == 'f';
}

inline bool is_ttf(const std::string &raw_file) {
	return raw_file.length() > 12 && raw_file[0] == 0 && raw_file[1] == 1 &&
	       raw_file[2] == 0 && raw_file[3] == 0;
}

inline bool is_otf(const std::string &raw_file) {
	return raw_file.length() > 12 && raw_file[0] == 'O' && raw_file[1] == 'T' &&
	       raw_file[2] == 'T' && raw_file[3] == 'O';
}

inline bool is_opentype(const std::string &raw_file) {
	return is_ttc(raw_file) || is_ttf(raw_file) || is_otf(raw_file);
}

inline bool is_single_sfnt(const std::string &raw_file) {
	return is_ttf(raw_file) && is_otf(raw_file);
}

inline table_directory read_sfnt(const std::string &raw_file) {
	util::buffer_view bv(raw_file);

	uint16_t num_tables;
	bv += 4; // sfnt version
	bv >> num_tables;
	bv += 6; // search range, entry selector, range shift

	table_directory td;
	for (uint16_t i = 0; i < num_tables; i++) {
		type::tag_t tag;
		bv >> tag;
		bv += 4; // checksum
		td.table_records[tag] = bv.read_offset32();
	}
	return td;
}

} // namespace otfcc::sfnt
