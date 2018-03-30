#include "http/http_request.hpp"

_IMPLEMENT_SCOPE

namespace http{

	request::request()
	{
	}

	request::request(const std::string& http_request)
	try
	{
		parse(http_request);
	}
	catch(const parse_exception& e)
	{
		std::cout << "parse error : " << e.what() << std::endl;

		throw e;
	}

	std::string request::operator[](const std::string& key)
	{
		return header[key];
	}

	void request::parse(const std::string& http_request)
	{	
		if(http_request.empty())
		{
			throw parse_exception("HTTP request is empty. Check that request");
		}

		const int request_length = http_request.length();

		//첫째줄
		const int start_offset = http_request.find(global::LINE_CHANGE, 0);
		if(start_offset != std::string::npos)
		{
			std::stringstream start_stream(http_request.substr(0, start_offset));

			start_stream >> method;
			start_stream >> url;
			start_stream >> version;
		}
		else
		{
			throw parse_exception("Can not resolve HTTP request, check that request");
		}

		//바디 파싱
		const int body_offset = http_request.find(global::DIVIDE_BODY, start_offset + 1);
		if(body_offset != std::string::npos)
		{
			body = http_request.substr(body_offset + 2, request_length);
		}
		
		//나머지 헤더
		std::string data_string = http_request.substr(
			start_offset + 1,
			//나머지 헤더 갯수 = 전체 길이 - (전체 길이 - body 시작 위치 - \n\n이 2개이므로 -1) - 시작 위치
			request_length - (request_length - body_offset + 1) - start_offset
		);

		//토큰을 기준으로 key value 추출
		//MEMO: 물론 크게 작용하지는 않을 것 같지만 알고리즘 수정 필요한 것 같음
		std::istringstream data_stream(data_string);
		for(std::string current_line; getline(data_stream, current_line);)
		{
			int token_offset = current_line.find(global::HEADER_TOKEN, 0);

			utils::strings::delete_string(
				token_offset - 1, token_offset + 1,
				global::WHITESPACE,current_line
			);

			token_offset = current_line.find(global::HEADER_TOKEN, 0);

			add(current_line.substr(0, token_offset), current_line.substr(token_offset + 1, current_line.length()));
		}
	}

	void request::add(const std::string key, const std::string value)
	{
		header.insert(std::pair<std::string, std::string>(key, value));
	}

}

_IMPLEMENT_END