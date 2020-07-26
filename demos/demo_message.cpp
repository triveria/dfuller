#include <dfuller/dfuller.hpp>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string demo_msg;
    demo_msg = dfuller::message("World");
    cout << demo_msg << endl;
    return 0;
}
