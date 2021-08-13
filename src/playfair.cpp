#include <iostream>

#include <tools/arg_manager.h>
#include <playfair/alphabet.h>
#include <playfair/classic_key.h>
#include <playfair/message.h>
#include <playfair/cypher.h>

void usage(const std::string&);
bool choose_mode(const std::string&, std::string&, int&, int&, char&);

int main(int argc, char ** argv) {

	tools::arg_manager argm(argc, argv);
	
	enum class states {none, cypher, decypher} state{states::none};
	int width=0,
		height=0;
		
	std::string str_alphabet, str_key, str_message;
	bool print_table=false;
	
	char substitution{0};
	
	//i is 1 to skip the executable name, of course.
	for(std::size_t i=1; i<argm.size(); i++) {
	
		const auto arg=argm.get_argument(i);
		if(arg=="--mode") {
				
			auto mode=argm.arg_follows(arg) ? argm.get_following(arg) : "";
			
			if(!choose_mode(mode, str_alphabet, width, height, substitution)) {
			
				usage("invalid mode");
				return 1;
			}
			
			++i;
			continue;
		}
		
		if(arg=="--alphabet") {
		
			str_alphabet=argm.arg_follows(arg) ? argm.get_following(arg) : "";
			if(0==str_alphabet.size()) {
			
				usage("alphabet must be specified with --alphabet");
				return 1;
			}
			
			i++;
			continue;
		}
		
		if(arg=="--key") {
		
			str_key=argm.arg_follows(arg) ? argm.get_following(arg) : "";
			if(0==str_key.size()) {
			
				usage("key must be specified with --key");
				return 1;
			}
			
			i++;
			continue;
		}
		
		if(arg=="--message") {
		
			str_message=argm.arg_follows(arg) ? argm.get_following(arg) : "";
			if(0==str_message.size()) {
			
				usage("message must be specified with --message");
				return 1;
			}
			i++;
			continue;
		}
		
		if(arg=="--substitution") {
		
			std::string sub=argm.arg_follows(arg) ? argm.get_following(arg) : "";
			if(1 != sub.size()) {
			
				usage("substitution must be a single character");
				return 1;
			}
			
			substitution=sub.front();
			i++;
			continue;
		}
		
		if(arg=="--width") {
		
			try {
				width=argm.arg_follows(arg) ? std::stoi(argm.get_following(arg)) : 0;
				++i;
				continue;
			}
			catch(std::exception& e) {
			
				usage("invalid width value");
				return 1;
			}
		}
		
		if(arg=="--height") {
		
			try {
				height=argm.arg_follows(arg) ? std::stoi(argm.get_following(arg)) : 0;
				++i;
				continue;
			}
			catch(std::exception& e) {
			
				usage("invalid height value");
				return 1;
			}
		}
		
		if(arg=="--cypher") {
		
			state=state==states::none 
				? states::cypher
				: states::none;
			continue;
		}
		
		if(arg=="--decypher") {
		
			state=state==states::none 
				? states::decypher
				: states::none;
			continue;
		}
		
		if(arg=="--print_table") {
		
			print_table=true;
			continue;
		}
		
		usage(std::string{"unkown argument "}+arg);
		return 1;
	}
		
	try {
	
		if(0==substitution) {
		
			usage("specify substitution character with --substitution");
			return 1;
		}
	
		//initialize and check alphabet.
		playfair::alphabet alphabet{str_alphabet};
		if( (width * height) != (int)alphabet.size()) {
		
			std::string err{"alphabet length ("};
				err+=std::to_string(alphabet.size())
				+" does not fit in given size of "
				+std::to_string(width)+" x "+std::to_string(height);
			usage(err);
			return 1;
		}
		
		//initialize and check key...
		playfair::classic_key key{str_key, width, height, alphabet};
		
		//initialize message
		playfair::message msg{str_message, alphabet};
		if(msg.size() != str_message.size()) {
			
			std::cout<<"warning: message dropped unkown characters"
				<<std::endl
				<<"message will be '"<<msg.get()<<"'"<<std::endl<<std::endl;
		}
		
		switch(state) {
		
			case states::none:
		
				usage("must specify --cypher or --decypher, mutually exclusive");
				return 1;
			case states::cypher:
			case states::decypher: 
			
			
			if(print_table) {
			
				std::cout<<key<<std::endl<<std::endl;
			}
			
			{
			
				playfair::cypher cypher{key};
				std::cout<<(
					state==states::cypher 
						? cypher.encode(msg, substitution) 
						: cypher.decode(msg, substitution)
				)<<std::endl;
			}
			break;
		}
		return 0;
	}
	catch(std::exception& e) {
	
		usage(std::string{"error: "}+e.what());
		return 1;
	}
}

