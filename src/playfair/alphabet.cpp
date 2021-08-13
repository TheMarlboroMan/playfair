#include <playfair/alphabet.h>
#include <playfair/exception.h>

#include <numeric>

using namespace playfair;

alphabet::alphabet(
	const std::string& _str
) {

	for(const char c : _str) {
	
		if(!check(c)) {
			components.push_back(c);
		}
	}
	
	if(!components.size()) {
	
		throw exception("empty alphabet");
	}
	
	if(components.size() != _str.size()) {
	
		throw exception("repeated characters were found in the alphabet");
	}
}

std::string alphabet::to_string() const {

	return std::accumulate(
		std::begin(components),
		std::end(components),
		std::string(),
		[](std::string& _ss, const char _c) {
			return _ss+=_c;
		}
	);
}
