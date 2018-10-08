#include "stdafx.h"
#include "Message.h"


Message::Message() : body_length_(0)
{
}


Message::~Message()
{
}

char* Message::data() {
	return data_;
}


void Message::setBodyLength(std::size_t new_length) {
	body_length_ = new_length;
	if (body_length_ > max_body_length)
		body_length_ = max_body_length;
}

std::size_t Message::getBodyLength() {

	return body_length_;
}

std::size_t Message::getLength() {

	return header_length + body_length_;
}

void Message::encodeHeader() {
	char header[header_length + 1] = "";
	sprintf_s(header, "%4d", static_cast<int>(body_length_));
	std::memcpy(data_, header, header_length);
}

bool Message::decodeHeader() {
	char header[header_length + 1] = "";
	strncat_s(header, data_, header_length);
	body_length_ = std::atoi(header);
	if (body_length_ > max_body_length) {
		body_length_ = 0;
		return false;
	}
	return true;
}


char* Message::body() {

	return data_ + header_length;
}