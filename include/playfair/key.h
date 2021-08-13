#pragma once

#include "alphabet.h"
#include "exception.h"

#include <ostream>
#include <string>

namespace playfair {

/**
*interface for the cypher key.
*/
class key {

	public:
	
	virtual std::string to_string() const=0;
	virtual bool        has(char) const=0;
	virtual int         get_width() const=0;
	virtual int         get_height() const=0;
	/** 
	*returns row of the character in the key. Assume it exists.
	*/
	virtual int         row(char) const=0;
	/** 
	*returns column of the character in the key. Assume it exists.
	*/
	virtual int         column(char) const=0;
	/** 
	*returns character at [x, y] in the key. Assume it exists.
	*/
	virtual char        at(int, int) const=0;
	
	friend std::ostream& operator<<(std::ostream&, const key&);
};

std::ostream& operator<<(std::ostream&, const key&);
}
