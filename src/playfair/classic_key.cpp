#include <playfair/classic_key.h>
#include <playfair/exception.h>

#include <sstream>

using namespace playfair;

classic_key::classic_key(
	const std::string& _key_str,
	int _width,
	int _height,
	const alphabet& _alphabet
):
	width{_width},
	height{_height}
{

	if(_width <= 0) {
	
		throw exception("invalid key width, must be an integer larger than zero");		
	}
	
	if(_height <= 0) {
	
		throw exception("invalid key height, must be an integer larger than zero");		
	}
	
	if(!_key_str.size()) {
	
		//TODO: actually, keys of two-four characters are mostly stupid too xD!		
		throw exception("invalid key size, must be longer than zero");
	}
	
	for(const char c : _key_str) {
	
		if(!_alphabet.check(c)) {
		
			std::string msg{"key contains non-alphabet character "};
			msg+=c;
			throw exception(msg);
		}
	}
	
	build_key(_key_str, width, _alphabet);
	
	if((int)key_sequence.size() != (width * height)) {
	
		throw exception("bad sequence key, cannot fit into given width and height");
	}
}

void classic_key::build_key(
	const std::string& _key_str,
	int _width,
	const alphabet& _alphabet
) {

	int x=0;
	int y=0;
	
	auto try_to_add=[&x,&y, _width, this](char _c) {
	
		if(!key_map.count(_c)) {
		
			key_map[_c]={x++, y};
			key_sequence.push_back(_c);
			
			if(x == _width) {
			
				x=0;
				++y;
			}
		}
	};

	//Insert first the key characters...
	for(const char c : _key_str) {
	
		try_to_add(c);
	}	

	for(const char c : _alphabet.to_string()) {

		try_to_add(c);
	}
}

std::string classic_key::to_string() const {

	int i=0;
	std::stringstream ss;
	for(const auto c : key_sequence) {
	
		ss<<c;
		if(++i==width) {
		
			i=0;
			ss<<std::endl;
		}
	}
	
	return ss.str();
}

bool classic_key::has(
	char _c
) const {

	return key_map.count(_c);
}

int classic_key::row(
	char _c
) const {

	return key_map.at(_c).y;
}

int classic_key::column(
	char _c
) const {

	return key_map.at(_c).x;
}

char classic_key::at(
	int _x, 
	int _y
) const {

	int index=_x+(_y * width);
	return key_sequence[index];
}
