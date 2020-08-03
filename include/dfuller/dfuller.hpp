/**
 **************************************************
 * @file     dfuller.hpp
 * @author   Michael Wagner
 * @date     2020-07-26
 * @brief Some brief description about this module.
 * @details
 * Some long description about this module.
 * @todo
 *
 */

/** @mainpage DFUller
 *
 * This is a cool project that does such and such.
 * @todo Add documentation
 *
 */

#pragma once

#include <libusb-1.0/libusb.h>
#include <string>

/*! @brief Some brief bla about dfuller.
 *  @details
 *  Some long bla about dfuller.
 * @todo Add documentation
 */
namespace dfuller {

/*! @brief Some brief bla about message().
 *  @details
 *  Some long bla about message().
 * @todo Add documentation
 *
 *  @param recipient The thing or person that shall be greeted.
 *  @return message
 */
std::string message(const std::string &recipient);

/* DFU states */
//! @todo Add disclaimer that descriptions here are taken from DFU1.1.pdf
#define STATE_APP_IDLE                0  //!< Device is running its normal application.
#define STATE_APP_DETACH              1  //!< Device is running its normal application, has received the DFU_DETACH request, and is waiting for a USB reset.
#define STATE_DFU_IDLE                2  //!< Device is operating in the DFU mode and is waiting for requests.
#define STATE_DFU_DOWNLOAD_SYNC       3  //!< Device has received a block and is waiting for the host to solicit the status via DFU_GETSTATUS.
#define STATE_DFU_DOWNLOAD_BUSY       4  //!< Device is programming a control-write block into its nonvolatile memories.
#define STATE_DFU_DOWNLOAD_IDLE       5  //!< Device is processing a download operation. Expecting DFU_DNLOAD requests.
#define STATE_DFU_MANIFEST_SYNC       6  //!< Device has received the final block of firmware from the host and is waiting for receipt of DFU_GETSTATUS to begin the Manifestation phase; or device has completed the Manifestation phase and is waiting for receipt of DFU_GETSTATUS. (Devices that can enter this state after the Manifestation phase set bmAttributes bit bitManifestationTolerant to 1.)
#define STATE_DFU_MANIFEST            7  //!< Device is in the Manifestation phase. (Not all devices will be able to respond to DFU_GETSTATUS when in this state.)
#define STATE_DFU_MANIFEST_WAIT_RESET 8  //!< Device has programmed its memories and is waiting for a USB reset or a power on reset. (Devices that must enter this state clear bitManifestationTolerant to 0.)
#define STATE_DFU_UPLOAD_IDLE         9  //!< The device is processing an upload operation. Expecting DFU_UPLOAD requests.
#define STATE_DFU_ERROR               10 //!< An error has occurred. Awaiting the DFU_CLRSTATUS request.

/* DFU status */
//! @todo Disclaimer that descriptions here are taken from DFU1.1.pdf
#define STATUS_OK                 0x00 //!< No error condition is present.
#define STATUS_ERROR_TARGET       0x01 //!< File is not targeted for use by this device.
#define STATUS_ERROR_FILE         0x02 //!< File is for this device but fails some vendor-specific verification test.
#define STATUS_ERROR_WRITE        0x03 //!< Device is unable to write memory.
#define STATUS_ERROR_ERASE        0x04 //!< Memory erase function failed.
#define STATUS_ERROR_CHECK_ERASED 0x05 //!< Memory erase check failed.
#define STATUS_ERROR_PROG         0x06 //!< Program memory function failed.
#define STATUS_ERROR_VERIFY       0x07 //!< Programmed memory failed verification.
#define STATUS_ERROR_ADDRESS      0x08 //!< Cannot program memory due to received address that is out of range.
#define STATUS_ERROR_NOTDONE      0x09 //!< Received DFU_DNLOAD with wLength = 0, but device does not think it has all of the data yet.
#define STATUS_ERROR_FIRMWARE     0x0A //!< Device’s firmware is corrupt. It cannot return to run-time (non-DFU) operations.
#define STATUS_ERROR_VENDOR       0x0B //!< iString indicates a vendor-specific error. @todo describe iString!
#define STATUS_ERROR_USBR         0x0C //!< Device detected unexpected USB reset signaling.
#define STATUS_ERROR_POR          0x0D //!< Device detected unexpected power on reset.
#define STATUS_ERROR_UNKNOWN      0x0E //!< Something went wrong, but the device does not know what it was.
#define STATUS_ERROR_STALLEDPKT   0x0F //!< Device stalled an unexpected request.

/* DFU commands */
//! @todo Disclaimer that descriptions here are taken from DFU1.1.pdf
#define DFU_DETACH    0 //!< Request device to enter STATE_APP_DETACH state.
#define DFU_DNLOAD    1 //!< Request device to accept a block of the new firmware from host.
#define DFU_UPLOAD    2 //!< Request device to send a block of the current firmware to the host.
#define DFU_GETSTATUS 3 //!< Request status information of the device. Do not confuse status and state.
#define DFU_CLRSTATUS 4 //!< Request device to clear the current status. Used for quitting dfuERROR state.
#define DFU_GETSTATE  5 //!< Request state information of the device. Do not confuse status and state.
#define DFU_ABORT     6 //!< Request device to exit current state and enter STATE_DFU_IDLE.

class Device {
public:
    libusb_device_handle *device_handle = NULL; //! @todo Add documentation
    libusb_context *ctx                 = NULL; //! @todo Add documentation
    int interface_number                = 0;    //! @todo Add documentation
    unsigned int communication_timeout  = -1;   //! @todo Add documentation
    uint8_t status;                             //! @todo Add documentation
    uint8_t status_description;                 //! @todo Add documentation
    uint8_t state;                              //! @todo Add documentation
    unsigned int poll_timeout;                  //! @todo Add documentation

    /**
     * @brief Construct a new Device object
     * @todo Add documentation
     * 
     * @param interface_number 
     * @param vid 
     * @param pid 
     * @param timeout 
     */
    Device(int interface_number, int vid, int pid, unsigned int communication_timeout);

    /**
     * @brief Destroy the Device object
     * @todo Add documentation
     * 
     */
    ~Device();

    /**
     * @brief
     * @todo Add documentation
     * 
     * @param detach_timeout 
     */
    void detach(uint16_t detach_timeout);

    /**
     * @brief 
     * @todo Add documentation
     * 
     * @param block_number 
     * @param data 
     * @param length 
     */
    void download(uint16_t block_number, unsigned char *data, uint16_t length);

    /**
     * @brief Get the status object
     * @todo Add documentation
     * 
     */
    void get_status();

    /**
     * @brief 
     * @todo Add documentation
     * 
     */
    void clear_status();

    /**
     * @brief 
     * @todo Add documentation
     * 
     */
    void abort();

    /**
     * @brief Get the state object
     * @todo Add documentation
     * 
     */
    void get_state();

    /**
     * @brief 
     * @todo Add documentation
     * 
     * @param block_number 
     * @param data 
     * @param length 
     */
    void upload(uint16_t block_number, unsigned char *data, uint16_t length);

private:
    /**
     * @brief 
     * @todo Add documentation
     * 
     */
    void check_if_kernel_attached();
};

} /* end namespace 'dfuller' */
