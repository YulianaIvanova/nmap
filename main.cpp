#include <iostream>

#include "tcp_scanner.h"
#include "work_with_ip.h"

using namespace std;


int main(int argc, char** argv) {

    char* s = "192.168.0.115/24";
    string ip = s;

    if(ip_validator(s))
    {
        vector<string> ip_list = calculation_ip(ip);
        start(ip_list);
    }
    else
        cout<<"Invalid IP"<<endl;

    return 0;
}