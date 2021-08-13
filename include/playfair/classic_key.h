#pragma once

#include "key.h"

#include <map>

namespace playfair {

class classic_key:
	public key {
	
	public:
	                classic_key(const std::string&, int, int, const alphabet&);
	std::string     to_string() const;
	bool            has(char) const;
	int             row(char) const;
	int             column(char) const;
	char            at(int, int) const;
	int             get_width() const {return width;}
	int             get_height() const {return height;}
	
	private:
	
	void            build_key(const std::string&, int, const alphabet&);
	
	struct charindex {
		int     x{0},
				y{0};
	};
		
	int                         width,
	                            height;
	std::vector<char>           key_sequence;
	std::map<char, charindex>   key_map;
};

}

