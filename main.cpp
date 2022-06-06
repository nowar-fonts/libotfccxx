#include <cstdio>
#include <fstream>
#include <iostream>

#include <otfcc.hpp>

using namespace std;

int main() {
	cout << "hello, world" << endl;

	ifstream ifs("AND-Regular.ttf");
	string raw_data{std::istreambuf_iterator<char>(ifs),
	                std::istreambuf_iterator<char>()};

	auto td = otfcc::sfnt::read_sfnt(raw_data);

	for (auto &[k, v] : td.table_records) {
		cout << k << ' ' << v.length() << endl;
	}
}
