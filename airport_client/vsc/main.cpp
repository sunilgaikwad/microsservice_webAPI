#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int main(int argc, char* argv[])
{
 //Default input   
 utility::string_t input = U("PUN,SIN:PHI,SDA:SIN,MAY:MAY,DEN:DEN,PHI:SDA,MUM:ITA,PAR:REZ,NYC:MUM,ITA:PAR,REZ");
 if(argc == 2)
 {
    input = argv[1];
 }
 // Create http_client to send the request.
 http_client client(U("http://127.0.0.1:12345/"));

 json::value postData;
 postData = json::value::string(input);

 pplx::task<void> requestTask = client.request(methods::POST, U(""), postData.serialize(), U("application/json")).then([](http_response response)
{
  printf("Received response status code:%u\n", response.status_code());
 
  if(response.status_code() == status_codes::OK)
  {
    //Returned result from server   
    auto body = response.extract_string();      
    std::wcout << body.get().c_str()<<std::endl;
  }
});
try
     {
         requestTask.wait();
     }
     catch (const std::exception &e)
     {
         printf("Error exception:%s\n", e.what());
     }                
    return 0;
}