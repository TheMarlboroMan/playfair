#include <playfair/exception.h>

using namespace playfair;

exception::exception(
	const std::string& _msg
)
	:std::runtime_error(_msg) {
	
}
