#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

class Config{
	ifstream reader;

	map<string, string> cfg;
	string name;

	public:
	Config(string filename){
		name = filename;
		read(name);
	}

	void read(string filename){
		reader.open("config/"+filename+".conf");

		string key = "";
        string value = "";
        bool isKey = true;

		for(string line; getline(reader, line);){
            for(char c: line){
                if(c == '='){
                    isKey = false;
                    continue;
                }
                
                if(isKey)
                    key = key + c;
                else
                    value = value + c;
            }
            cfg[key] = value;
            key = "";
            value = "";
            isKey = true;
		}
	}

	string operator [](string key){
        if(cfg.find(key) != cfg.end())
            return cfg[key];
        else
            throw Exception("Key: " + key + " not found in config object " + name);
    }

	vector<string> getMany(string key){
        vector<string> args;

        stringstream ss(cfg[key]);
        string arg;

        int argCount;
        ss >> argCount;

        for(int i=0;i<argCount;i++){
            ss >> arg;
            
            args.push_back(arg);
        }

        return args;
    }
};