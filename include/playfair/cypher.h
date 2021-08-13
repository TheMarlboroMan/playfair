#pragma once

#include "key.h"
#include "digraph.h"
#include "message.h"

namespace playfair {

class cypher {

	public:
	
	                        cypher(const key&);
	std::string             encode(const message&, char) const;
	std::string             decode(const message&, char) const;
	            
	private:
	
	std::string             perform(const message&, char, int) const;
	const playfair::key&    cypher_key;
	digraph                 transform(const digraph&, int) const;

};

}
