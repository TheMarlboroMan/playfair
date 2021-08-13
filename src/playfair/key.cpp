#include <playfair/key.h>

using namespace playfair;

std::ostream& playfair::operator<<(std::ostream& _stream, const key& _key) {

	_stream<<_key.to_string();
	return _stream;
}
