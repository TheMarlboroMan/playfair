#pragma once

#include <stdexcept>

namespace playfair {

struct exception
	:public std::runtime_error {
	
			exception(const std::string&);
};

}
