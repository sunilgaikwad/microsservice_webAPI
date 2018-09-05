#include "../include/handler.h"
#include <unordered_set>

//Class for stored state of each airport
class node
{
    public:
        string airportcode; //Airport name
        bool incoming; //Is this visited?
        bool outgoing; //Did fly from here?
        node *next; //Next airport flown to

        node(): incoming(false), outgoing(false),next(NULL)
        {
            
        }
};

//Convert the string data to vector data
void FlightPathtoVector(const string &path, vector<pair<string,string>> &flightpath)
{
    size_t strLength = path.length();
    size_t i = 1; //Since json adds additional double quotes, we will skip first and last character
    size_t start = 1;
    size_t end = 1;
    while(i < strLength - 1)
    {
        if(path[i] == ',')
        {
            string firstcity = path.substr(start, i - start);
            size_t start2 = i + 1;
            while((i < strLength - 1) && (path[i] != ':'))
            {
                i++;
            }
            string secondcity = path.substr(start2, i - start2);
            start = i + 1;
            flightpath.push_back(make_pair(firstcity,secondcity));
        }
        i++;
    }
}

//Find start and end airports
string FindFilghtPath(string &stringPath)
{
    //Parse string to list
    vector<pair<string,string>> vectorPath;
    FlightPathtoVector(stringPath, vectorPath);

    //Create a graph of all airport nodes
    //Remove intermediate nodes 
    string result;
    unordered_map<string, node *> hmap;
    unordered_map<string, node *>::iterator it_first;
    unordered_map<string, node *>::iterator it_second;
    for(size_t i = 0; i < vectorPath.size(); i++)
    {
        it_first = hmap.find(vectorPath[i].first);
        if(it_first != hmap.end())
        {
            //found entry
            it_first->second->outgoing = true;
            it_second = hmap.find(vectorPath[i].second);
            if(it_second != hmap.end())
            {
               it_second->second->incoming = true;
               it_first->second->next = it_second->second;
               //No need to track in between airports
               if((it_second->second->incoming == true) && (it_second->second->outgoing == true))
                 hmap.erase(it_second);
            }
            else
            {
                node *endcity = new node();
                endcity->airportcode = vectorPath[i].second;
                endcity->incoming = true;
                hmap[vectorPath[i].second] = endcity;
                it_first->second->next = endcity;
            }
            
            if((it_first->second->incoming == true) && (it_first->second->outgoing == true))
                hmap.erase(it_first);
        }
        else
        {
            node *startcity = new node();
            startcity->airportcode = vectorPath[i].first;
            startcity->outgoing = true;
            it_second = hmap.find(vectorPath[i].second);
            if(it_second != hmap.end())
            {
               it_second->second->incoming = true;
               startcity->next = it_second->second; 
               //No need to track in between airports
               if((it_second->second->incoming == true) && (it_second->second->outgoing == true))
                 hmap.erase(it_second);
            }
            else
            {
                node *endcity = new node();
                endcity->airportcode = vectorPath[i].second;
                endcity->incoming = true;
                hmap[vectorPath[i].second] = endcity;
                startcity->next = endcity;
                hmap[vectorPath[i].first] = startcity;
            }
        }
    }

    //we should remain with only two entries at this point
    if(hmap.size() != 2)
    {
        result = "Invalid City Entries";
    }
    else
    {   
        for(it_first = hmap.begin(); it_first != hmap.end(); it_first++)
        {
            it_second = it_first;
        }
        it_first = hmap.begin();

        if(it_first->second->incoming == true)
           result = "Boarded at "+ it_second->first + " and landed at " + it_first->first;     
        else
           result = "Boarded at "+ it_first->first + " and landed at " + it_second->first;
    }
    return result;
}

handler::handler()
{
    //ctor
}

 handler::handler(utility::string_t url):m_listener(url)
 {
       m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
       m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
       m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
       m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
 }
handler::~handler()
{
}

 void handler::handle_error(pplx::task<void>& t)
 {
   try
    {
        t.get();
    }
    catch(...)
    {}
 }


//Get Request 
 void handler::handle_get(http_request message)
 {
    string rep = "Not Implemented";
    message.reply(status_codes::OK,rep);
    return;
 };

// POST request
 void handler::handle_post(http_request message)
 {
     json::value temp;
     message.extract_json()       //extracts the request content into a json
         .then([&temp](pplx::task<json::value> task) {
         try
        {
            temp = task.get();
        }
        catch(http_exception const & e)
        {
            printf("Error exception:%s\n", e.what());
        }
        })
        .wait();
        
    string pathString = temp.serialize();
    string result = FindFilghtPath(pathString);
    message.reply(status_codes::OK, result); //send the reply as a json.
    return ;
 };

//DELETE request
 void handler::handle_delete(http_request message)
 {
    string rep = "Not Implemented";
    message.reply(status_codes::OK,rep);
    return;
 };

//PUT request 
 void handler::handle_put(http_request message)
{
    string rep = "Not Implemented";
    message.reply(status_codes::OK,rep);
    return;
 };
