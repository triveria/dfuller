/**
 **************************************************
 * @file     dfuller.hpp
 * @author   Michael Wagner
 * @date     2020-07-26
 * @brief Some brief description about this module.
 * @details
 * Some long description about this module.
 *
 */

/** @mainpage DFUller
 *
 * This is a cool project that does such and such.
 *
 */

#pragma once

#include <libusb-1.0/libusb.h>
#include <string>

/*! @brief Some brief bla about dfuller.
 *  @details
 *  Some long bla about dfuller.
 */
namespace dfuller {

/*! @brief Some brief bla about message().
 *  @details
 *  Some long bla about message().
 *
 *  @param recipient The thing or person that shall be greeted.
 *  @return message
 */
std::string message(const std::string &recipient);

/* DFU states */
//! @todo add doxygen comment: copy from spec
#define STATE_APP_IDLE                0
#define STATE_APP_DETACH              1
#define STATE_DFU_IDLE                2
#define STATE_DFU_DOWNLOAD_SYNC       3
#define STATE_DFU_DOWNLOAD_BUSY       4
#define STATE_DFU_DOWNLOAD_IDLE       5
#define STATE_DFU_MANIFEST_SYNC       6
#define STATE_DFU_MANIFEST            7
#define STATE_DFU_MANIFEST_WAIT_RESET 8
#define STATE_DFU_UPLOAD_IDLE         9
#define STATE_DFU_ERROR               10

/* DFU status */
//! @todo add doxygen comment: copy from spec
#define STATUS_OK                 0x00
#define STATUS_ERROR_TARGET       0x01
#define STATUS_ERROR_FILE         0x02
#define STATUS_ERROR_WRITE        0x03
#define STATUS_ERROR_ERASE        0x04
#define STATUS_ERROR_CHECK_ERASED 0x05
#define STATUS_ERROR_PROG         0x06
#define STATUS_ERROR_VERIFY       0x07
#define STATUS_ERROR_ADDRESS      0x08
#define STATUS_ERROR_NOTDONE      0x09
#define STATUS_ERROR_FIRMWARE     0x0A
#define STATUS_ERROR_VENDOR       0x0B
#define STATUS_ERROR_USBR         0x0C
#define STATUS_ERROR_POR          0x0D
#define STATUS_ERROR_UNKNOWN      0x0E
#define STATUS_ERROR_STALLEDPKT   0x0F

/* DFU commands */
//! @todo add doxygen comment: copy from spec
#define DFU_DETACH    0
#define DFU_DNLOAD    1
#define DFU_UPLOAD    2
#define DFU_GETSTATUS 3
#define DFU_CLRSTATUS 4
#define DFU_GETSTATE  5
#define DFU_ABORT     6

class Device {
public:
    libusb_device_handle *device_handle = NULL;
    libusb_context *ctx                 = NULL;
    int interface_number                = 0;
    unsigned int timeout                = -1;
    uint8_t status;
    uint8_t status_description;
    uint8_t state;
    unsigned int poll_timeout;

    Device(int interface_number, int vid, int pid, unsigned int timeout);
    ~Device();

    void detach();
    void download(uint16_t wBlockNum, unsigned char *data, uint16_t length);
    void get_status();
    void clear_status();
    void abort();
    void get_state();
    void upload(uint16_t wBlockNum, unsigned char *data, uint16_t length);

private:
    void check_if_kernel_attached();
};

} /* end namespace 'dfuller' */