bool choose_mode(
	const std::string& _mode, 
	std::string& _alphabet, 
	int& _width, 
	int& _height,
	char& _substitution
) {

	if(_mode=="classic") {
		
		_alphabet="ABCDEFGHIKLMNOPQRSTUVWXYZ";
		_substitution='X';
		_width=5;
		_height=5;
	}
	else if(_mode=="alnum") {
		_alphabet="ABCDEFGHIKLMNOPQRSTUVWXYZ0123456789";
		_substitution='X';
		_width=5;
		_height=7;
	}
	else if(_mode=="extended") {
	
		_alphabet="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,;:!?@_+-*/'\"[]()";
		_substitution='*';
		_width=9;
		_height=9;
	}
	else {
		return false;
	}
	
	return true;
}

void usage(
	const std::string& _err
) {

	std::cout<<"usage: "<<std::endl
		<<"\tplayfair --width 5 --height 5 --alphabet abcdefghiklmnopqrstuvwxyz --key playfairexample --cypher --message \"hide the gold in the tree stump\" --substitution x"<<std::endl
		<<"\tplayfair --mode classic --key PLAYFAIREXAMPLE --cypher --message \"HIDE THE GOLD IN THE TREE STUMP\""<<std::endl
		<<"\tplayfair --width 5 --height 5 --alphabet abcdefghiklmnopqrstuvwxyz --key playfairexample --decypher --message \"bmodzbxdnabekudmuixmmouvif\" --substitution x"<<std::endl
		<<"\tplayfair --mode classic --key PLAYFAIREXAMPLE --decypher --message \"BMODZBXDNABEKUDMUIXMMOUVIF\""<<std::endl		
		<<std::endl
		<<"options:"<<std::endl		
		<<"\t--mode: shorthand for cypher options, see \"modes\" below."<<std::endl
		<<"\t--alphabet: specifies the full alphabet to be used"<<std::endl
		<<"\t--key: specifies the cypher key"<<std::endl
		<<"\t--message: specifies the message to encode / decode"<<std::endl
		<<"\t--substitution: specifies substitution character for fills and missing characters"<<std::endl
		<<"\t--width: specifies cypher table width"<<std::endl
		<<"\t--height: specifies cypher table height"<<std::endl
		<<"\t--cypher: uses cypher mode"<<std::endl
		<<"\t--decypher: uses decypher mode"<<std::endl
		<<"\t--print_table: prints the cypher table before the result"<<std::endl
		<<std::endl
		<<"modes:"<<std::endl
		<<"\tclassic: shorthand for an alphabet of ABCDEFGHIKLMNOPQRSTUVWXYZ with dimensions of 5x5 and X as a substitution character"<<std::endl
		<<"\talnum: shorthand for an alphabet of ABCDEFGHIKLMNOPQRSTUVWXYZ0123456789 with dimensions of 5x7 and X as a substitution character"<<std::endl
		<<"\textended: shorthand for an alphabet of abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,;:!?@_+-*/'\"[]() with dimensions of 9x9 and * as a substitution character"<<std::endl
		<<std::endl;
	
	if(_err.size()) {
	
		std::cerr<<_err<<std::endl;
	}
}
