#ifndef STRING_H
#define STRING_H
#include <iostream>
#include <cstring>
#include <stdio.h>


namespace cs540{
class String {
public:
	String() : internal_str(nullptr) {}
	String(char* str) : internal_str(str) {}
	String(const String &str) : internal_str(str.internal_str) {}
	String(std::string str){
		char *temp = new char[str.length() + 1];
		strcpy(temp, str.c_str());
		internal_str = temp;
		delete [] temp;
	}
/*
	char* toCharPtr() {
		return internal_str;
	}
*/
	char& operator[](int n){
		std::cout << internal_str[0] << std::endl;
		std::cout << "lol" << std::endl;
		return internal_str[0];

	}

	operator std::string (){
		return internal_str;
	}
/*
	String& operator=(const String& s) {
		internal_str = s.internal_str;
		return *this;
	}
*/
	~String() {}

	String& operator+(const String& str) {
		size_t siz = static_cast<size_t>(strlen(this->internal_str) + strlen(str.internal_str));
//		printf("new size: %d\n", siz);
		char* new_arr = (char*)malloc((siz + 1) * sizeof(char));
//		std::cout << "this: " << *this << std::endl;
//		std::cout << "other: " << str << std::endl;
		//strcat(new_arr, this->internal_str);
		//memcpy(new_arr, this->internal_str, strlen(internal_str) + 1);
		//memcpy(new_arr[strlen(internal_str) + 1], str.internal_str, strlen(str.internal_str) + 1);
		strcpy(new_arr, this->internal_str);
		strcat(new_arr, str.internal_str);
		//strcat(new_arr, '\0');
		this->internal_str = new_arr;

		//delete new_arr;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream &os, const String &s) {
		//os << s.internal_str;
		for(auto i = 0; i < strlen(s.internal_str); i++) {
			os << s.internal_str[i];
		}
		return os;
	}
private:
	char* internal_str;
};
}

#endif
