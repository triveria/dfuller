#include <dfuller/dfuller.hpp>
#include <iostream>

using namespace std;

int main()
{
    int vid              = 0x0483;
    int pid              = 0xdf11;
    int interface_number = 0;
    unsigned int timeout = 5000;

    dfuller::Device my_device(interface_number, vid, pid, timeout);

    cout << "Determining device status..." << endl;
    my_device.get_status();
    cout << "state              = " << (int)my_device.state << endl;
    cout << "status             = " << (int)my_device.status << endl;
    cout << "poll_timeout       = " << my_device.poll_timeout << endl;
    cout << "status_description = " << (int)my_device.status_description << endl;

    return 0;
}
