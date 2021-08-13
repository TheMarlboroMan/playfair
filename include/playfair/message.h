#pragma once

#include "alphabet.h"

#include <string>

namespace playfair {

class message {

	public:
	
	                    message(const std::string&, const alphabet&);
	const std::string&  get() const {return contents;}
	std::size_t         size() const {return contents.size();}
		
	private:
	
	std::string         contents;
};


}
