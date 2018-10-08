#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>

class Message
{
public:
	enum {header_length = 4};
	enum {max_body_length = 512};

	__declspec(dllexport) Message();
	__declspec(dllexport) ~Message();

	__declspec(dllexport) char* data();
	__declspec(dllexport) char* body();
	__declspec(dllexport) void setBodyLength(std::size_t new_length);
	__declspec(dllexport) std::size_t getBodyLength();
	__declspec(dllexport) std::size_t getLength();

	__declspec(dllexport) void encodeHeader();
	__declspec(dllexport) bool decodeHeader();

private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;
};

