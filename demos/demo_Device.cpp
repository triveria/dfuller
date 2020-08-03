// ref. for CLI menu: https://stackoverflow.com/questions/4688055/create-menus-in-command-line

#include <algorithm>
#include <dfuller/dfuller.hpp>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Item {
public:
    Item(string description, void (*fnc)(dfuller::Device *device));
    ~Item() {};
    string description;
    void (*callback)(dfuller::Device *device);
};

Item::Item(string description, void (*callback)(dfuller::Device *device))
{
    this->description = description;
    this->callback    = callback;
}

class Menu {
public:
    Menu()
        : items()
    {
    }
    ~Menu() { }
    void add_item(string description, void (*fnc)(dfuller::Device *device));
    void display();
    void loop();

    dfuller::Device *device;
    vector<unique_ptr<Item>> items;
    bool show_menu;
};

void Menu::add_item(string description, void (*fnc)(dfuller::Device *device))
{
    unique_ptr<Item> uptr(new Item(description, fnc));
    items.push_back(move(uptr));
}

int get_user_input()
{
    int user_input;

    while (!(cin >> user_input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Bad input data. Try again" << endl;
    }
    return user_input;
}

void Menu::display()
{
    cout << "Choose: \n";
    int i = 0;
    for_each(items.begin(), items.end(), [&i](unique_ptr<Item> const &item) {
        cout << "\t" << i << ": " << item->description << "\n";
        i++;
    });
    cout << "\t" << i << ": "
         << "quit" << endl;

    cout << "\n\n\tYour choice: ";
    int choice = 0;
    choice     = get_user_input();
    if (choice < items.size()) {
        items[choice]->callback(device);
    } else if (choice == items.size()) {
        cout << "Bye" << endl;
        show_menu = false;
    } else {
        cout << "Invalid choice" << endl;
    }
}

void Menu::loop()
{
    show_menu = true;
    while (show_menu == true) {
        display();
    }
}

void call_detach(dfuller::Device *device)
{
    cout << "called detach" << endl;
    uint16_t detach_timeout;
    cout << "Set detach timeout in milliseconds: ";
    cin >> detach_timeout;
    device->detach(detach_timeout);
}

void call_download(dfuller::Device *device)
{
    cout << "called download" << endl;
    uint16_t block_number;
    unsigned char *data;
    uint16_t length;
    cout << "Set block_number: ";
    cin >> block_number;
    cout << "Set *data: ";
    cin >> data;
    cout << "Set length: ";
    cin >> length;
    device->download(block_number, data, length);
}

void call_get_status(dfuller::Device *device)
{
    cout << "called get_status" << endl;
    device->get_status();
    cout << "status: " << (int)device->status << endl;
    cout << "poll_timeout: " << (int)device->poll_timeout << endl;
    cout << "state: " << (int)device->state << endl;
    cout << "status_description: " << (int)device->status_description << endl;
}

void call_clear_status(dfuller::Device *device)
{
    cout << "called clear_status" << endl;
    device->clear_status();
}

void call_abort(dfuller::Device *device)
{
    cout << "called abort" << endl;
}

void call_get_state(dfuller::Device *device)
{
    cout << "called get_state" << endl;
    device->get_state();
    device->state;
    cout << "state: " << (int)device->state << endl;
}

void call_upload(dfuller::Device *device)
{
    cout << "called upload" << endl;
    uint16_t block_number;
    unsigned char *data;
    uint16_t length;
    cout << "Set block_number: ";
    cin >> block_number;
    cout << "Set *data: ";
    cin >> data;
    cout << "Set length: ";
    cin >> length;
    device->upload(block_number, data, length);
}

//! @todo Add comments?
int main()
{
    std::cin.unsetf(std::ios::hex);
    std::cin.unsetf(std::ios::dec);
    int vid                            = 0x0483;
    int pid                            = 0xdf11;
    int interface_number               = 0;
    unsigned int communication_timeout = 5000;

    cout << "Vendor ID:";
    cin >> vid;

    cout << "Product ID:";
    cin >> pid;

    dfuller::Device my_device(interface_number, vid, pid, communication_timeout);

    Menu menu;
    menu.device = &my_device;
    menu.add_item("detach", call_detach);
    menu.add_item("download", call_download);
    menu.add_item("get_status", call_get_status);
    menu.add_item("clear_status", call_clear_status);
    menu.add_item("abort", call_abort);
    menu.add_item("get_state", call_get_state);
    menu.add_item("upload", call_upload);
    menu.loop();

    return 0;
}
