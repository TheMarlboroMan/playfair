#include <playfair/cypher.h>
#include <playfair/exception.h>

#include <algorithm>
#include <sstream>
//TODO:
#include <iostream>

using namespace playfair;

cypher::cypher(
	const key& _key
):
 cypher_key{_key}
{

}

std::string cypher::encode(
	const message& _msg,
	char _sub
) const {

	return perform(_msg, _sub, 1);
}


std::string cypher::decode(
	const message& _msg,
	char _sub
) const {

	return perform(_msg, _sub, -1);
}

std::string cypher::perform(
	const message& _msg,
	char _sub,
	int _direction
) const {

	if(!cypher_key.has(_sub)) {
	
		throw exception("substition character not present in key");
	}

	std::string message=_msg.get();
	std::reverse(std::begin(message), std::end(message));
	
	//fill digraphs...
	std::vector<digraph> digraphs;
	while(message.size()) {
	
		char first=message.back();
		message.pop_back();
		
		char second=_sub;
		if(message.size()) {
		
			second=message.back();
			if(first==second) {
			
				second=_sub;
			}
			else {
				message.pop_back();
			}
		}
		else {
		
			second=_sub;
		}
		
		if(!cypher_key.has(first) || !cypher_key.has(second)) {
		
			throw exception("character not present in key, should not happen if we use the same key and alphabet!");
		}
		
		digraphs.push_back({first, second});
	}
	
	//transform digraphs...
	std::vector<digraph> transformed_digraphs;
	transformed_digraphs.resize(digraphs.size());
	
	std::transform(
		std::begin(digraphs),
		std::end(digraphs),
		std::back_inserter(transformed_digraphs), 
		[this, _direction](const digraph& _d) {return transform(_d, _direction);}
	);
	
	std::stringstream  ss;
	for(const auto dig : transformed_digraphs) {
	
		ss<<dig.first<<dig.second;
		
	}
	
	return ss.str();
}

digraph cypher::transform(
	const digraph& _input,
	int _direction
) const {

	int first_row{cypher_key.row(_input.first)},
		first_column{cypher_key.column(_input.first)},
		second_row{cypher_key.row(_input.second)},
		second_column{cypher_key.column(_input.second)},
		width=cypher_key.get_width(),
		height=cypher_key.get_width();
	
	//same row, shift them right...
	if(first_row == second_row) {
	
		auto wrap=[width](int& _v) {
		
			if(_v == width) {
			
				_v=0;
			}
			
			if(_v < 0) {
			
				_v=width-1;
			}
		};
	
		wrap(first_column+=_direction);
		wrap(second_column+=_direction);
		return {cypher_key.at(first_column, first_row), cypher_key.at(second_column, second_row)};
	}
	
	//same column, shift them down.
	if(first_column == second_column) {
	
		auto wrap=[height](int& _v) {
		
			if(_v == height) {
			
				_v=0;
			}
			
			if(_v < 0) {
			
				_v=height-1;
			}
		};
	
		wrap(first_row+=_direction);
		wrap(second_row+=_direction);
		return {cypher_key.at(first_column, first_row), cypher_key.at(second_column, second_row)};
	}
	
	return {
		cypher_key.at(second_column, first_row),
		cypher_key.at(first_column, second_row)
	};
}
