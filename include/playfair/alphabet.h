#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace playfair {

class alphabet {

	public:
	                            alphabet(const std::string&);
	std::size_t                 size() const {return components.size();}
	std::string                 to_string() const;
	
	/**
	*returns true if the character can be found.
	*/
	bool                        check(const char _c) const {
	
		return std::find(
			std::begin(components),
			std::end(components),
			_c
		) != std::end(components);
	}

	private:
	
	std::vector<char>              components;

};

}
