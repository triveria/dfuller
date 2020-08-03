// ref.: https://www.dreamincode.net/forums/topic/148707-introduction-to-using-libusb-10/
// ref.: DFU Spec: https://www.usb.org/sites/default/files/DFU_1.1.pdf

/**
 **************************************************
 * @file     dfuller.cpp
 * @author   Michael Wagner
 * @date     2020-07-26
 * @brief Some brief description about this module.
 * @details
 * Some long description about this module.
 * @todo  Add documentation
 *
 */

#include <dfuller/dfuller.hpp>
#include <iostream>

using namespace std;

namespace dfuller {

std::string message(const std::string &recipient)
{
    const std::string greeting { "Hello" };
    return recipient.empty() ? greeting : greeting + ", " + recipient;
}

Device::Device(int interface_number, int vid, int pid, unsigned int communication_timeout)
{
    interface_number      = this->interface_number;
    communication_timeout = this->communication_timeout;
    libusb_device **devs  = NULL;
    int claim_status      = -1;
    int alt_status        = -1;

    libusb_init(&ctx);
    libusb_get_device_list(ctx, &devs);

    // get handle of device of interest
    device_handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
    libusb_free_device_list(devs, 1);
    if (device_handle == NULL) {
        cout << "Could not open device" << endl; //! @todo move to some logging function?
        return;
    } else {
        cout << "Got handle to device" << endl; //! @todo move to some logging function?
    }

    check_if_kernel_attached();

    claim_status = libusb_claim_interface(device_handle, interface_number);
    // cout << "Claiming USB DFU Runtime Interface..." << endl; //! @todo move to some logging function?
    if (claim_status < 0) {
        cout << "Could not claim interface" << endl; //! @todo move to some logging function?
        device_handle = NULL;
        return;
    }

    alt_status = libusb_set_interface_alt_setting(device_handle, interface_number, 0);
    if (alt_status < 0) {
        cout << "Could not set alternative interface to 0: %s" << libusb_error_name(alt_status) << endl; //! @todo move to some logging function?
        device_handle = NULL;
        return;
    }
}

Device::~Device()
{
    if (device_handle != NULL) {
        libusb_release_interface(device_handle, interface_number);
        libusb_close(device_handle);
    }
    libusb_exit(ctx);
}

void Device::detach(uint16_t detach_timeout)
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT;
    uint8_t bRequest      = DFU_DETACH;
    uint16_t wValue       = detach_timeout;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = 0;
    libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, communication_timeout);
}

void Device::download(uint16_t block_number, unsigned char *data, uint16_t length)
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT;
    uint8_t bRequest      = DFU_DNLOAD;
    uint16_t wValue       = block_number;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = length;
    libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, communication_timeout);
}

void Device::get_status()
{
    if (device_handle == NULL) {
        return;
    }
    const int expected_return_code = 6;
    int return_code                = -1;

    uint8_t bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
    uint8_t bRequest      = DFU_GETSTATUS;
    uint16_t wValue       = 0;
    uint16_t wIndex       = interface_number;
    uint8_t data[6]       = { 0 };
    uint16_t wLength      = 6;
    return_code           = libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, communication_timeout);

    if (return_code == expected_return_code) {
        status             = data[0];
        poll_timeout       = (data[3] << 2 * 8) | (data[2] << 1 * 8) | (data[1] << 0 * 8);
        state              = data[4];
        status_description = data[5];
    } else {
        status             = STATUS_ERROR_UNKNOWN;
        poll_timeout       = 0;
        state              = STATE_DFU_ERROR;
        status_description = 0;
    }
}

void Device::clear_status()
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT;
    uint8_t bRequest      = DFU_CLRSTATUS;
    uint16_t wValue       = 0;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = 0;
    libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, communication_timeout);
}

void Device::abort()
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_OUT;
    uint8_t bRequest      = DFU_ABORT;
    uint16_t wValue       = 0;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = 0;
    libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, communication_timeout);
}

void Device::get_state()
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
    uint8_t bRequest      = DFU_GETSTATE;
    uint16_t wValue       = 0;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = 1;
    state                 = libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, communication_timeout);
}

void Device::upload(uint16_t block_number, unsigned char *data, uint16_t length)
{
    if (device_handle == NULL) {
        return;
    }
    uint8_t bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE;
    uint8_t bRequest      = DFU_UPLOAD;
    uint16_t wValue       = block_number;
    uint16_t wIndex       = interface_number;
    uint16_t wLength      = length;
    libusb_control_transfer(device_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, communication_timeout);
}

void Device::check_if_kernel_attached()
{
    if (libusb_kernel_driver_active(device_handle, 0) == 1) {     // is kernel driver attached?
        cout << "Kernel Driver Active" << endl;                   //! @todo move to some logging function?
        if (libusb_detach_kernel_driver(device_handle, 0) == 0) { // try to detach it
            cout << "Kernel Driver Detached!" << endl;            //! @todo move to some logging function?
        }
    }
}

} /* end namespace 'dfuller' */
