# microsservice_webAPI
Server/Client micro service using cpprestsdk (casablanca) 

This tutorial walks through how to setup up a simple microservice webapi utilizing Casablanca. In particular taking a look at how to use our http_client to connect to a server and get some data processed. 
With brew on OSX

Step 1
$ brew install cpprestsdk

Step 2
Build server using
cd ./airport_server/vsc/
g++ -std=c++11 ./src/handler.cpp main.cpp -stdlib=libc++ -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include

Step 3
Build client using
cd ./airport_client/vsc/
g++ -std=c++11 main.cpp -stdlib=libc++ -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include

Step 4
Run server with default port
./airport_server/vsc/a.out
Run server with custom port
./airport_server/vsc/a.out portnumber

Step 5
Run client with default cities list
./airport_client/vsc/a.out
Run client with custom cities list
./airport_client/vsc/a.out citieslist

citieslist argument format
"start,end:start,end:start,end:...."

API endpoints
handle_post hadles the request from client and processes the airport list

