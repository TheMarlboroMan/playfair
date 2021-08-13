#include <playfair/message.h>
#include <playfair/exception.h>

using namespace playfair;

message::message(
	const std::string& _msg, 
	const alphabet& _alphabet
) {
	
	if(2 > _msg.size()) {
	
		throw exception("invalid input message, must have at least 2 characters");
	}

	for(const auto c : _msg) {
	
		if(_alphabet.check(c)) {
		
			contents+=c;
		}
	}
	
	if(!contents.size()) {
	
		throw exception("invalid input message, dropped characters caused it to be empty");
	}
}
