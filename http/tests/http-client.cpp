#include "http/client.h"

// Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Added for the default_resource example
#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

using namespace std;
// Added for the json-example:
using namespace boost::property_tree;

using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

int main() {

  // Client examples
  HttpClient client("localhost:8080");

  string json_string = "{\"firstName\": \"John\",\"lastName\": \"Smith\",\"age\": 25}";

  // Synchronous request examples
  try {
    auto r1 = client.request("GET", "/match/123");
    cout << r1->content.rdbuf() << endl; // Alternatively, use the convenience function r1->content.string()

    auto r2 = client.request("POST", "/string", json_string);
    cout << r2->content.rdbuf() << endl;
  }
  catch(const SimpleWeb::system_error &e) {
    cerr << "Client request error: " << e.what() << endl;
  }

  // Asynchronous request example
  client.request("POST", "/json", json_string, [](shared_ptr<HttpClient::Response> response, const SimpleWeb::error_code &ec) {
    if(!ec)
      cout << response->content.rdbuf() << endl;
  });
    client.io_service->run();

}
