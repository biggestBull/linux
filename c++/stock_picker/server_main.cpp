#include "./third-parts/yhirose/cpp-httplib/httplib.h"

//doc: https://github.com/yhirose/cpp-httplib

int main(){
	// HTTP
	httplib::Server svr;

	svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
		res.set_content("Hello World!", "text/plain");
	});

	svr.listen("0.0.0.0", 8080);

	return 0;
}
