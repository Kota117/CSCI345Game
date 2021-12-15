#pragma once

using namespace std;

class Exception{
	string description;

	public:
	Exception(string newDescription){
		description = newDescription;
	}
	
	friend ostream & operator << (ostream &out, const Exception &e){	
		return out << "Error: " << e.description << endl;
	}
};