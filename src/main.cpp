#include <stdlib.h>
#include <limits>

#include <server/server.hpp>

#include "http/http_request.hpp"

int main(int argc, char ** argv)
{
	server::server server(1, 300);
	server.start("127.0.0.1", "80");

	system("pause");
	return 0;

}