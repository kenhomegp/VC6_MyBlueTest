/*******************************************************************************

    FILE:       testengine.h
 
                Copyright (C) 2001-2008 Cambridge Silicon Radio Ltd.
                Confidential. http://www.csr.com.
 
    PURPOSE:    Defines the TestEngine dll API, used for production testing
                of CSR BlueCore devices.

*******************************************************************************/

#ifndef __INCTestEngineh
#define __INCTestEngineh

#ifndef COMMON__TYPES_H
typedef unsigned char  uint8;
typedef signed char   int8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned long int uint32;
typedef signed long int int32;
typedef double float64;
#else
typedef double float64;
#endif /* COMMON__TYPES_H */

#define BER_BIT_COUNT 0
#define BER_BIT_ERRORS 1
#define BER_ACCESS_CODE 2 /* Not used */
#define BER_RCVD_PKTS 3
#define BER_EXP_PKTS 4
#define BER_HDR_ERRORS 5
#define BER_CRC_ERRORS 6
#define BER_UNCORR_ERRORS 7
#define BER_SYNC_ERRORS 8
#define BER_MAX 9

// VM Status codes
#define VM_STATUS_BOOT 0
#define VM_STATUS_FAIL 1
#define VM_STATUS_RUN 2
#define VM_STATUS_PANIC 3
#define VM_STATUS_EXIT 4

// CVC check exit codes
#define CVC_PRODTEST_PASS 1
#define CVC_PRODTEST_FAIL 2
#define CVC_PRODTEST_NO_CHECK 3
#define CVC_PRODTEST_FILE_NOT_FOUND 4

#ifdef WIN32
#ifdef _WIN32_WCE
#define TESTENGINE_API(T) T
#else
#define TESTENGINE_API(T) T _stdcall
#endif
#else
#define TESTENGINE_API(T) T
#endif

const int32 TE_INVALID_HANDLE = -1;
const int32 TE_ERROR = 0;
const int32 TE_OK = 1;
const int32 TE_UNSUPPORTED_FUNCTION = 2;

/* ERROR CODES - in the format returned by teGetLastError */
#define TE_ERROR_NONE                                                   0x000000
#define TE_ERROR_BCCMD_NO_SUCH_VARID                                    0x000001
#define TE_ERROR_BCCMD_DATA_EXCEEDED                                    0x000002
#define TE_ERROR_BCCMD_VAR_HAS_NO_VALUE                                 0x000003
#define TE_ERROR_BCCMD_BAD_VALUE                                        0x000004 
#define TE_ERROR_BCCMD_NO_ACCESS                                        0x000005
#define TE_ERROR_BCCMD_READ_ONLY                                        0x000006
#define TE_ERROR_BCCMD_WRITE_ONLY                                       0x000007
#define TE_ERROR_BCCMD_OTHER_ERROR                                      0x000008
#define TE_ERROR_BCCMD_PERMISSION_DENIED                                0x000009
#define TE_ERROR_TRANPSORT_ERROR                                        0x000100
#define TE_ERROR_HCI_UNKNOWN_COMMAND                                    0x010000
#define TE_ERROR_HCI_UNKNOWN_CONNECTION_ID                              0x020000
#define TE_ERROR_HCI_HARDWARE_FAILURE                                   0x030000
#define TE_ERROR_HCI_PAGE_TIMEOUT                                       0x040000
#define TE_ERROR_HCI_AUTHENTICATION_FAILURE                             0x050000
#define TE_ERROR_HCI_PIN_MISSING                                        0x060000
#define TE_ERROR_HCI_MEMORY_CAPACITY_EXCEEDED                           0x070000
#define TE_ERROR_HCI_CONNECTION_TIMEOUT                                 0x080000
#define TE_ERROR_HCI_CONNECTION_LIMIT_EXCEEDED                          0x090000
#define TE_ERROR_HCI_SYNCHRONOUS_CONNECTION_LIMIT_EXCEEDED              0x0a0000
#define TE_ERROR_HCI_ACL_CONNECTION_ALREADY_EXISTS                      0x0b0000
#define TE_ERROR_HCI_COMMAND_DISALLOWED                                 0x0c0000
#define TE_ERROR_HCI_CONNECTION_REJECTED_LIMITED_RESOURCES              0x0d0000
#define TE_ERROR_HCI_CONNECTION_REJECTED_SECURITY_REASONS               0x0e0000
#define TE_ERROR_HCI_CONNECTION_REJECTED_UNACCEPTABLE_BD_ADDR           0x0f0000
#define TE_ERROR_HCI_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED                 0x100000
#define TE_ERROR_HCI_UNSUPPORTED_FEATURE                                0x110000
#define TE_ERROR_HCI_INVALID_COMMAND_PARAMETERS                         0x120000
#define TE_ERROR_HCI_REMOTE_USER_TERMINATED_CONNECTION                  0x130000
#define TE_ERROR_HCI_REMOTE_DEVICE_TERMINATED_CONNECTION_LOW_RESOURCES  0x140000
#define TE_ERROR_HCI_REMOTE_DEVICE_TERMINATED_CONNECTION_POWER_OFF      0x150000
#define TE_ERROR_HCI_CONNECTION_TERMINATED_BY_LOCAL_HOST                0x160000
#define TE_ERROR_HCI_REPEATED_ATTEMPTS                                  0x170000
#define TE_ERROR_HCI_PAIRING_NOT_ALLOWED                                0x180000
#define TE_ERROR_HCI_UNKNOWN_LMP_PDU                                    0x190000
#define TE_ERROR_HCI_UNSUPPORTED_REMOTE_FEATURE                         0x1a0000
#define TE_ERROR_HCI_SCO_OFFSET_REJECTED                                0x1b0000
#define TE_ERROR_HCI_SCO_INTERVAL_REJECTED                              0x1c0000
#define TE_ERROR_HCI_SCO_AIR_MODE_REJECTED                              0x1d0000
#define TE_ERROR_HCI_INVALID_LMP_PARAMETERS                             0x1e0000
#define TE_ERROR_HCI_UNSPECIFIED_ERROR                                  0x1f0000
#define TE_ERROR_HCI_UNSUPPORTED_LMP_PARAMETER_VALUE                    0x200000
#define TE_ERROR_HCI_ROLE_CHANGE_NOT_ALLOWED                            0x210000
#define TE_ERROR_HCI_LMP_RESPONSE_TIMEOUT                               0x220000
#define TE_ERROR_HCI_LMP_ERROR_TRANSACTION_COLLISION                    0x230000
#define TE_ERROR_HCI_LMP_PDU_NOT_ALLOWED                                0x240000
#define TE_ERROR_HCI_ENCRYPTION_MODE_NOT_ACCEPTABLE                     0x250000
#define TE_ERROR_HCI_LINK_KEY_CANNOT_BE_CHANGED                         0x260000
#define TE_ERROR_HCI_REQUESTED_QOS_NOT_SUPPORTED                        0x270000
#define TE_ERROR_HCI_INSTANT_PASSED                                     0x280000
#define TE_ERROR_HCI_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                0x290000
#define TE_ERROR_HCI_DIFFERENT_TRANSACTION_COLLISION                    0x2a0000
#define TE_ERROR_HCI_RESERVED_1                                         0x2b0000
#define TE_ERROR_HCI_QOS_UNACCEPTABLE_PARAMETER                         0x2c0000
#define TE_ERROR_HCI_QOS_REJECTED                                       0x2d0000
#define TE_ERROR_HCI_CHANNEL_CLASSIFICATION_NOT_SUPPORTED               0x2e0000
#define TE_ERROR_HCI_INSUFFICIENT_SECURITY                              0x2f0000
#define TE_ERROR_HCI_PARAMETER_OUT_OF_MANDATORY_RANGE                   0x300000
#define TE_ERROR_HCI_RESERVED_2                                         0x310000
#define TE_ERROR_HCI_ROLE_SWITCH_PENDING                                0x320000
#define TE_ERROR_HCI_RESERVED_3                                         0x330000
#define TE_ERROR_HCI_RESERVED_SLOT_VIOLATION                            0x340000
#define TE_ERROR_HCI_ROLE_SWITCH_FAILED                                 0x350000

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*************************************************************************************
    VERSION INFORMATION
*************************************************************************************/
/*************************************************************************************
    
    Function :      int32 teGetVersion(char * versionStr)

    Parameters :    versionStr - 
                        A pointer to a preallocated string that will have the
                        version string copied to it.

    Returns :       Always 1.

    Description :   Retrieves the current version string.

    Example :
        {
            char versionStr[255];

            teGetVersion(versionStr);

            cout << "TestEngine version = " << versionStr << endl;
        }

*************************************************************************************/
TESTENGINE_API(int32) teGetVersion(char* versionStr);

/*************************************************************************************
    LINK MANAGEMENT FUNCTIONS
*************************************************************************************/

#define BCSP 0x1
#define USB 0x2
#define H4 0x4
#define H5 0x8
#define H4DS 0x10

#define SPI_LPT 0x1
#define SPI_USB 0x2

/*************************************************************************************
    
    Function :      uint32 openTestEngine(int32 transport, 
                                            char* transportDevice, 
                                            uint32 dataRate, int32 retryTimeOut, 
                                            int32 usbTimeOut)

    Parameters :    transport - 
                        Defines the protocol to be used when setting up the 
                        communication with BlueCore, where
                            BCSP = 1
                            USB = 2
                            H4 = 4
                            H5 = 8
                            H4DS = 16

                    transportDevice - 
                        Defines the physical port to use. A string which for comm 
                        ports is "\\.\COMn" where 'n' is the number of the port. USB 
                        device names take the form "\\.\CSRn".
                        <p>NOTE: In C/C++, and possibly some other languages, literal 
                        '\' characters are written as "\\", otherwise they are 
                        interpreted as the start of an escape sequence. So for these
                        languages, the literal string "\\.\COMn" is written as 
                        "\\\\.\\COMn" in the code. The prefix can be dropped for COM 
                        ports 1 to 9, i.e. "COM1" works the same as "\\\\.\\COM1".
                                
                    dataRate - 
                        Defines the baud rate to be used (for UART connections only).

                    retryTimeOut - 
                        Defines how long the function will retry, in ms, when trying 
                        to establish communication with BlueCore. Care should be taken
                        when selecting an appropriate retry timeout. BlueCore1 and 
                        BlueCore2 running early firmware versions require a longer 
                        timeout. 5000ms should be enough to pick up these early 
                        devices but this value will make openTestEngine unresponsive. 
                        It would be better to set this parameter to 1000ms and call 
                        openTestEngine up to 5 times or until it passes.
                                
                    usbTimeout - 
                        Where transport = USB, this parameter will define how long the 
                        function will retry, in ms, when trying to detect if the 
                        specified USB device has ennumerated. It has been found that 
                        some PC's take a longer time than expected to ennumerate a USB 
                        device and, in some cases, the PC's internal USB hub may have 
                        been reset which can cause a further delay in ennumeration. 
                        This parameter can be set to accomodate the extra time taken 
                        to reset USB devices.If USB is not used then this value can 
                        be set to 0.

    Returns :       This function will return 0 on failure or an unsigned integer 
                    which defines the handle to the host / host controller connection. 
                    This value is used as a parameter for all other function calls.

    Description :   One of the openTestEngine... set of functions has to called 
                    before any communication with BlueCore can begin.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            // Perform all your testing here

            closeTestEngine(iHandle);
        }

*************************************************************************************/
TESTENGINE_API(uint32) openTestEngine(int32 transport, 
                                        char const * const transportDevice, 
                                        uint32 dataRate, int32 retryTimeOut, 
                                        int32 usbTimeOut);

/*************************************************************************************
    
    Function :      uint32 openTestEngineSpi(int32 port, int32 multi, int32 transport)

    Parameters :    port - 
                        Defines the physical port to use. An LPT port number starting
                        from 1, or the id number of a USB SPI device. Set to -1 to 
                        use the default (port 1 if LPT, first USB SPI device if USB).
                        NOTE: Default may be overriden by the "SPIPORT" environment
                        variable.
                        <p>NOTE: When using SPI over LPT, the driver only supports
                        "traditional" parallel ports, i.e. LPT1 connected to the 
                        Super I/O chip on the PC motherboard. Some users have reported 
                        success with some PCI LPT cards, but this is not supported by 
                        CSR. The USB SPI device can be used where an LPT port is not 
                        available, or if more than one SPI port is required.
                                
                    multi - 
                        A gate on a multi-spi interface. Set to zero when 
                        communicating with a single device attached to a standard LPT 
                        or USB-SPI port. When using the gang programmer hardware, set 
                        to 0 - 15 to select the device to connect to (the port 
                        parameter must be SPI_LPT in this case).

                    transport -
                        Defines the SPI transport to be used, where:
                            SPI_LPT = 1
                            SPI_USB = 2
                        Set to -1 to use the default (from SPITRANS environment
                        variable if present), otherwise LPT.

    Returns :       This function will return 0 on failure or an unsigned integer 
                    which defines the handle to the host / host controller connection. 
                    This value is used as a parameter for all other function calls.

    Description :   One of the openTestEngine... set of functions has to called 
                    before any communication with BlueCore can begin.
                    <p>Note that over SPI transport the function set is limited to 
                    bccmd..., radiotest..., and hq... functions. hq functions (and 
                    radiotest functions that generate HQ traffic) are only supported 
                    if the BlueCore firmware supports HQ over SPI.
                    <p>Commands which are not supported will return 
                    TE_UNSUPPORTED_FUNCTION.
                    
    Example :

        uint32 iHandle = 0;

        iHandle = openTestEngineSpi(1, 0, SPI_LPT);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            // Perform all your testing here

            closeTestEngine(iHandle);
        }

*************************************************************************************/
TESTENGINE_API(uint32) openTestEngineSpi(int32 port, int32 multi, int32 transport);

/*************************************************************************************
    
    Function :      uint32 openTestEngineSpiTrans(char const * const trans, 
                                                    int32 multi)

    Parameters :    trans - 
                        String of space separated transport options that define the
                        transport to use. Commonly used options include:
                            SPITRANS (The physical transport to use, e.g. LPT, USB)
                            SPIPORT (The port number)
                        E.g. for LPT1, trans would be "SPITRANS=LPT SPIPORT=1".
                        These options override any environment variables of the same
                        names.
                        The transport string may be one of those returned by 
                        teGetAvailableSpiPorts, which returns transport strings for
                        all available SPI ports.
                        <p>NOTE: When using SPI over LPT, the driver only supports
                        "traditional" parallel ports, i.e. LPT1 connected to the 
                        Super I/O chip on the PC motherboard. Some users have reported 
                        success with some PCI LPT cards, but this is not supported by 
                        CSR. The USB SPI device can be used where an LPT port is not 
                        available, or if more than one SPI port is required.
                                
                    multi - 
                        A gate on a multi-spi interface. Set to zero when 
                        communicating with a single device attached to a standard LPT 
                        or USB-SPI port. When using the gang programmer hardware, set 
                        to 0 - 15 to select the device to connect to (the port 
                        parameter must be SPI_LPT in this case).

    Returns :       This function will return 0 on failure or an unsigned integer 
                    which defines the handle to the host / host controller connection. 
                    This value is used as a parameter for all other function calls.

    Description :   One of the openTestEngine... set of functions has to called 
                    before any communication with BlueCore can begin.
                    <p>Note that over SPI transport the function set is limited to 
                    bccmd..., radiotest..., and hq... functions. hq functions (and 
                    radiotest functions that generate HQ traffic) are only supported 
                    if the BlueCore firmware supports HQ over SPI.
                    <p>Commands which are not supported will return 
                    TE_UNSUPPORTED_FUNCTION.
                    <p>The transport string may be one of those returned by 
                    teGetAvailableSpiPorts, which returns transport strings for
                    all available SPI ports.

    Example :

        uint32 iHandle(0);
        uint16 maxLen(256);
        uint16 count(0);
        char* portsStr = new char[maxLen];
        char* transStr = new char[maxLen];
        vector<string> ports; // The human readable port strings (e.g. "LPT1")
        vector<string> trans; // The transport option strings (e.g. "SPITRANS=LPT SPIPORT=1")

        int32 status = teGetAvailableSpiPorts(&maxLen, portsStr, transStr, &count);
        if( status != TE_OK && maxLen != 0 )
        {
            // Not enough space - resize the storage
            portsStr = new char[maxLen];
            transStr = new char[maxLen];
            status = teGetAvailableSpiPorts(&maxLen, portsStr, transStr, &count);
        }
        if( status != TE_OK || count == 0 )
        {
            cout << "Error getting SPI ports, or none found" << endl;
            delete[] portsStr;
            delete[] transStr;
            return;
        }

        // Split up the comma separated strings of ports / transport options
        split(ports, portsStr, ','); // Use these for user selection (e.g. drop down list)
        split(trans, transStr, ','); // Use these to open a transport

        // Open the SPI port using the trans string
        // For the purposes of this example, we're just using the first in the list
        iHandle = openTestEngineSpiTrans(trans.at(0).c_str(), 0);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            // Perform all your testing here

            closeTestEngine(iHandle);
        }

        delete[] portsStr;
        delete[] transStr;

        return;
                    
*************************************************************************************/
TESTENGINE_API(uint32) openTestEngineSpiTrans(char const * const trans, int32 multi);

/*************************************************************************************
    
    Function :      int32 closeTestEngine(uint32 handle)

    Parameters :    handle - 
                        Handle to the device.

    Returns :       Always 1.

    Description :   This function should always be called when terminating 
                    communications with BlueCore. This function will NOT issue a 
                    reset.

    Example :
    
        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            // Perform all your testing here

            closeTestEngine(iHandle);
        }
                    
*************************************************************************************/
TESTENGINE_API(int32) closeTestEngine(uint32 handle);

/*************************************************************************************
    
    Function :      uint32 teGetLastError(uint32 handle)

    Parameters :    handle - 
                        Handle to the device.

    Returns :       Last reported error code. If this value is zero then no error was 
                    reported. A non-zero value signifies an error and the value is 
                    decoded as follows:
                    <table>
                        <tr><td>Byte 1 (0x000000ff) = BCCMD error
                        <tr><td>Byte 2 (0x0000ff00) = Reserved
                        <tr><td>Byte 3 (0x00ff0000) = HCI / DM error
                        <tr><td>Byte 4 (0xff000000) = Reserved
                    </table>
                    BCCMD errors are defined as:
                    <table>
                        <tr><td>0 = No error
                        <tr><td>1 = BCCMD command ID not supported
                        <tr><td>2 = Data exceeded
                        <tr><td>3 = Variable ahs no value
                        <tr><td>4 = Get or set command held an error
                        <tr><td>5 = Value inaccessable
                        <tr><td>6 = Unwriteable
                        <tr><td>7 = Unreadable
                        <tr><td>8 = Other error
                        <tr><td>9 = Request not allowed
                    </table>
                    HCI / DM errors are defined in the Bluetooth specification.

    Description :   This function can be called to obtain a more detailed error
                    description when a TestEngine function fails.<br>
                    The return value of this function should not be relied upon unless 
                    the preceding TestEngine function call returned 0 (for failure).
                    <br>
                    Error codes in the format returned by this function are defined 
                    at the top of TestEngine.h.

    Example :
    
        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            int32 iSuccess = bccmdChargerPsuTrim(iHandle, 15);

            if(iSuccess == TE_INVALID_HANDLE)
                cout << "Invalid handle" << endl;
            else if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                cout << "Unsupported function" << endl;
            else if(iSuccess == TE_ERROR)
            {
                int32 iStatus = 0;
                iStatus = teGetLastError(iHandle);
                // Handle iStatus
            }
            else
            {
                cout << "Command success" << endl;
                // Perform all your testing here
            }

            closeTestEngine(iHandle);
        }
                    
*************************************************************************************/
TESTENGINE_API(uint32) teGetLastError(uint32 handle);

/*************************************************************************************
    
    Function :      int32 bccmdSetColdReset(uint32 handle, int32 usbTimeout)

    Parameters :    handle - 
                        Handle to the device.

                    usbTimeout - 
                        Where transport = USB, this parameter will define the timeout 
                        to allow the USB device to ennumerate if it has not already 
                        ennumerated. It has been found that some PC's take a longer 
                        time than expected to ennumerate a USB device and, in some 
                        cases, the PC's internal USB hub may have been reset which can
                        cause a further delay in ennumeration. This parameter can be 
                        set to accomodate the extra time taken to reset USB devices.

    Returns :       -1 on invalid handle, 1 on success, 0 on failure.

    Description :   This command forces a hardware reset of BlueCore, deliberately 
                    discarding all of its current state - the command emulates removing 
                    power from the chip, then restoring it. It will attempt to 
                    reinitialise communication using the same transport settings as 
                    declared in openTestEngine...(). If the reset or the 
                    re-establishment fails the objects associated with the 
                    communication stack will be deleted.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            int32 iResetError = 0;  

            do
            {
                cout << "Connected!" << endl;

                int32 iSuccess = psWriteVmDisable(iHandle, 1);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

                iSuccess = hciSlave(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciSlave error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmSlave instead" 
                             << endl;
                    }
                    break;
                }

                iSuccess = hciEnableDeviceUnderTestMode(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciEnableDeviceUnderTestMode error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmEnableDeviceUnderTestMode instead"
                             << endl;
                    }
                    break;
                }

                // PERFORM TESTING HERE.....
                cout << "TESTING.........." << endl;

                iSuccess = psWriteVmDisable(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

            }while(0);

            if(!iResetError)
            {
                closeTestEngine(iHandle);
            }
        }


*************************************************************************************/
TESTENGINE_API(int32) bccmdSetColdReset(uint32 handle, int32 usbTimeout);

/*************************************************************************************
    
    Function :      int32 bccmdSetWarmReset(uint32 handle, int32 usbTimeout)

    Parameters :    handle - 
                        Handle to the device.

                    usbTimeout - 
                        Where transport = USB, this parameter will define the timeout 
                        to allow the USB device to ennumerate if it has not already 
                        ennumerated. It has been found that some PC's take a longer 
                        time than expected to ennumerate a USB device and, in some 
                        cases, the PC's internal USB hub may have been reset which can 
                        cause a further delay in ennumeration. This parameter can be 
                        set to accomodate the extra time taken to reset USB devices.

    Returns :       -1 on invalid handle, 1 on success, 0 on failure.

    Description :   This forces a hardware reset of the BlueCore, arranging that some 
                    elements of the chip�s current state may be available when the 
                    chip is restarted, assuming the chip remains powered through the 
                    reset. It will then attempt to reinitialise communication using 
                    the same transport settings as declared in openTestEngine...(). 
                    If the reset or the re-establishment fails the objects associated 
                    with the communication stack will be deleted.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            int32 iResetError = 0;
            do
            {
                cout << "Connected!" << endl;

                int32 iSuccess = psWriteVmDisable(iHandle, 1);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetWarmReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetWarmReset error" << endl;
                    break;
                }

                iSuccess = hciSlave(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciSlave error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmSlave instead"
                             << endl;
                    }
                    break;
                }

                iSuccess = hciEnableDeviceUnderTestMode(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciEnableDeviceUnderTestMode error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmEnableDeviceUnderTestMode instead"
                             << endl;
                    }
                    break;
                }

                // PERFORM TESTING HERE.....
                cout << "TESTING.........." << endl;

                iSuccess = psWriteVmDisable(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetWarmReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetWarmReset error" << endl;
                    break;
                }

            }while(0);

            if(!iResetError)
            {
                closeTestEngine(iHandle);
            }
        }


*************************************************************************************/
TESTENGINE_API(int32) bccmdSetWarmReset(uint32 handle, int32 usbTimeout);

/*************************************************************************************
    MISC. TESTS
*************************************************************************************/

/*************************************************************************************
    
    Function :      int32 radiotestPause(uint32 handle)

    Parameters :    handle - 
                        Handle to the device
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will cause any tests running to terminate.

*************************************************************************************/
TESTENGINE_API(int32) radiotestPause(uint32 handle);

/*************************************************************************************
    
    Function :      int32 radiotestDeepSleep(uint32 handle)

    Parameters :    handle - 
                        Handle to the device
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will cause the DUT to enter deep sleep.

*************************************************************************************/
TESTENGINE_API(int32) radiotestDeepSleep(uint32 handle);

/*************************************************************************************
    
    Function :      int32 radiotestPcmExtLb(uint32 handle, uint16 pcm_mode)

    Parameters :    handle - 
                        Handle to the device

                    pcm_mode - 
                        Can be:
                        <table>
                            <tr><td>0  Slave in normal 4-wire configuration
                            <tr><td>1  Master in normal 4-wire configuration
                            <tr><td>2  Master in Alcatel-specific 2-wire configuration
                        </table>
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command enables PCM external loopback test mode in which a 
                    block of random data is written to the PCM output port and is read 
                    back again on the PCM input port. A check is made that the data 
                    matches. External wiring must be provided between the corresponding 
                    pins. The pcm_mode parameter can take the following values:
                        0 Slave in normal four-wire configuration
                        1 Master in normal four-wire configuration
                        2 Master in Manchester encoded, two-wire configuration
                    For a �normal� loopback configuration pcm_mode should be 1.

*************************************************************************************/
TESTENGINE_API(int32) radiotestPcmExtLb(uint32 handle, uint16 pcm_mode);

/*************************************************************************************
    
    Function :      int32 radiotestPcmLb(uint32 handle, uint16 pcm_mode)

    Parameters :    handle - 
                        Handle to the device

                    pcm_mode - 
                        Can be:
                        <table>
                            <tr><td>0  Slave in normal 4-wire configuration
                            <tr><td>1  Master in normal 4-wire configuration
                            <tr><td>2  Master in Alcatel-specific 2-wire configuration
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command enables PCM LOOPBACK test mode in which data read 
                    from the PCM input port is written back out again via the pulse 
                    code modulation (PCM) output port after a delay. The pcm_mode 
                    parameter can take the following values:
                        0 Slave in normal 4-wire configuration
                        1 Master in normal 4-wire configuration
                        2 Master in Manchester encoded, 2-wire configuration
                    For a �normal� loopback configuration pcm_mode should be 1.

*************************************************************************************/
TESTENGINE_API(int32) radiotestPcmLb(uint32 handle, uint16 pcm_mode);

/*************************************************************************************
    
    Function :      int32 radiotestPcmTimingIn(uint32 handle, uint16 pio_out, 
                                                uint16 pcm_in)

    Parameters :    handle - 
                        Handle to the device

                    pio_out - 
                        0 to 7, PIO pin used for writing

                    pcm_in - 
                        0 to 1, PCM pin used for reading
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command is similar to PCM_EXT_LB: this makes a series of 
                    writes to a PIO pin and tests that the result is readable on one 
                    of the PCM pins used for timing in slave mode, PCM_SYNC and 
                    PCM_CLK. 
                    A check is made that the data matches. External wiring must be 
                    provided between the corresponding pins. The pio_out parameter can 
                    take the values 0 to 7, corresponding to the PIO pin to be used 
                    for output. The pcm_in parameter can take the following values:
                        0 Test input on PCM_SYNC
                        1 Test input on PCM_CLK

*************************************************************************************/
TESTENGINE_API(int32) radiotestPcmTimingIn(uint32 handle, uint16 pio_out, 
                                            uint16 pcm_in);

/*************************************************************************************
    
    Function :      int32 radiotestPcmTone(uint32 handle, uint16 freq, uint16 ampl, 
                                            uint16 dc)

    Parameters :    handle - 
                        Handle to the device

                    freq - 
                        Controls the frequency of the sine wave. The value 0 
                        corresponds to 250Hz; each increment of 1 doubles the 
                        frequency. Hence, 1 corresponds to 500Hz, 2 to 1kHz and so on. 
                        No bounds checking is performed on the parameter, but large 
                        values will not be useful owing to hardware limitations.

                    ampl - 
                        Controls the amplitude of the sine wave. 8 is full volume; 
                        Each decrement of 1 reduces the amplitude by a factor of 2. 
                        0 is valid and causes the hardware to be activated with 
                        constant audio data.

                    dc - 
                        Constant offset to be added to the audio data.
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command outputs a sine wave to the PCM port. For chips with 
                    stereo codecs, use radiotestPcmToneStereo instead.

*************************************************************************************/
TESTENGINE_API(int32) radiotestPcmTone(uint32 handle, uint16 freq, uint16 ampl, 
                                        uint16 dc);

/*************************************************************************************
    
    Function :      int32 radiotestPcmToneStereo(uint32 handle, uint16 freq, 
                                                 uint16 ampl, uint16 dc, 
                                                 uint16 channel)

    Parameters :    handle - 
                        Handle to the device

                    freq - 
                        Controls the frequency of the sine wave. The value 0 
                        corresponds to 250Hz; each increment of 1 doubles the 
                        frequency. Hence, 1 corresponds to 500Hz, 2 to 1kHz and so on. 
                        No bounds checking is performed on the parameter, but large 
                        values will not be useful owing to hardware limitations.

                    ampl - 
                        Controls the amplitude of the sine wave. 8 is full volume; 
                        Each decrement of 1 reduces the amplitude by a factor of 2. 
                        0 is valid and causes the hardware to be activated with 
                        constant audio data.

                    dc - 
                        Constant offset to be added to the audio data.

                    channel -
                        Controls the channel(s) on which the sine wave is output, 
                        where:
                        <table>
                            <tr><td>0 = Both (L & R)
                            <tr><td>1 = Left only
                            <tr><td>2 = Right only
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command outputs a sine wave to the PCM port on either one, or 
                    both stereo channels. Requires a BlueCore chip with a stereo 
                    codec. For mono chips, use radiotestPcmTone.

*************************************************************************************/
TESTENGINE_API(int32) radiotestPcmToneStereo(uint32 handle, uint16 freq, uint16 ampl, 
                                        uint16 dc, uint16 channel);

/*************************************************************************************
    
    Function :      int32 radiotestCtsRtsLb(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command performs a series of writes to the UART CTS (clear to 
                    send) line and attempts to read back the value written on the UART 
                    RTS (ready to send) line. External wiring must be provided between 
                    the corresponding pins.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCtsRtsLb(uint32 handle);

/*************************************************************************************
    
    Function :      int32 radiotestRadioStatus(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will request BlueCore to send the current radio 
                    status information. Once the command is issued BlueCore will need 
                    to be polled for the status using the hqGetRadioStatus(..) 
                    function.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.
                    <p>This function is deprecated. Use radiotestRadioStatusArray
                    instead.
                
    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) radiotestRadioStatus(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hqGetRadioStatus(uint32 handle, uint16 * r, int32 timeout)

    Parameters :    handle - 
                        Handle to the device

                    r - 
                        Pointer to an array of 7 unsigned 16-bit integers. These will 
                        be used to store the radio status array as follows:
                        <table>
                            <tr><td>Index Parameter<td>Type of Status Report
                            <tr><td>0   <td>Internal transmission level
                            <tr><td>1   <td>External transmission level
                            <tr><td>2   <td>Reception level
                            <tr><td>3   <td>Receiver�s attenuation
                            <tr><td>4   <td>Local oscillator level
                            <tr><td>5   <td>IQ trim
                            <tr><td>6   <td>Signal to image ratio in dB
                        </table>

                    timeout - 
                        Used to specify a timeout, in ms, which the software will wait 
                        for the HQ radio status packet to be returned from BlueCore.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will wait for a time period specified by "timeout" 
                    for the HQ radio status packet.
                    <p>This function is deprecated. Use hqGetRadioStatusArray
                    instead.

    Deprecated :
                
*************************************************************************************/
TESTENGINE_API(int32) hqGetRadioStatus(uint32 handle, uint16 * r, int32 timeout);

/*************************************************************************************
    
    Function :      int32 radiotestRadioStatusArray(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will request BlueCore to send the current radio 
                    status information. Once the command is issued BlueCore will need 
                    to be polled for the status using the hqGetRadioStatusArray(..) 
                    function.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

    Example :
        
        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            uint16 aStatus[10];

            cout << "Connected!" << endl;

            // Put the radio into the required state

            int32 iSuccess = radiotestRadioStatusArray(iHandle);

            if(iSuccess == TE_OK)
            {
                iTimeout = 1000;
                do
                {
                    iSuccess = hqGetRadioStatusArray(iHandle, aStatus, 100);
                    iTimeout -= 100;
                }while(iSuccess == TE_ERROR && iTimeout > 0);
            }

            if(iSuccess == TE_OK)
            {
                cout << "Internal transmission level = " << aStatus[0] << endl;
                cout << "External transmission level = " << aStatus[1] << endl;
                cout << "Reception level             = " << aStatus[2] << endl;
                cout << "Local oscillator level      = " << aStatus[3] << endl;
                cout << "IQ trim                     = " << aStatus[4] << endl;
                cout << "Signal to image ratio in dB = " << aStatus[5] << endl;
                cout << "Receiver�s attenuation      = " << aStatus[6] << endl;
                cout << "Local oscillator amplitude  = " << aStatus[7] << endl;
                cout << "Frequency error             = " << aStatus[8] << endl;
                cout << "Receive frequency error     = " << aStatus[9] << endl;
            }

            closeTestEngine(iHandle);
        }
                
*************************************************************************************/
TESTENGINE_API(int32) radiotestRadioStatusArray(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hqGetRadioStatusArray(uint32 handle, uint16 * r, 
                                                int32 timeout)

    Parameters :    handle - 
                        Handle to the device

                    r - 
                        Pointer to an array of 10 unsigned 16-bit integers. These will 
                        be used to store the radio status array as follows:
                        <table>
                            <tr><td>Index Parameter<td>Type of Status Report
                            <tr><td>0   <td>Internal transmission level
                            <tr><td>1   <td>External transmission level
                            <tr><td>2   <td>Reception level
                            <tr><td>3   <td>Local oscillator level
                            <tr><td>4   <td>IQ trim
                            <tr><td>5   <td>Signal to image ratio in dB
                            <tr><td>6   <td>Receiver�s attenuation
                            <tr><td>7   <td>Local oscillator amplitude
                            <tr><td>8   <td>Frequency error
                            <tr><td>9   <td>Receive frequency error
                        </table>

                    timeout - 
                        Used to specify a timeout, in ms, which the software will wait 
                        for the HQ radio status packets to be returned from BlueCore.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will wait for a time period specified by "timeout" 
                    for all of the HQ radio status array packets.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            uint16 aStatus[10];

            cout << "Connected!" << endl;

            // Put the radio into the required state

            int32 iSuccess = radiotestRadioStatusArray(iHandle);

            if(iSuccess == TE_OK)
            {
                iTimeout = 1000;
                do
                {
                    iSuccess = hqGetRadioStatusArray(iHandle, aStatus, 100);
                    iTimeout -= 100;
                }while(iSuccess == TE_ERROR && iTimeout > 0);
            }

            if(iSuccess == TE_OK)
            {
                cout << "Internal transmission level = " << aStatus[0] << endl;
                cout << "External transmission level = " << aStatus[1] << endl;
                cout << "Reception level             = " << aStatus[2] << endl;
                cout << "Local oscillator level      = " << aStatus[3] << endl;
                cout << "IQ trim                     = " << aStatus[4] << endl;
                cout << "Signal to image ratio in dB = " << aStatus[5] << endl;
                cout << "Receiver�s attenuation      = " << aStatus[6] << endl;
                cout << "Local oscillator amplitude  = " << aStatus[7] << endl;
                cout << "Frequency error             = " << aStatus[8] << endl;
                cout << "Receive frequency error     = " << aStatus[9] << endl;
            }

            closeTestEngine(iHandle);
        }
                
*************************************************************************************/
TESTENGINE_API(int32) hqGetRadioStatusArray(uint32 handle, uint16 * r, int32 timeout);

/*************************************************************************************
    
    Function :      int32 bccmdMemoryGet(uint32 handle, uint16 baseAddr, 
                                        uint16 dataLength, uint16* data)

    Parameters :    handle - 
                        Handle to the BlueCore device.

                    baseAddr - 
                        Address to read data from.

                    dataLength - 
                        Length (in words) of the array pointed to by data. The 
                        function can retrieve a maximum of 48 words.

                    data - 
                        Pointer to an array of words to retrieve contents of memory.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to read the contents of the BlueCore
                    memory.
                    <p>Note that the data is specified in terms of 16-bit words and
                    not bytes.
                    <p><b>See also</b> bccmdMemorySet.

*************************************************************************************/
TESTENGINE_API(int32) bccmdMemoryGet(uint32 handle, uint16 baseAddr, uint16 dataLength, 
                                    uint16* data);

/*************************************************************************************
    
    Function :      int32 bccmdMemorySet(uint32 handle, uint16 baseAddr, 
                                        uint16 dataLength, uint16 const * const data)

    Parameters :    handle - 
                        Handle to the BlueCore device.

                    baseAddr - 
                        Address to write data to.

                    dataLength - 
                        Length (in words) of the array pointed to by data. The  
                        function can write a maximum of 48 words.

                    data - 
                        Pointer to an array of words to write into memory.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to write the contents of the BlueCore
                    memory. It should be used with caution as writing to certain
                    memory locations may have unpleasant side effects.
                    <p> Note that the data is specified in terms of 16-bit words and
                    not bytes.
                    <p><b>See also</b> bccmdMemoryGet.

*************************************************************************************/
TESTENGINE_API(int32) bccmdMemorySet(uint32 handle, uint16 baseAddr, uint16 dataLength, 
                                    uint16 const * const data);

/*************************************************************************************
    
    Function :      int32 bccmdGetBuildId(uint32 handle, uint16 * buildid)

    Parameters :    handle - 
                        Handle to the device

                    buildid - 
                        Pointer to a variable to hold the returned firmware build ID

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the firmware's build ID

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout += 1000;
        }while(handle == 0 && timeout < MAX_TIMEOUT);

        if(handle != 0)
        {
            uint16 temp = 0;
            cout << "Connected!" << endl;

            int32 success = bccmdGetBuildId(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Build id = " << temp << endl;
            }

            success = bccmdGetChipVersion(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip version = " << temp << endl;
            }

            success = bccmdGetChipRevision(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip revision = " << temp << endl;
            }

            char name[MAX_LENGTH];
            success = bccmdBuildName(handle, name, MAX_LENGTH, &temp);
            if(success == TE_OK)
            {
                cout << "Build name = " << name << endl;
            }

            // Perform other testing if necessary
        
            closeTestEngine(handle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetBuildId(uint32 handle, uint16 * buildid);

/*************************************************************************************
    
    Function :      int32 bccmdBuildName(uint32 handle, char* name, uint16 max_len, 
                                        uint16 * length)

    Parameters :    handle - 
                        Handle to the device

                    name - 
                        Pointer to an array to hold the returned firmware build ID. A 
                        terminating null character will be added. If the array is not 
                        large enoungh then the string will be truncated.

                    max_len - 
                        Length of the "name" array.

                    length - 
                        Total length of the build name not including any NULL 
                        character and assuming the string will not have been truncated.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the firmware's build name.

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout += 1000;
        }while(handle == 0 && timeout < MAX_TIMEOUT);

        if(handle != 0)
        {
            uint16 temp = 0;
            cout << "Connected!" << endl;

            int32 success = bccmdGetBuildId(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Build id = " << temp << endl;
            }

            success = bccmdGetChipVersion(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip version = " << temp << endl;
            }

            success = bccmdGetChipRevision(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip revision = " << temp << endl;
            }

            char name[MAX_LENGTH];
            success = bccmdBuildName(handle, name, MAX_LENGTH, &temp);
            if(success == TE_OK)
            {
                cout << "Build name = " << name << endl;
            }

            // Perform other testing if necessary
        
            closeTestEngine(handle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdBuildName(uint32 handle, char* name, uint16 max_len, 
                                    uint16 * length);

/*************************************************************************************
    
    Function :      int32 bccmdGetChipVersion(uint32 handle, uint16 * chipver)

    Parameters :    handle - 
                        Handle to the device

                    chipver - 
                        Pointer to a variable to hold the returned chip version

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the chip major version number

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout += 1000;
        }while(handle == 0 && timeout < MAX_TIMEOUT);

        if(handle != 0)
        {
            uint16 temp = 0;
            cout << "Connected!" << endl;

            int32 success = bccmdGetBuildId(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Build id = " << temp << endl;
            }

            success = bccmdGetChipVersion(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip version = " << temp << endl;
            }

            success = bccmdGetChipRevision(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip revision = " << temp << endl;
            }

            char name[MAX_LENGTH];
            success = bccmdBuildName(handle, name, MAX_LENGTH, &temp);
            if(success == TE_OK)
            {
                cout << "Build name = " << name << endl;
            }

            // Perform other testing if necessary
        
            closeTestEngine(handle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetChipVersion(uint32 handle, uint16 * chipver);

/*************************************************************************************
    
    Function :      int32 bccmdGetChipRevision(uint32 handle, uint16 * chiprev)

    Parameters :    handle - 
                        Handle to the device

                    chiprev - 
                        Pointer to a variable to hold the returned chip revision, 
                        as follows:
                        <table>
                            <tr><td>0x43 = BC3-MM
                            <tr><td>0x15 = BC3-ROM
                            <tr><td>0xE2 = BC3-Flash
                            <tr><td>0x26 = BC4-EXT
                            <tr><td>0x30 = BC4-ROM
                            <tr><td>0xE1 = BC5-MM
                            <tr><td>0x41 = BC5-FM
                            <tr><td>0x11 = BC6-ROM
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the chip revision number. 
                    bccmdGetChipRevision can be used following bccmdGetChipVersion to 
                    identify BlueCore chip variants for all chips up to and including 
                    BC6. For future chips, i.e. those for which bccmdGetChipVersion 
                    returns a value greater than 6, use bccmdGetChipAnaVer instead.

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout += 1000;
        }while(handle == 0 && timeout < MAX_TIMEOUT);

        if(handle != 0)
        {
            uint16 temp = 0;
            cout << "Connected!" << endl;

            int32 success = bccmdGetBuildId(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Build id = " << temp << endl;
            }

            success = bccmdGetChipVersion(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip version = " << temp << endl;
            }

            success = bccmdGetChipRevision(handle, &temp);
            if(success == TE_OK)
            {
                cout << "Chip revision = " << temp << endl;
            }

            char name[MAX_LENGTH];
            success = bccmdBuildName(handle, name, MAX_LENGTH, &temp);
            if(success == TE_OK)
            {
                cout << "Build name = " << name << endl;
            }

            // Perform other testing if necessary
        
            closeTestEngine(handle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetChipRevision(uint32 handle, uint16 * chiprev);

/*************************************************************************************
    
    Function :      int32 bccmdGetChipAnaVer(uint32 handle, uint8 * major, 
                                                uint8 * minor, uint8 * vari)

    Parameters :    handle - 
                        Handle to the device

                    major - 
                        Pointer to a variable to hold the returned major version value, 
                        as follows:
                        <table>
                            <tr><td>0x08 = BC3-MM
                            <tr><td>0x09 = BC3-ROM
                            <tr><td>0x0A = BC3-Flash
                            <tr><td>0x0B = BC4-EXT
                            <tr><td>0x0C = BC4-ROM
                            <tr><td>0x0E = BC4-Audio-ROM
                            <tr><td>0x11 = BC5-MM
                            <tr><td>0x12 = BC5-FM
                            <tr><td>0x13 = BC4-Audio-Flash
                            <tr><td>0x17 = BC6-ROM
                            <tr><td>0x21 = BC7-FM
                        </table>

                    minor - 
                        Pointer to a variable to hold the returned incremental minor 
                        version value.

                    vari - 
                        Pointer to a variable to hold the returned variant value.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the version information for the chip's 
                    analogue components. bccmdGetChipAnaVer can be used following 
                    bccmdGetChipVersion to identify BlueCore chip variants. The major 
                    version number identifies the chip type. 

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout += 1000;
        }while(handle == 0 && timeout < 5000);

        if(handle != 0)
        {
            cout << "Connected!" << endl;

            uint16 ver;
            int32 success = bccmdGetChipVersion(handle, &ver);
            if(success == TE_OK)
            {
                cout << "Chip version = " << ver << endl;
            }

            uint8 major, minor, variant;
            success = bccmdGetChipAnaVer(handle, &major, &minor, &variant);
            if(success == TE_OK)
            {
                cout << "Chip analogue major version = " << (uint16)major << endl;
                cout << "Chip analogue minor version = " << (uint16)minor << endl;
                cout << "Chip analogue variant = " << (uint16)variant << endl;
            }

            // Perform other testing if necessary

            closeTestEngine(handle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetChipAnaVer(uint32 handle, uint8 * major, uint8 * minor, 
                                            uint8 * vari);

/*************************************************************************************
    
    Function :      int32 bccmdRouteClock(uint32 handle)

    Parameters :    handle - 
                        Handle to the BlueCore device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to issue a BCCMDVARID_ROUTE_CLOCK to the 
                    connected BlueCore device.<br>
                    On supported firmware this will route the clock to AIO1.<br>
                    The device must be reset with the bccmdSetColdReset function to 
                    stop the clock signal being routed to the AIO pin.

*************************************************************************************/
TESTENGINE_API(int32) bccmdRouteClock(uint32 handle);

/*************************************************************************************
    
    Function :      int32 bccmdRssiAcl(uint32 handle, uint16 connectionHandle, 
                                        int16 * rssi)

    Parameters :    handle - 
                        Handle to the BlueCore device.

                    connectionHandle - 
                        Bluetooth connection handle to remote device.

                    rssi - 
                        RSSI of a received signal in dBm.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Retrieves the received signal strength indication (RSSI) for a 
                    given ACL connection handle.The RSSI value is interpreted as a 
                    signed integer with units of dBm.

*************************************************************************************/
TESTENGINE_API(int32) bccmdRssiAcl(uint32 handle, uint16 connectionHandle, 
                                    int16 * rssi);

/*************************************************************************************
    
    Function :      int32 bccmdSetPio(uint32 handle, uint16 mask, uint16 port)

    Parameters :    handle - 
                        Handle to the device

                    mask - 
                        Read / write bit mask of PIO port
                    
                    port - 
                        Bit settings of PIO port

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will set the direction mask of the PIO and the 
                    state of the pio port.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetPio(uint32 handle, uint16 mask, uint16 port);

/*************************************************************************************
    
    Function :      int32 bccmdGetPio(uint32 handle, uint16 * mask, uint16 * port)

    Parameters :    handle - 
                        Handle to the device

                    mask - 
                        Direction mask of PIO port
                    
                    port - 
                        Bit settings of PIO port

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will retrieve the direction mask of the PIO and the 
                    state of the pio port.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetPio(uint32 handle, uint16 * mask, uint16 * port);

/*************************************************************************************
    
    Function :      int32 bccmdGetAio(uint32 handle, uint16 aio, uint16 * result, 
                                        uint8 * numBits)

    Parameters :    handle - 
                        Handle to the device

                    aio - 
                        AIO pin which will be read, 0 to 3.
                    
                    result - 
                        Holds the result of the ADC reading. This value is from 0 to 
                        ADC_MAX, where ADC_MAX = 255 or 1023 for 8 and 10 bit results 
                        respectively (the numBits parameter indicates 8 or 10 bit 
                        result).
                        <p>0 = 0V and ADC_MAX = VDD, therefore:
                        <p>voltage = (result/ADC_MAX) * VDD
                        <p>VDD can be calculated using a vref ADC reading and the vref 
                        constant. See the help for bccmdGetVrefAdc for details.

                    numBits - 
                        Holds the number of bits used for the result. In BlueCore 
                        firmware from v24, the ADC readings have been changed to return 
                        a 10 bit result (8 previously). This output parameter indicates 
                        how many bits in the result are valid, 8 or 10, removing the 
                        need to check the firmware version when using this function.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will perform an ADC reading on the specified AIO
                    pin. This function supports both the old ADC reading method (8 
                    bit results) and the new (10 bit results). BlueCore devices prior 
                    to BC5 have 8 bit ADCs. With these devices, and where the 
                    firmware used uses the new method (returns 10 bit results), the 
                    readings are shifted up by 2 bits.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetAio(uint32 handle, uint16 aio, uint16 * result, 
                                    uint8 * numBits);

/*************************************************************************************
    
    Function :      int32 bccmdBC5MMGetBatteryVoltage(uint32 handle, uint16 * voltage)

    Parameters :    handle - 
                        Handle to the device

                    voltage - 
                        Pointer to 16 bit unsigned int, holds the voltage in 
                        milli-volts, calculated from the battery voltage ADC reading.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the value of the internal battery voltage 
                    monitoring ADC and convert it to a voltage. It can be used with
                    BC5MM chips running firmware version 22d and newer.
                    <p>Using this function with chips other than BC5MM is not 
                    supported, and the results are undefined. The same goes for 
                    firmware versions older than 22d.
                    <p>Note that the accuracy of this function is limited by the ADC 
                    accuracy, resolution and stability. An ADC step of 1 will result
                    in a voltage change of approximately 16-24mV. Testing has shown 
                    that the voltage returned is generally within +/-50mV of the 
                    voltage measured using a DVM.
                    <p>This function will fail if a radiotest mode is running on the 
                    chip (e.g. radiotestTxdata1() was called previously). If this is 
                    the case, use a warm reset before calling this function.

*************************************************************************************/
TESTENGINE_API(int32) bccmdBC5MMGetBatteryVoltage(uint32 handle, uint16 * voltage);

/*************************************************************************************
    
    Function :      int32 bccmdGetFirmwareCheckMask(uint32 handle, uint16 * mask)

    Parameters :    handle - 
                        Handle to the device

                    mask - 
                        Pointer to mask which states which firmware component is 
                        present AND if that component contains a pre-calculated 
                        checksum.
                        <table>
                            <tr><td>Bit  <td>Component
                            <tr><td>0    <td>Stack
                            <tr><td>1    <td>Loader
                            <tr><td>2    <td>VM Application
                            <tr><td>3    <td>File System
                        </table>
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will check if each firmware component is present and 
                    that it contains a valid checksum. This function should be called 
                    before bccmdGetFirmwareCheck.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetFirmwareCheckMask(uint32 handle, uint16 * mask);

/*************************************************************************************
    
    Function :      int32 bccmdGetFirmwareCheck(uint32 handle, uint16 * check)

    Parameters :    handle - 
                        Handle to the device

                    check - 
                        Pointer to a mask which states which firmware component is 
                        present AND if that component contains the correct checksum.
                        <table>
                            <tr><td>Bit  <td>Component
                            <tr><td>0    <td>Stack
                            <tr><td>1    <td>Loader
                            <tr><td>2    <td>VM Application
                            <tr><td>3    <td>File System
                        </table>
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will check if each firmware component checksum is 
                    correct.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetFirmwareCheck(uint32 handle, uint16 * check);

/*************************************************************************************
    
    Function :      int32 bccmdGetExternalClockPeriod(uint32 handle, uint16 * period)

    Parameters :    handle - 
                        Handle to the device

                    period - 
                        Pointer to a value in units of 0.25us. Measures the 32kHz 
                        clock once it's been divided down to 1kHz. The expected value 
                        is therefore about 4000 (or 0xFA0).
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the external clock period over the last 
                    1kHz cycle from the appropriate pin.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetExternalClockPeriod(uint32 handle, uint16 * period);

/*************************************************************************************
    
    Function :      int32 bccmdEnableDeviceConnect(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to be discoverable and connectable. This 
                    command is effectively the BCCMD equivalent of hciSlave / dmSlave. 
                    It will enable the user to set the device into testmode via the 
                    SPI interface (a call to bccmdEnableDeviceUnderTestMode following 
                    this command is required).
                    <p>Firmware from 23g releases should support this command. It is 
                    unsupported in older firmware (function will return 2).
                    <p>Success is determined by:
                    <ol>
                    <li> Valid handle
                    <li> Command complete within timeout
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdEnableDeviceConnect(uint32 handle);

/*************************************************************************************
    
    Function :      int32 bccmdEnableDeviceUnderTestMode(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to set the enable device under test mode flag 
                    using a BCCMD instead of an HCI or DM command. This will enable 
                    the user to set the device into testmode via the SPI interface as 
                    well as UART and USB. 
                    This command will <b>NOT</b> make the device discoverable or 
                    connectable. To do this, use bccmdEnableDeviceConnect if 
                    supported by the BlueCore firmware (if not, a VM app can be used 
                    to do this, or use a host connection and hciSlave / dmSlave).
                    <p>Success is determined by:
                    <ol>
                    <li> Valid handle
                    <li> Command complete within timeout
                    </ol>

    Example :

        uint32 handle = openTestEngineSpi(1, 0, SPI_LPT);

        if(handle != 0)
        {
            int32 success;
            do
            {
                // Make connectable
                success = bccmdEnableDeviceConnect(handle);
                if(success != TE_OK)
                    break;

                // Enable DUT mode
                success = bccmdEnableDeviceUnderTestMode(handle);
                if(success != TE_OK)
                    break;

                // Run tests on BT tester

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;
        }
        if(handle != 0)
            closeTestEngine(handle);

*************************************************************************************/
TESTENGINE_API(int32) bccmdEnableDeviceUnderTestMode(uint32 handle);

/*************************************************************************************
    TRANSMITTER TESTS
*************************************************************************************/

/*************************************************************************************
    
    Function :      int32 radiotestStereoCodecLB(uint32 handle, uint16 sampleRate, 
                                                uint16 reroute)

    Parameters :    handle - 
                        Handle to the device.

                    sampleRate - 
                        Sample rate in Hz, which can be one of the following values:
                        <table>
                            <tr><td>8000
                            <tr><td>11025
                            <tr><td>16000
                            <tr><td>22050
                            <tr><td>24000
                            <tr><td>32000
                            <tr><td>44100
                        </table>

                    reroute - 
                        Controls how signal is routed from mic to speaker.
                        <table>
                            <tr><td>0 - <td>MicL to SpkL, MicR to SpkR
                            <tr><td>1 - <td>MicR to SpkL and SpkR
                            <tr><td>2 - <td>MicL to SpkL and SpkR
                            <tr><td>3 - <td>MicL to SpkR, MicR to SpkL
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to turn on the codec hardware for stereo
                    loopback.

*************************************************************************************/
TESTENGINE_API(int32) radiotestStereoCodecLB(uint32 handle, uint16 sampleRate, 
                                            uint16 reroute);

/*************************************************************************************
    
    Function :      int32 radiotestTxstart(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA, 
                                            int16 modulation)

    Parameters :    handle - 
                        Handle to the device.

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.
    
                    extPA - 
                        Drive level for external PA

                    modulation - 
                        Modulation offset - 4096 = 1MHz
    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates TXSTART

*************************************************************************************/
TESTENGINE_API(int32) radiotestTxstart(uint32 handle, uint16 frequency, uint16 intPA, 
                                        uint16 extPA, int16 modulation);

/*************************************************************************************
    
    Function :      int32 radiotestTxdata1(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates TXDATA1

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;
        int32 hopping = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;
        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {
                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;

                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;


                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                uint16 uiFreq = 2441;
                uint16 uiIntPA = 50;
                uint16 uiExtPA = 0;
                uint16 uiCC = 0;
                uint8 uiHiside = 0;
                uint16 uiRxAtt = 0;
                uint16 uiSampleSize = 10000;

                // 3. Start the transmitter and receiver
                if(!hopping)
                {
                    success = radiotestTxdata1(txHandle, uiFreq, uiIntPA, uiExtPA);
                    if(success != TE_OK)
                        break;

                    success = radiotestBer1(rxHandle, uiFreq, uiHiside, uiRxAtt, 
                                            uiSampleSize);
                    if(success != TE_OK)
                        break;
                }
                else
                {
                    success = radiotestTxdata2(txHandle, uiCC, uiIntPA, uiExtPA);
                    if(success != TE_OK)
                        break;

                    success = radiotestBer2(rxHandle, uiCC, uiHiside, uiRxAtt, 
                                            uiSampleSize);
                    if(success != TE_OK)
                        break;
                }

                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(rxHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestTxdata1(uint32 handle, uint16 frequency, uint16 intPA, 
                                        uint16 extPA);

/*************************************************************************************
    
    Function :      int32 radiotestTxdata2(uint32 handle, uint16 countrycode, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    countrycode - 
                        Must be 0. Setting the countrycode to anything other than 
                        zero is deprecated.

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates TXDATA2

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;
        int32 hopping = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {

                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;

                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;


                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                uint16 uiFreq = 2441;
                uint16 uiIntPA = 50;
                uint16 uiExtPA = 0;
                uint16 uiCC = 0;
                uint8 uiHiside = 0;
                uint16 uiRxAtt = 0;
                uint16 uiSampleSize = 10000;

                // 3. Start the transmitter and receiver
                if(!hopping)
                {
                    success = radiotestTxdata1(txHandle, uiFreq, uiIntPA, uiExtPA);
                    if(success != TE_OK)
                        break;

                    success = radiotestBer1(rxHandle, uiFreq, uiHiside, uiRxAtt, 
                                            uiSampleSize);
                    if(success != TE_OK)
                        break;
                }
                else
                {
                    success = radiotestTxdata2(txHandle, uiCC, uiIntPA, uiExtPA);
                    if(success != TE_OK)
                        break;

                    success = radiotestBer2(rxHandle, uiCC, uiHiside, uiRxAtt, 
                                            uiSampleSize);
                    if(success != TE_OK)
                        break;
                }

                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(rxHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestTxdata2(uint32 handle, uint16 countrycode, uint16 intPA, 
                                        uint16 extPA);

/*************************************************************************************
    
    Function :      int32 radiotestTxdata3(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates TXDATA3

*************************************************************************************/
TESTENGINE_API(int32) radiotestTxdata3(uint32 handle, uint16 frequency, uint16 intPA, 
                                        uint16 extPA);

/*************************************************************************************
    
    Function :      int32 radiotestTxdata4(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates TXDATA4

*************************************************************************************/
TESTENGINE_API(int32) radiotestTxdata4(uint32 handle, uint16 frequency, uint16 intPA, 
                                        uint16 extPA);

/*************************************************************************************
    
    Function :      int32 radiotestCfgTxPower(uint32 handle, int16 power)

    Parameters :    handle - 
                        Handle to the device

                    power - 
                        Requested power in dBm. This value is used to select the 
                        power table entry to use.
                        <p>The power table entry with the closest output power equal 
                        to or less than this value is selected.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function utilises the CFG_TXPOWER radiotest function. It is 
                    used to specify the output power for subsequent radiotests, e.g. 
                    radiotestTxstart. Subsequent radiotests will ignore power 
                    parameters (e.g. internal PA level, external PA level) and use 
                    the power table values instead.
                    <p>The closest power table entry with output power
                    equal to or less than the power parameter value is selected. 
                    Therefore, the actual output power may be less than what is 
                    specified. The difference is dependant on the power table 
                    configuration for the device.
                    <p>Setting the power value to the int16 representation of 0xDEAD 
                    (-8529) switches off the use of the power table for subsequent 
                    radiotest operations. After this value is set, power parameters 
                    for subsequent radiotests are used instead of the power table 
                    values.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgTxPower(uint32 handle, int16 power);

/*************************************************************************************
    RECIEVER TESTS
*************************************************************************************/
/*************************************************************************************
    
    Function :      int32 radiotestRxstart1(uint32 handle, uint16 frequency, 
                                            uint8 hiside, uint16 rx_attenuation)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency in MHz. Should be in the range 
                        2402 to 2480.

                    hiside - 
                        Set to 0 for low side modulation and 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates RXSTART1.
                    <p>Enables the receiver in continuous reception at a designated
                    frequency (LO Freq) with a choice of low or high side modulation 
                    (hi-side) and with a designated attenuation setting (RX 
                    Attenuation).
                    <p>Requires a second unit to be running TXSTART.

*************************************************************************************/
TESTENGINE_API(int32) radiotestRxstart1(uint32 handle, uint16 frequency, uint8 hiside, 
                                        uint16 rx_attenuation);

/*************************************************************************************
    
    Function :      int32 radiotestRxstart2(uint32 handle, uint16 frequency, 
                                            uint8 hiside, uint16 rx_attenuation, 
                                            uint16 sample_size)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency in MHz. Should be in the range 
                        2402 to 2480.

                    hiside - 
                        Set to 0 for low side modulation and 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

                    sample_size - 
                        Number of RSSI reports to capture (for averaging purposes), up 
                        to a maximum value of 100. Values greater than 100 will result 
                        in the function returning Error.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates RXSTART2.
                    <p>Enables the receiver in continuous reception, at a designated 
                    frequency (LO Freq), with a choice of low or high side modulation 
                    (hi-side) and with a designated attenuation setting (RX 
                    Attenuation).
                    <p>Requires a second unit to be running TXSTART (see 
                    radiotestTxstart).
                    <p>RSSI results can be obtained by calling hqGetRssi. When 
                    sample_size results have been obtained, further results are 
                    ignored until either the set has been collected with hqGetRssi, 
                    or until radiotestRxstart2 is called again.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) radiotestRxstart2(uint32 handle, uint16 frequency, uint8 hiside, 
                                        uint16 rx_attenuation, uint16 sample_size);

/*************************************************************************************
    
    Function :      int32 hqGetRssi(uint32 handle, int32 timeout, uint16 max_size, 
                                    uint16 * r)

    Parameters :    handle - 
                        Handle to the device
                 
                    timeout - 
                        Time, in ms, which the function will wait for valid results. 
                        If set to zero the function will just check for completion.

                    max_size - 
                        Size of the array allocated to hold the RSSI samples

                    r - 
                        Array to contain the RSSI samples

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function obtains a set of RSSI results from BlueCore, 
                    obtained as a result of calling radiotestRxstart2. The number of 
                    results that will be written is dependant on the sample_size 
                    parameter for radiotestRxstart2.

*************************************************************************************/
TESTENGINE_API(int32) hqGetRssi(uint32 handle, int32 timeout, uint16 max_size, 
                                uint16 * r);

/*************************************************************************************
    
    Function :      int32 radiotestBer1(uint32 handle, uint16 frequency, uint8 hiside, 
                                        uint16 rx_attenuation, uint32 sampleSize)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency in MHz. Should be in the range 
                        2402 to 2480.

                    hiside - 
                        Set to 0 for low side modulation and 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

                    sampleSize - 
                        Number of bits to be sampled.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates BER1.
                    <p>Enables the receiver at a designated frequency (LO Freq) with 
                    a choice of low or high side modulation (hi-side) and with a 
                    designated attenuation setting (RX Attenuation).
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) radiotestBer1(uint32 handle, uint16 frequency, uint8 hiside, 
                                    uint16 rx_attenuation, uint32 sampleSize);

/*************************************************************************************
    
    Function :      int32 radiotestBer2(uint32 handle, uint16 country_code, 
                                        uint8 hiside, uint16 rx_attenuation, 
                                        uint32 sampleSize)

    Parameters :    handle - 
                        Handle to the device

                    country_code - 
                        Must be 0. Setting the country_code to anything other than 
                        zero is deprecated.

                    hiside - 
                        Set to 0 for low side modulation and 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

                    sampleSize - 
                        Number of bits to be sampled.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates BER2.
                    <p>Enables the receiver with hopping, with a choice of low or 
                    high side modulation (hiside), and with a designated attenuation 
                    setting (rx_attenuation) as for radiotestRxdata2.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) radiotestBer2(uint32 handle, uint16 country_code, uint8 hiside, 
                                    uint16 rx_attenuation, uint32 sampleSize);

/*************************************************************************************
    
    Function :      int32 radiotestBerLoopback(uint32 handle, uint16 frequency, 
                                                uint16 intPA, uint16 extPA, 
                                                uint32 sampleSize);

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

                    sampleSize - 
                        Number of bits to be sampled.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates BER_LOOPBACK.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {
                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;
                
                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                static const uint16 freq(2441);
                static const uint16 intPA(50);
                static const uint16 extPA(0);
                static const uint16 sampleSize(10000);

                // 3. Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, freq, intPA, extPA, 
                                                sampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, freq, intPA, extPA);
                if(success != TE_OK)
                    break;
        
                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestBerLoopback(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA, 
                                            uint32 sampleSize);

/*************************************************************************************
    
    Function :      int32 radiotestRxLoopback(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates RX_LOOPBACK.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) radiotestRxLoopback(uint32 handle, uint16 frequency, 
                                        uint16 intPA, uint16 extPA);

/*************************************************************************************
    
    Function :      int32 radiotestLoopback(uint32 handle, uint16 frequency, 
                                            uint16 intPA, uint16 extPA)

    Parameters :    handle - 
                        Handle to the device

                    frequency - 
                        Local Oscillator frequency

                    intPA - 
                        Drive level for internal PA. 0 to 63 for basic rate packets,
                        0 to 127 for EDR packets. Typical values are 50 for basic 
                        rate and 105 for EDR packet types.

                    extPA - 
                        Drive level for external PA

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function initiates LOOPBACK

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {
                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;
                
                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                static const uint16 freq(2441);
                static const uint16 intPA(50);
                static const uint16 extPA(0);
                static const uint16 sampleSize(10000);

                // 3. Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, freq, intPA, extPA, 
                                                sampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, freq, intPA, extPA);
                if(success != TE_OK)
                    break;
        
                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestLoopback(uint32 handle, uint16 frequency, uint16 intPA, 
                                        uint16 extPA);

/*************************************************************************************
    
    Function :      int32 hqGetBer(uint32 handle, int32 timeout, uint32 * r)

    Parameters :    handle - 
                        Handle to the device
                    
                    timeout - 
                        Time, in ms, which the function will wait for valid results. 
                        If set to zero the function will just check for completion.

                    r - 
                        Array to contain culmative BER report. Defined as follows:
                        <table>
                            <tr><td>r[0] = cumulative bit count
                            <tr><td>r[1] = cumulative bit errors
                            <tr><td>r[2] = NOT USED
                            <tr><td>r[3] = cumulative recieved packets
                            <tr><td>r[4] = cumulative expected packets
                            <tr><td>r[5] = cumulative header errors
                            <tr><td>r[6] = cumulative CRC errors
                            <tr><td>r[7] = cumulative uncorrected errors
                            <tr><td>r[8] = cumulative sync errors
                        </table>

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function obtains the last complete BER results sample from 
                    BlueCore.

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;
        int32 hopping = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {

                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;

                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;


                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                uint16 uiFreq = 2441;
                uint16 uiIntPA = 50;
                uint16 uiExtPA = 0;
                uint16 uiCC = 0;
                uint8 uiHiside = 0;
                uint16 uiRxAtt = 0;
                uint16 uiSampleSize = 10000;

                // 3. Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, 2441, uiIntPA, uiExtPA, 
                                                uiSampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, uiFreq, uiIntPA, uiExtPA);
                if(success != TE_OK)
                    break;

                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) hqGetBer(uint32 handle, int32 timeout, uint32 * r);

/*************************************************************************************
    
    Function :      int32 radiotestRxdata1(uint32 handle, uint16 frequency, 
                                            uint8 hiside, uint16 rx_attenuation)

    Parameters :    handle - 
                        Handle to the device.

                    frequency - 
                        Local Oscillator (LO) frequency in MHz. Should be in the range 
                        2402 to 2480.

                    hiside - 
                        Set to 0 for low side modulation or 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Enables the receiver, at a designated frequency (LO Freq) with 
                    a choice of low or high side modulation (hi-side), and with a 
                    designated attenuation setting (RX Attenuation). The software 
                    counts the number of received packets and the number of payloads 
                    with correctable errors.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) radiotestRxdata1(uint32 handle, uint16 frequency, uint8 hiside, 
                                        uint16 rx_attenuation);

/*************************************************************************************
    
    Function :      int32 radiotestRxdata2(uint32 handle, uint16 countrycode, 
                                            uint8 hiside, uint16 rx_attenuation)

    Parameters :    handle - 
                        Handle to the device.

                    countrycode - 
                        Must be 0. Setting the countrycode to anything other than 
                        zero is deprecated.

                    hiside - 
                        Set to 0 for low side modulation or 1 for high side 
                        modulation.

                    rx_attenuation - 
                        Attenuation setting. Should be in the range 0 to 15.

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Enables the receiver with hopping, with a choice of low or high 
                    side modulation (hiside) and with a designated attenuation 
                    setting (rx_attenuation). The software counts the number of 
                    received packets and the number of payloads with correctable 
                    errors.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) radiotestRxdata2(uint32 handle, uint16 countrycode, 
                                        uint8 hiside, uint16 rx_attenuation);

/*************************************************************************************
    
    Function :      int32 hqGetRxdata(uint32 handle, int32 timeout, uint16 * r)

    Parameters :    handle - 
                        Handle to the device

                    timeout - 
                        Time, in ms, which the function will wait for valid results. 
                        If set to zero the function will just check for completion.

                    r - 
                        Pointer to an array which will receive the following:
                        <table>
                            <tr><td> r[0] = Number of packets received
                            <tr><td> r[1] = Number of good packets
                            <tr><td> r[2] = Number of corrected packets
                            <tr><td> r[3] = RSSI value
                            <tr><td> r[4] = Boolean flag showing if RSSI value is valid
                        </table>

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will return the last packet generated by either 
                    RXDATA1 or RXDATA2. Once the packet has been read it cannot be 
                    read again until the next packet is generated.

*************************************************************************************/
TESTENGINE_API(int32) hqGetRxdata(uint32 handle, int32 timeout, uint16 * r);

/*************************************************************************************
    CONFIGURATION COMMANDS
*************************************************************************************/
/*************************************************************************************
    
    Function :      int32 radiotestCfgFreq(uint32 handle, uint16 txrxinterval, 
                                            uint16 loopback, uint16 report)

    Parameters :    handle - 
                        Handle to the device

                    txrxinterval - 
                        Sets the period in microseconds between TX and RX events in 
                        RXDATA, TXDATA, BIT ERR and LOOP BACK test modes. Default is 
                        1250 (20 slots), maximum 65536. If passed as 0, current value 
                        unchanged.
                    
                    loopback - 
                        Sets the offset in microseconds between a reception event and 
                        retransmission of the data in loopback. Default is 1875 (two 
                        slots later), must be less than txrxinterval. If passed as 
                        zero current value unchanged.

                    report - 
                        Sets the time in seconds between reports to host sent by 
                        RXDATA and BIT ERR functions. The default is 1s. If passed 0, 
                        the current value is unchanged. The maximum value is 65. 
                        Passing a value greater than 65 results in error.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets three values used in deciding timing details of tests. This 
                    command has been superceded by radiotestCfgFreqMs, which allows 
                    finer control of the reporting interval (in milli-seconds).

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;
        int32 hopping = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {

                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;

                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;


                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                uint16 uiFreq = 2441;
                uint16 uiIntPA = 50;
                uint16 uiExtPA = 0;
                uint16 uiCC = 0;
                uint8 uiHiside = 0;
                uint16 uiRxAtt = 0;
                uint16 uiSampleSize = 10000;

                // 3. Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, 2441, uiIntPA, uiExtPA, 
                                                uiSampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, uiFreq, uiIntPA, uiExtPA);
                if(success != TE_OK)
                    break;

                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgFreq(uint32 handle, uint16 txrxinterval, 
                                        uint16 loopback, uint16 report);

/*************************************************************************************
    
    Function :      int32 radiotestCfgFreqMs(uint32 handle, uint16 txrxinterval, 
                                                uint16 loopback, uint16 report)

    Parameters :    handle - 
                        Handle to the device

                    txrxinterval - 
                        Sets the period in microseconds between TX and RX events in 
                        RXDATA, TXDATA, BIT ERR and LOOP BACK test modes. Default is 
                        1250 (20 slots), maximum 65536. If passed as 0, current value 
                        unchanged.
                    
                    loopback - 
                        Sets the offset in microseconds between a reception event and 
                        retransmission of the data in loopback. Default is 1875 (two 
                        slots later), must be less than txrxinterval. If passed as 
                        zero current value unchanged.

                    report - 
                        Sets the time in milli-seconds between reports to host sent 
                        by RXDATA and BIT ERR functions. The default is 1000mS. If 
                        passed 0, the current value is unchanged. The minimum value 
                        for this parameter is 100mS. Attempting to set a value greater 
                        than 0 and less than 100mS results in error.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets three values used in deciding timing details of tests, with 
                    the report interval being set in milli-seconds (whereas 
                    radiotestCfgFreq sets the interval in seconds). If the firmware 
                    version being used does not support this function, it will 
                    return error. In this case, use radiotestCfgFreq instead.
                    <p>For BIT ERR functions, reducing the reporting time 
                    from the default 1000mS may improve test time for small sample 
                    sizes tested with big packet sizes (i.e. if you are getting far 
                    more bits in each report than are required for your sample size).

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;

        cout << "Trying rxHandle..." << endl;
        rxHandle = openTestEngine(BCSP, "COM1", 115200, 5000, 0);

        cout << "Trying txHandle..." << endl;
        txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 5000, 100);

        if((txHandle != 0) && (rxHandle != 0))
        {
            int32 success = 0;

            do
            {
                // Configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;
                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // Set the TX/RX interval
                success = radiotestCfgFreqMs(rxHandle, 37500, 9375, 200);
                if(success != TE_OK)
                    break;
                success = radiotestCfgFreqMs(txHandle, 37500, 9375, 200);
                if(success != TE_OK)
                    break;

                static const uint16 freq(2441);
                static const uint16 intPA(50);
                static const uint16 extPA(0);
                static const uint16 sampleSize(10000);

                // Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, freq, intPA, extPA, 
                                                sampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, freq, intPA, extPA);
                if(success != TE_OK)
                    break;

                int32 count = 0;
                uint32 r[9];

                // Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgFreqMs(uint32 handle, uint16 txrxinterval, 
                                            uint16 loopback, uint16 report);

/*************************************************************************************
    
    Function :      int32 radiotestCfgPkt(uint32 handle, uint16 type, uint16 size)

    Parameters :    handle - 
                        Handle to the device

                    type - 
                        A standard Bluetooth packet type from the following table:
                            <table>
                                <tr><td>DM1 = 3
                                <tr><td>DH1 = 4
                                <tr><td>HV1 = 5
                                <tr><td>HV2 = 6
                                <tr><td>HV3 = 7
                                <tr><td>DV = 8
                                <tr><td>AUX1 = 9
                                <tr><td>DM3 = 10
                                <tr><td>DH3 = 11
                                <tr><td>EV4 = 12
                                <tr><td>EV5 = 13
                                <tr><td>DM5 = 14
                                <tr><td>DH5 = 15
                                <tr><td>2-DH1 = 20
                                <tr><td>2-EV3 = 22
                                <tr><td>3-EV3 = 23
                                <tr><td>3-DH1 = 24
                                <tr><td>2-DH3 = 26
                                <tr><td>3-DH3 = 27
                                <tr><td>2-EV5 = 28
                                <tr><td>3-EV5 = 29
                                <tr><td>2-DH5 = 30
                                <tr><td>3-DH5 = 31
                            </table>
                        <p>Any other number sets the default, which is DH1. In this
                        case the size is also set to the maximum for a DH1 packet.

                    size - 
                        The size of the data payload in each packet, from 1 to the
                        maximum for the type. The maximum sizes for each type (taken 
                        from the BlueTooth specification) are:
                            <table>
                                <tr><td>DM1 = 17
                                <tr><td>DH1 = 27
                                <tr><td>HV1 = 10
                                <tr><td>HV2 = 20
                                <tr><td>HV3 = 30
                                <tr><td>DV = 19
                                <tr><td>AUX1 = 29
                                <tr><td>DM3 = 121
                                <tr><td>DH3 = 183
                                <tr><td>EV4 = 120
                                <tr><td>EV5 = 180
                                <tr><td>DM5 = 224
                                <tr><td>DH5 = 339
                                <tr><td>2-DH1 = 54
                                <tr><td>2-EV3 = 60
                                <tr><td>3-EV3 = 90
                                <tr><td>3-DH1 = 83
                                <tr><td>2-DH3 = 367
                                <tr><td>3-DH3 = 552
                                <tr><td>2-EV5 = 360
                                <tr><td>3-EV5 = 540
                                <tr><td>2-DH5 = 679
                                <tr><td>3-DH5 = 1021
                            </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets packet type and size for transmitter tests. An invalid 
                    packet type or size exceeding the maximum for the specified
                    type will result in an error.

    Example :

        uint32 rxHandle = 0;
        uint32 txHandle = 0;
        int32 timeout = 0;
        int32 hopping = 0;

        do
        {
            cout << "Trying rxHandle..." << endl;
            rxHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(rxHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying txHandle..." << endl;
            txHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(txHandle == 0 && timeout<MAX_TIMEOUT);

        if((txHandle != 0) && (rxHandle != 0))
        {
            uint16 dut, ref;
            int32 success = 0;

            do
            {

                success = bccmdGetBuildId(rxHandle, &dut);
                if(success == TE_OK)
                    cout << "DUT firmware = " << dut << endl;
                else
                    break;

                success = bccmdGetBuildId(txHandle, &ref);
                if(success == TE_OK)
                    cout << "REF firmware = " << ref << endl;
                else
                    break;

                // 1. configure the packet types
                success = radiotestCfgPkt(rxHandle, 15, 339);
                if(success != TE_OK)
                    break;

                success = radiotestCfgPkt(txHandle, 15, 339);
                if(success != TE_OK)
                    break;

                // 2. Set the TX/RX interval
                success = radiotestCfgFreq(rxHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;


                success = radiotestCfgFreq(txHandle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                uint16 uiFreq = 2441;
                uint16 uiIntPA = 50;
                uint16 uiExtPA = 0;
                uint16 uiCC = 0;
                uint8 uiHiside = 0;
                uint16 uiRxAtt = 0;
                uint16 uiSampleSize = 10000;

                // 3. Start the transmitter and receiver
                success = radiotestBerLoopback(txHandle, 2441, uiIntPA, uiExtPA, 
                                                uiSampleSize);
                if(success != TE_OK)
                    break;

                success = radiotestLoopback(rxHandle, uiFreq, uiIntPA, uiExtPA);
                if(success != TE_OK)
                    break;

                int32 count = 0;
                uint32 r[9];

                // 4. Gather the BER information
                do
                {
                    count++;
                    success = hqGetBer(txHandle, 1000, r);
                }while(success == TE_ERROR && count<10);

                if(success == TE_OK)
                {
                    cout << "Bit count = " << r[0] << endl;
                    cout << "Bit errors = " << r[1] << endl;
                    cout << "Received packets = " << r[3] << endl;
                    cout << "Expected packets = " << r[4] << endl;
                    cout << "Header errors = " << r[5] << endl;
                    cout << "CRC errors = " << r[6] << endl;
                    cout << "Uncorrected errors = " << r[7] << endl;
                    cout << "Sync errors = " << r[8] << endl;
                }
                else
                    cout << "****FAIL****" << endl;

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(txHandle != 0)
            closeTestEngine(txHandle);
        if(rxHandle != 0)
            closeTestEngine(rxHandle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgPkt(uint32 handle, uint16 type, uint16 size);

/*************************************************************************************
    
    Function :      int32 radiotestCfgBitError(uint32 handle, uint32 sampleSize, 
                                                uint8 reset)

    Parameters :    handle - 
                        Handle to the device

                    sampleSize - 
                        If non-zero, the target for total counters is set to this and 
                        total count resets at this value. If passed as 0 the current 
                        value is unchanged. 
                    
                    reset - 
                        Boolean value (0 = False, anything else = True). If True, and 
                        if Ber1, Ber2, or BerLoopback is active, immediately resets 
                        the counters for the total statistics, but not over the last 
                        report period.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Can be used to set the sample size used in bit error measurements, 
                    and reset the counters if required.
                    <p>Note that the radiotestBer1, radiotestBer2, and 
                    radiotestBerLoopback functions also take a sampleSize parameter 
                    and set the value before running the test. Therefore there is 
                    normally no need to call this function, unless a reset of the 
                    counters is required.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgBitError(uint32 handle, uint32 sampleSize, 
                                            uint8 reset);

/*************************************************************************************
    
    Function :      int32 radiotestCfgTxPaAtten(uint32 handle, uint16 atten)

    Parameters :    handle - 
                        Handle to the device

                    atten - 
                        Attenuation value to set. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the TX PA attenuation value used during transmission.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgTxPaAtten(uint32 handle, uint16 atten);

/*************************************************************************************
    
    Function :      int32 radiotestCfgXtalFtrim(uint32 handle, uint16 ftrim)

    Parameters :    handle - 
                        Handle to the device

                    ftrim - 
                        A number between 0 and 63 inclusive. This is not a permanent 
                        change.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Timing for BlueCore is controlled by a crystal. This requires
                    trimming for new hardware. This command can be used to set a new 
                    trim value either before a radiotest command is started or while 
                    a test is already in operation; the change takes effect 
                    immediately.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgXtalFtrim(uint32 handle, uint16 ftrim);

/*************************************************************************************
    
    Function :      int32 radiotestCfgUapLap(uint32 handle, uint16 uap, uint32 lap)

    Parameters :    handle - 
                        Handle to the device.

                    uap - 
                        UAP portion of the device address (0 to 0xff).

                    lap - 
                        LAP portion of the device address (0 to 0xffffff).

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the UAP and LAP to be used in radio tests. BlueCore usually 
                    uses its own Bluetooth Device address to determine the access sync 
                    code, as if it is master of a piconet.
                    The UAP and LAP are the only parts used.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgUapLap(uint32 handle, uint16 uap, uint32 lap);

/*************************************************************************************
    
    Function :      int32 radiotestCfgIqTrim(uint32 handle, uint16 trim)

    Parameters :    handle - 
                        Handle to the device

                    trim - 
                        Value to be written to the register, ANA_IQ_TRIM.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   The IQ block may be trimmed by setting a register, ANA_IQ_TRIM.
                    This allows the variable to be set for the test modes.  The value
                    "trim", 0 to 511 inclusive, takes effect immediately, is not 
                    saved, and must be reset on each test, as the IQ is calibrated at 
                    the start.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgIqTrim(uint32 handle, uint16 trim);

/*************************************************************************************
    
    Function :      int32 radiotestCfgTxIf(uint32 handle, int16 offset)

    Parameters :    handle - 
                        Handle to the device

                    offset - 
                        Value to be written to the register, ANA_IQ_TRIM.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   The bc01 transmitter uses an additional offset in the IF part of 
                    the transmitter, which needs to be configured for optimum 
                    performance. The target is to be able to set this to zero. 
                    However, the stack currently uses an offset of 1MHz 1.5MHz.
                    "offset_half_mhz" sets the value of this in units of 0.5MHz for 
                    the radiotests. The default is the value used in standard 
                    operation, currently 3 units, i.e. 1.5MHz offset.  Note this is a 
                    signed integer.  The range is -5 to +5. This must be set before 
                    the start of a radio transmitter test.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgTxIf(uint32 handle, int16 offset);

/*************************************************************************************
    
    Function :      int32 radiotestCfgTxTrim(uint32 handle, uint16 am_addr)

    Parameters :    handle - 
                        Handle to the device

                    am_addr - 
                        Active member address.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the AM_ADDR used in the header of test transmissions, and
                    expected in receiving test transmissions.  It is a number between 
                    0 and 7, inclusive.  The default value is 7.  This takes effect
                    immediately and lasts until a new test is restarted, at which
                    point a default read from persistent store is used.  The defaults
                    are: PSKEY_RADIOTEST_TX_TRIM1 for PRBS data (which is assumed
                    for all reception routines), PSKEY_RADIOTEST_TX_TRIM2 for alternating
                    bits (1010...), PSKEY_RADIOTEST_TX_TRIM3 for alternating nibbles
                    (11110000).

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgTxTrim(uint32 handle, uint16 am_addr);

/*************************************************************************************
    
    Function :      int32 radiotestCfgLoLvl(uint32 handle, uint16 lo_lvl)

    Parameters :    handle - 
                        Handle to the device

                    lo_lvl - 
                        Analogue LO output level

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command sets the value of the Analogue Local Oscillator 
                    output level to lvl, over-riding the value calculated by the 
                    internal calibration algorithm.
                    <p>Important Note:
                        Use of this command is unnecessary for all normal users.

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgLoLvl(uint32 handle, uint16 lo_lvl);

/*************************************************************************************
    
    Function :      int32 radiotestCfgHoppingSeq(uint32 handle, uint16* channels)

    Parameters :    handle - 
                        Handle to the device

                    channels - 
                        Pointer to an array of 5 uint16 values. The 5 values are bit 
                        fields used to enable / disable each of the 79 channels. If a 
                        bit = 1, the channel is used, otherwise it is not used. The 
                        array elements map to channels as follows:
                        <table>
                            <tr><td>0 = Channels 0-15
                            <tr><td>1 = Channels 16-31
                            <tr><td>2 = Channels 32-47
                            <tr><td>3 = Channels 48-63
                            <tr><td>4 = Channels 64-78 (bit 15 unused)
                        </table>
                        For each uint16 element, bit 0 (LSB) is for the lowest channel 
                        number in the range, i.e. bit 0 of the first uint16 is for 
                        channel 0.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Configures the set of hop frequencies to be used with 
                    radiotestRxdata2, radiotestTxdata2 and radiotestBer2. The 
                    functionality is analogous to the hciSetAfhHostChannelClass 
                    function. The adjusted hopping sequence allows use of these 
                    radiotest modes while simulating a reduced hop set. 
                    <p>The hopping algorithm is the same as that used when all 
                    frequencies are in use, but only on the selected frequencies. 
                    This guarantees that the radio spends equal amounts of time on 
                    each selected frequency.

    Example :

        uint32 handle = 0;
        int32 timeout = 0;

        do
        {
            cout << "Attempting to open connection..." << endl;
            handle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(handle == 0 && timeout<MAX_TIMEOUT);

        if(handle != 0)
        {
            int32 success = 0;

            do
            {
                // Configure the packet type
                success = radiotestCfgPkt(handle, 15, 339);
                if(success != TE_OK)
                    break;

                // Set the TX/RX interval
                success = radiotestCfgFreq(handle, 37500, 9375, 1);
                if(success != TE_OK)
                    break;

                // Use lowest 20 channels only
                uint16 channels[5] = { 0xffff, 0x000f, 0x0000, 0x0000, 0x0000 };
                success = radiotestCfgHoppingSeq(handle, channels);
                if(success != TE_OK)
                    break;

                // Start transmission
                success = radiotestTxdata2(handle, 0, 50, 0);
                if(success != TE_OK)
                    break;

                // Perform tests

            }while(0);

            if(success != TE_OK)
                cout << "TestEngine error" << endl;

        }
        if(handle != 0)
            closeTestEngine(handle);

*************************************************************************************/
TESTENGINE_API(int32) radiotestCfgHoppingSeq(uint32 handle, uint16* channels);

/*************************************************************************************
    MISC TESTS
*************************************************************************************/

/*************************************************************************************
    
    Function :      int32 radiotestSettle(uint32 handle, uint16 start, uint16 end)

    Parameters :    handle - 
                        Handle to the device.

                    start - 
                        Channel to start stepping from. 0 to 78.

                    end - 
                        Channel to finish stepping at. 0 to 78.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Builds the radio's channel LO_TRIM frequency lookup table (LUT), 
                    then does a step from the start channel to the end channel, while 
                    the synthesiser is running. It digitises the synthesiser (LO_TUNE) 
                    error voltage at intervals of 10 | 20us over the next 200us.
                    <p>The results can be retrieved using hqGetSettle.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) radiotestSettle(uint32 handle, uint16 start, uint16 end);

/*************************************************************************************
    
    Function :      int32 hqGetSettle(uint32 handle, uint16 * r)

    Parameters :    handle - 
                        Handle to the device.

                    r - 
                        Pointer to a preallocated 9 element array to receive the 
                        results.

    Returns :        <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to retrieve the results made from a call to 
                    radiotestSettle.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) hqGetSettle(uint32 handle, uint16 * r);

/*************************************************************************************
    
    Function :      int32 get_freq_offset(uint32 handle, float64 * offset, 
                                            int32 sample_size)

    Parameters :    handle - 
                        Handle to the device

                    offset - 
                        Pointer to the offset. Measured in parts per million (PPM), 
                        this value is an average of sample_size readings from the 
                        BlueCore receiver.
                
                    sample_size - 
                        Determines how many readings of offset are averaged. The 
                        software will only accept values in the range of 20 to 200, 
                        any values outside this range will be forced up to 20 or 200. 
                        The choice of value will determine the accuracy and speed. 
                        The lower the value the quicker but less accurate the results, 
                        the higher the value the more accurate but slower the function. 
                        The optimal value is 60 to 100.

    Returns :       -1 on error, 1 on success, 0 on fail

    Description :   This command will read the frequency offset of a received signal.

*************************************************************************************/
TESTENGINE_API(int32) get_freq_offset(uint32 handle, float64 * offset, 
                                        int32 sample_size);

/*************************************************************************************
    PS ACCESS COMMANDS
*************************************************************************************/

/*************************************************************************************
    PS_STORES - the BlueCore persistent store is conceptually a four layer model. When 
    the BlueCore firmware initialises or reads a persistent store value it will always 
    read it from the top most layer where that PS entry is present. When writing or 
    reading to the persistent store in production it is possible to select the PS 
    store layer or let the firmware decide. This selection is carried out by the use of
    the 'stores' parameter of all the PS read and write functions. This parameter is a 
    bit mask so multiple layers can be selected. The layers are defined as follows 
    (bottom to top):

        1. ROM layer - the bottom layer in the model, basically these are the hard 
        coded firmware defaults. This is a read-only layer so all PS writes to this 
        layer will fail. Note that not all PS entries will have a default value so in 
        some cases a PS read will also fail.

        2. Factory-set layer - this layer is not present on BlueCore ROM devices unless
        an external EEPROM is present. PS keys are moved to this layer when 
        "psFactorySet()" is called. This layer can also be written to and read from 
        directly by selecting this layer as the 'stores' parameter.

        3. Implementation - this layer is not present on BlueCore ROM devices unless
        an external EEPROM is present. This layer is where most PS keys are written 
        during production for BlueCore Flash, External and ROM devices with an EEPROM.
        In these cases calling "psFactorySet()" will move the PS values from this 
        layer to the factory-set layer. Calling "psFactoryRestoreAll()" will erase 
        the implemention layer and expose the factory-set layer. When using BlueCore 
        Flash, External and ROM devices with an EEPROM if this layer is selected then
        the function will fail if the flash or EEPROM is read-only, for example if 
        there is a fault on the flash write-enable.

        4. Transient - this layer is the BlueCore RAM. By definition a cold reset or 
        power down reset will clear this layer. A warm reset will retain this layer.
        This layer exists on all BlueCore devices.

    The bitmasks to define these layers are shown below. If no bits are set (0), then
    the firmware will select the layer. If an external storage device is present and 
    functioning correctly then the firmware will write to the implemention layer (this 
    includes BlueCore Flash), otherwise the firmware will write to the transient layer.
**************************************************************************************/
#define PS_STORES_DEFAULT (0x0000)    /* default */
#define PS_STORES_I       (0x0001)    /* implementation (normal) */
#define PS_STORES_F       (0x0002)    /* factory-set */
#define PS_STORES_R       (0x0004)    /* rom (read-only) */
#define PS_STORES_T       (0x0008)    /* transient (RAM) */

#define PS_STORES_FR      (0x0006)
#define PS_STORES_IF      (0x0003)
#define PS_STORES_IFR     (0x0007)

/*************************************************************************************
    
    Function :      int32 bccmdSetEeprom(uint32 handle, uint16 log2bytes, 
                                        uint16 addrMask)

    Parameters :    handle - 
                        Handle to the device

                    log2bytes - 
                        Specifies the size of the EEPROM (log2 bytes). That is the 
                        number of bits required to address any byte of the memory 
                        array.

                    addrMask - 
                        A bitmap specifying which, if any, of the slave address bits 
                        are used to specify high-order bits of the array address.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes the header to the I2C EEPROM to allow the device to be used 
                    to store PS values and VM applications.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetEeprom(uint32 handle, uint16 log2bytes, uint16 addrMask);

/*************************************************************************************
    
    Function :      int32 psReadBdAddr(uint32 handle, uint32 * lap, uint8 * uap, 
                                        uint16 * nap)

    Parameters :    handle - 
                        Handle to the device
                    
                    lap - 
                        Pointer to a uint32 to hold the LAP portion of the BD_ADDR
                    
                    uap - 
                        Pointer to a uint8 to hold the UAP portion of the BD_ADDR

                    nap - 
                        Pointer to a uint16 to hold the NAP portion of the BD_ADDR

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Function to read the BD_ADDR from the persistent store.

*************************************************************************************/
TESTENGINE_API(int32) psReadBdAddr(uint32 handle, uint32 * lap, uint8 * uap, 
                                    uint16 * nap);

/*************************************************************************************
    
    Function :      int32 psRead(uint32 handle, uint16 ps_key, uint16 store, 
                                uint16 arraylen, uint16 * ps, uint16 * len)

    Parameters :    handle - 
                        Handle to the device

                    ps_key - 
                        PS key ID of the value to be read.
                    
                    store - 
                        Bit mask to identify the PS store layer to write to. See 
                        comments above under PS_STORES.

                    arraylen - 
                        Size of the array allocated to hold the PS value

                    ps - 
                        Array of 16-bit words of size "arraylen" to hold the PS value

                    len - 
                        Pointer to a 16-bit word to hold to length of the PS key

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Used to read the value of the PS key specified

*************************************************************************************/
TESTENGINE_API(int32) psRead(uint32 handle, uint16 ps_key, uint16 store, 
                            uint16 arraylen, uint16 * ps, uint16 * len);

/*************************************************************************************
    
    Function :      int32 psClear(uint32 handle, uint16 ps_key, uint16 store)

    Parameters :    handle - 
                        Handle to the device

                    ps_key - 
                        PS key value of the entry to be removed

                    store - 
                        Bit mask to identify the PS store layer(s) from which the PS 
                        entry will be erased.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function removes from the ps the value stored in the layer 
                    specified by the stores parameter. Clearing values from the ps 
                    can have subtle effects. The description of the Layered Model of 
                    the ps stores above shows how clearing a value from the psfl 
                    layer can (re)expose a value in the psf or psrom layer.

*************************************************************************************/
TESTENGINE_API(int32) psClear(uint32 handle, uint16 ps_key, uint16 store);

/*************************************************************************************
    
    Function :      int32 psClearAll(uint32 handle, uint16 store)

    Parameters :    handle - 
                        Handle to the device

                    store - 
                        Bit mask to identify the PS store layer(s) from which the PS 
                        entries will be erased.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This removes all values from the ps flash; only values in the 
                    psrom layer remain. This command should be used with caution.

*************************************************************************************/
TESTENGINE_API(int32) psClearAll(uint32 handle, uint16 store);

/*************************************************************************************
    
    Function :      int32 psFactorySet(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This moves all values in the psfl layer to the psf layer.

*************************************************************************************/
TESTENGINE_API(int32) psFactorySet(uint32 handle);

/*************************************************************************************
    
    Function :      int32 psFactoryRestoreAll(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command removes all values from the psfl store, so restoring 
                    the chip�s ps to the state it was in when the PS_FACTORY_SET 
                    command was invoked.

*************************************************************************************/
TESTENGINE_API(int32) psFactoryRestoreAll(uint32 handle);

/*************************************************************************************
    
    Function :      int32 psFactoryRestore(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This is identical to PS_FACTORY_RESTORE_ALL except that values in 
                    the psfl layer that do not obscure values in the psf and/or psrom 
                    layers are left in place.

*************************************************************************************/
TESTENGINE_API(int32) psFactoryRestore(uint32 handle);

/*************************************************************************************
    
    Function :      int32 psSize(uint32 handle, uint16 pskey, uint16 store, 
                                uint16 * size)

    Parameters :    handle - 
                        Handle to the device

                    pskey - 
                        PS entry to return the size of.
    
                    store - 
                        PS store to access

                    size - 
                        Reference to size of PS entry.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Returns the internal amount of data, in uint16's, held by a 
                    particular PS entry. Failure may indicate that the the store mask 
                    selected does not contain a valid PS entry.

*************************************************************************************/
TESTENGINE_API(int32) psSize(uint32 handle, uint16 pskey, uint16 store, uint16 * size);

/*************************************************************************************
    
    Function :      int32 psWrite(uint32 handle, uint16 ps_key, uint16 store, 
                                uint16 ps_length, uint16 const * const ps)

    Parameters :    handle - 
                        Handle to the device

                    ps_key - 
                        PS key value of the entry to be written

                    store - 
                        PS store to access

                    ps_length - 
                        Length of the data to be written.

                    ps - 
                        Pointer to an array to hold the key contents

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes the PS setting "ps_key" array "ps" of length "ps_length" 
                    to the persistent store layer identified by "store".

*************************************************************************************/
TESTENGINE_API(int32) psWrite(uint32 handle, uint16 ps_key, uint16 store, 
                            uint16 ps_length, uint16 const * const ps);

/*************************************************************************************
    
    Function :      int32 psWriteVerify(uint32 handle, uint16 ps_key, uint16 store, 
                                        uint16 ps_length, uint16 const * const ps)

    Parameters :    handle - 
                        Handle to the device

                    ps_key - 
                        PS key value of the entry to be written

                    store - 
                        PS store to access

                    ps_length - 
                        Length of the data to be written.

                    ps - 
                        Pointer to an array to hold the key contents

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes the PS setting "ps_key" array "ps" of length "ps_length" 
                    to the persistent store layer PS_STORE_I or PS_STORE_F if one 
                    those values are identified by "store". The function will then 
                    perform a PS read and verify that the PS key has been written 
                    correctly.
                    <p>This function will fail if a store mask does not contain 
                    PS_STORE_I or PS_STORE_F.

*************************************************************************************/
TESTENGINE_API(int32) psWriteVerify(uint32 handle, uint16 ps_key, uint16 store, 
                                    uint16 ps_length,  uint16 const * const ps);

/*************************************************************************************
    
    Function :      int32 psWriteBdAddr(uint32 handle, uint32 lap, uint32 uap, 
                                        uint32 nap)

    Parameters :    handle - 
                        Handle to the device.

                    lap - 
                        LAP portion of the address to write to the persistent store.

                    uap - 
                        UAP portion of the address to write to the persistent store.

                    nap - 
                        NAP portion of the address to write to the persistent store.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to write the BlueTooth address of the
                    device.

*************************************************************************************/
TESTENGINE_API(int32) psWriteBdAddr(uint32 handle, uint32 lap, uint32 uap, uint32 nap);

/*************************************************************************************
    
    Function :      int32 psReadModuleId(uint32 handle, uint32 * module_id)

    Parameters :    handle - 
                        Handle to the device

                    module_id - 
                        Pointer to a variable that will hold the returned module ID.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will attempt to read the module ID stored in 
                    BlueCore. The default for this value is 0 (zero)

*************************************************************************************/
TESTENGINE_API(int32) psReadModuleId(uint32 handle, uint32 * module_id);

/*************************************************************************************
    
    Function :      int32 psReadXtalFtrim(uint32 handle, uint16 * ftrim)

    Parameters :    handle - 
                        Handle to the device

                    ftrim - 
                        Pointer to a variable that will hold the returned xtal ftrim.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will attempt to read the xtal ftrim stored in 
                    BlueCore.

*************************************************************************************/
TESTENGINE_API(int32) psReadXtalFtrim(uint32 handle, uint16 * ftrim);

/*************************************************************************************
    
    Function :      int32 psWriteXtalFtrim(uint32 handle, uint16 ftrim)

    Parameters :    handle - 
                        Handle to the device

                    ftrim - 
                        Crystal frequency trim

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write the crystal trim value to the persistent 
                    store.

*************************************************************************************/
TESTENGINE_API(int32) psWriteXtalFtrim(uint32 handle, uint16 ftrim);

/*************************************************************************************
    
    Function :      int32 psWriteModuleSecurityCode(uint32 handle, 
                                                    uint16 const * const code)

    Parameters :    handle - 
                        Handle to the device

                    code - 
                        Pointer to 8 member unsigned 16 bit integer array holding the 
                        128 bit module security code.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write the module security code to the 
                    persistent store.

*************************************************************************************/
TESTENGINE_API(int32) psWriteModuleSecurityCode(uint32 handle, 
                                                uint16 const * const code);

/*************************************************************************************
    
    Function :      int32 psWriteModuleId(uint32 handle, uint32 module_id)

    Parameters :    handle - 
                        Handle to the device

                    module_id - 
                        Module ID to be stored in BlueCore persistent store

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will attempt to write the module ID to BlueCore. 
                    The default for this value is 0 (zero).

*************************************************************************************/
TESTENGINE_API(int32) psWriteModuleId(uint32 handle, uint32 module_id);

/*************************************************************************************
    
    Function :      int32 psWriteBaudrate(uint32 handle, uint16 value)

    Parameters :    handle - 
                        Handle to the device

                    value - 
                        Used determine the baudrate where: 
                            baudrate = value/0.004096

                        <p>Some common values are:
                        <table>
                            <tr><td> 38k4    baud    -   157   (0x009d)
                            <tr><td> 57k6    baud    -   236   (0x00ec)
                            <tr><td> 115k2   baud    -   472   (0x01d8)
                            <tr><td> 230k4   baud    -   944   (0x03b0)
                            <tr><td> 460k8   baud    -   1887  (0x075f)
                            <tr><td> 921k6   baud    -   3775  (0x0ebf)
                            <tr><td> 1382k4  baud    -   5662  (0x161e)
                        </table>
                        The maximum rated speed for the UART hardware is 1.5 Mbaud, 
                        although this key can be set to a higher value.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a value corresponding to the baudrate to 
                    the persistent store. This function can only be used in builds 
                    later than 18.x. For earlier builds the 
                    PSKEY_HOSTIO_UART_PS_BLOCK should be modified.

*************************************************************************************/
TESTENGINE_API(int32) psWriteBaudrate(uint32 handle, uint16 value);

/*************************************************************************************
    
    Function :      int32 psWriteRadiotestFirstTrimTime(uint32 handle, uint32 time)

    Parameters :    handle - 
                        Handle to the device

                    time - 
                        The time from the start of a test till the first IQ trim 
                        auto-calibration is done. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write the time to the persistent store.A value 
                    of 0 disables the automatic calibration completely during the 
                    test. A calibration is still done before the test starts. 

*************************************************************************************/
TESTENGINE_API(int32) psWriteRadiotestFirstTrimTime(uint32 handle, uint32 time);

/*************************************************************************************
    
    Function :      int32 psReadRadiotestFirstTrimTime(uint32 handle, uint32 * time)

    Parameters :    handle - 
                        Handle to the device

                    time - 
                        The time from the start of a test till the first IQ trim 
                        auto-calibration is done. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the trim time from the persistent store.

*************************************************************************************/
TESTENGINE_API(int32) psReadRadiotestFirstTrimTime(uint32 handle, uint32 * time);

/*************************************************************************************
    
    Function :      int32 psWriteRadiotestLoLvlTrimEnable(uint32 handle, uint16 state)

    Parameters :    handle - 
                        Handle to the device

                    state - 
                        Whether IQ trim is enabled. Basically this is a boolean 
                        where 1 = true and 0 = false.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function enables trimming of the value for the register 
                    ANA_LO_ENABLE; otherwise, has no effect. This trimming takes place 
                    before the start of each test.  

*************************************************************************************/
TESTENGINE_API(int32) psWriteRadiotestLoLvlTrimEnable(uint32 handle, uint16 state);

/*************************************************************************************
    
    Function :      int32 psReadRadiotestLoLvlTrimEnable(uint32 handle, uint16 * state)

    Parameters :    handle - 
                        Handle to the device

                    state - 
                        Whether IQ trim is enabled. Basically this is a boolean 
                        where 1 = true and 0 = false.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads value of PS setting

*************************************************************************************/
TESTENGINE_API(int32) psReadRadiotestLoLvlTrimEnable(uint32 handle, uint16 * state);

/*************************************************************************************
    
    Function :      int32 psWriteRadiotestSubsequentTrimTime(uint32 handle, uint32 time)

    Parameters :    handle - 
                        Handle to the device

                    time - 
                        The time between each IQ trim auto-calibration.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Once the first IQ trim auto calibration is done in test mode, then 
                    subsequent trims are done at regular intervals with the period 
                    being given by this key. If this key is 0 then only the first trim 
                    is done. If PSKEY_RADIOTEST_FIRST_TRIM_TIME is 0 then no trims are 
                    done during the test regardless of the setting of this key. 

*************************************************************************************/
TESTENGINE_API(int32) psWriteRadiotestSubsequentTrimTime(uint32 handle, uint32 time);

/*************************************************************************************
    
    Function :      int32 psReadRadiotestSubsequentTrimTime(uint32 handle, 
                                                            uint32 * time)

    Parameters :    handle - 
                        Handle to the device

                    time - 
                        The time between each IQ trim auto-calibration.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Read the PS setting

*************************************************************************************/
TESTENGINE_API(int32) psReadRadiotestSubsequentTrimTime(uint32 handle, uint32 * time);

/*************************************************************************************
    
    Function :      int32 psWriteHostInterface(uint32 handle, uint16 transport)

    Parameters :    handle - 
                        Handle to the device

                    transport - 
                        The choice of physical bus. Valid values are:
                        <table>
                            <tr><td> 1 : BCSP 
                            <tr><td> 2 : H2 - USB 
                            <tr><td> 3 : H4 - UART 
                            <tr><td> 4 : Raw UART (VM)
                            <tr><td> 6 : H5
                            <tr><td> 7 : H4DS
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Select the interface type that is to be used to pass information 
                    to and from the host. Only one of these can be used at a time, 
                    not least because the USB interface uses some of the same chip 
                    pins as the UART. If this key specifies use of USB and the chip 
                    has no USB hardware support then it reverts to using BCSP. Much 
                    of the chip's support for BCSP/H5 is performed by hardware - this 
                    allows interrupt routines to deal with complete BCSP/H5 packets. 
                    When the chip uses its UART in a more naive fashion, e.g., for 
                    H4, this support must be disabled. The UART's hardware support 
                    for BCSP/H5 must be enabled via PSKEY_UART_CONFIG_BCSP/H5. The H4 
                    specification requires the use of UART hardware flow control and 
                    no use of parity. These must also be set via PSKEY_UART_CONFIG_H4. 

*************************************************************************************/
TESTENGINE_API(int32) psWriteHostInterface(uint32 handle, uint16 transport);

/*************************************************************************************
    
    Function :      int32 psReadHostInterface(uint32 handle, uint16 * transport)

    Parameters :    handle - 
                        Handle to the device

                    transport - 
                        The selected physical bus. Valid values are:
                        <table>
                            <tr><td> 1 : BCSP 
                            <tr><td> 2 : H2 - USB 
                            <tr><td> 3 : H4 - UART 
                            <tr><td> 4 : Raw UART (VM)
                            <tr><td> 6 : H5
                            <tr><td> 7 : H4DS
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the Host interface PS setting

*************************************************************************************/
TESTENGINE_API(int32) psReadHostInterface(uint32 handle, uint16 * transport);

/*************************************************************************************
    
    Function :      int32 psWriteUsbAttributes(uint32 handle, uint16 bmAttributes)

    Parameters :    handle - 
                        Handle to the device

                    bmAttributes - 
                        The bmAttributes of the Standard Configuration Descriptor, 
                        described in Table 9.8 of USB specification, version 1.1. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Configuration characteristics
                    <table>                        
                        <tr><td> D7: Reserved (set to one)
                        <tr><td> D6: Self-powered
                        <tr><td> D5: Remote Wakeup
                        <tr><td> D4...0: Reserved (reset to zero)
                    </table>
                    D7 is reserved and must be set to one for historical reasons. A 
                    device configuration that uses power from the bus and a local 
                    source reports a non-zero value in MaxPower to indicate the 
                    amount of bus power required and sets D6. The actual power 
                    source at runtime may be determined using the GetStatus(DEVICE) 
                    request (see Section 9.4.5 of USB specification 1.1). If a device 
                    configuration supports remote wakeup, D5 is set to one. 

*************************************************************************************/
TESTENGINE_API(int32) psWriteUsbAttributes(uint32 handle, uint16 bmAttributes);

/*************************************************************************************
    
    Function :      int32 psWriteDPlusPullup(uint32 handle, uint16 pio_pin)

    Parameters :    handle - 
                        Handle to the device

                    pio_pin - 
                        The PIO line used to control the pull-up resistor on the USB 
                        D+ line. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   The absence of this key in the ps indicates that this feature is 
                    not in use. This value is only used if the chip is presenting its 
                    USB interface. See the description of PSKEY_HOST_INTERFACE.

*************************************************************************************/
TESTENGINE_API(int32) psWriteDPlusPullup(uint32 handle, uint16 pio_pin);

/*************************************************************************************
    
    Function :      int32 psWriteUsbMaxPower(uint32 handle, uint16 max_power)

    Parameters :    handle - 
                        Handle to the device

                    max_power - 
                        The MaxPower field of the local USB device, as defined in 
                        Table 9.8 of version 1.1 of the USB specification. Bizarrely, 
                        this value is given in units of 2mA. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Function to write the pskey PSKEY_USB_MAX_POWER to the persistent
                    store. This value is only used if the chip is presenting its USB 
                    interface. See the description of PSKEY_HOST_INTERFACE.

*************************************************************************************/
TESTENGINE_API(int32) psWriteUsbMaxPower(uint32 handle, uint16 max_power);

/*************************************************************************************
    
    Function :      int32 psWritePioProtectMask(uint32 handle, uint16 mask)

    Parameters :    handle - 
                        Handle to the device
    
                    mask - 
                        PIO mask to protect PIO lines from unwanted access. If a bit 
                        of the pskey's value is set high then application code cannot 
                        change the value of the corresponding PIO port pin. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   The chip's PIO port can function as a general purpose input and 
                    output port. However, various optional hardware and software 
                    configurations require some PIO pins for their own needs, e.g., an 
                    optional external PA/LNA block is driven using PIO pins 0 and 1. 
                    This pskey prevents applications changing PIO pins that are 
                    required for lower level functions. This blocking action applies 
                    to host-based applications requesting PIO changes via BCCMD and to 
                    VM applications running on the chip. Bits 0 to 7 of this uint16 
                    pskey map to PIO0 to PIO7 respectively. If a bit of the pskey's 
                    value is set high then application code cannot change the value of 
                    the corresponding PIO port pin. The default value, 0x03, suits 
                    Casira as this has an external PA/LNA. 

*************************************************************************************/
TESTENGINE_API(int32) psWritePioProtectMask(uint32 handle, uint16 mask);

/*************************************************************************************
    
    Function :      int32 psReadPioProtectMask(uint32 handle, uint16 * mask)

    Parameters :    handle - 
                        Handle to the device

                    mask - 
                        PIO mask to protect PIO lines from unwanted access. If a bit 
                        of the pskey's value is set high then application code cannot 
                        change the value of the corresponding PIO port pin. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the PIO protect mask PS setting

*************************************************************************************/
TESTENGINE_API(int32) psReadPioProtectMask(uint32 handle, uint16 * mask);

/*************************************************************************************
    
    Function :      int32 psWriteTxOffsetHalfMhz(uint32 handle, int16 offset)

    Parameters :    handle - 
                        Handle to the device

                    offset - 
                        Transmit offset in units of 500kHz. For example -2 == -1MHz

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the transmit offset.

*************************************************************************************/
TESTENGINE_API(int32) psWriteTxOffsetHalfMhz(uint32 handle, int16 offset);

/*************************************************************************************
    
    Function :      int32 psReadTxOffsetHalfMhz(uint32 handle, int16 * offset)

    Parameters :    handle - 
                        Handle to the device

                    offset - 
                        Transmit offset in units of 500kHz. For example -2 == -1MHz

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the transmit offset.

*************************************************************************************/
TESTENGINE_API(int32) psReadTxOffsetHalfMhz(uint32 handle, int16 * offset);

/*************************************************************************************
    
    Function :      int32 psWriteUsrValue(uint32 handle, uint16 user_no, uint16 length, 
                                            uint16 const * const entry)

    Parameters :    handle - 
                        Handle to the device

                    user_no - 
                        Value between 0 and 49 which refers to 'n' in PSKEY_USRn

                    length - 
                        Length of PS key value in uint16's

                    entry - 
                        Pointer to an array containing the data to be written to the 
                        PS.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes to PSKEY_USRn.

*************************************************************************************/
TESTENGINE_API(int32) psWriteUsrValue(uint32 handle, uint16 user_no, uint16 length, 
                                        uint16 const * const entry);

/*************************************************************************************
    
    Function :      int32 psReadUsrValue(uint32 handle, uint16 user_no, uint16 maxLen, 
                                        uint16 * length, uint16 * entry)

    Parameters :    handle - 
                        Handle to the device

                    user_no - 
                        Value between 0 and 49 which refers to 'n' in PSKEY_USRn

                    maxLen - 
                        Length of the array to hold the returned value

                    length - 
                        Pointer to the length of the returned ps entry

                    entry - 
                        Pointer to an array to hold the returned PS value

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the PSKEY_USRn.

*************************************************************************************/
TESTENGINE_API(int32) psReadUsrValue(uint32 handle, uint16 user_no, uint16 maxLen, 
                                    uint16 * length, uint16 * entry);

/*************************************************************************************
    
    Function :      int32 psWritePowerTable(uint32 handle, uint16 num_entries, 
                                            uint8 const * const intPA, 
                                            uint8 const * const extPA, 
                                            int32 const * const power)

    Parameters :    handle - 
                        Handle to the device

                    num_entries - 
                        Number of entries in the power table

                    intPA - 
                        Pointer to an array of unsigned 8 bit integers of size 
                        num_entries containing the intPA entries for the power table. 
                    
                    extPA - 
                        Pointer to an array of unsigned 8 bit integers of size 
                        num_entries containing the extPA entries for the power table.
 
                    power - 
                        Pointer to an array of signed integers of size num_entries 
                        containing the Output power entries, in dBm, for the power 
                        table. 
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes to PSKEY_LC_POWER_TABLE

*************************************************************************************/
TESTENGINE_API(int32) psWritePowerTable(uint32 handle, uint16 num_entries, 
                                        uint8 const * const intPA, 
                                        uint8 const * const extPA, 
                                        int32 const * const power);

/*************************************************************************************
    
    Function :      int32 psReadPowerTable(uint32 handle, int32 maxSize, 
                                            int32 * num_entries, uint8 * intPA, 
                                            uint8 * extPA, int32 * power)

    Parameters :    handle - 
                        Handle to the device

                    maxSize - 
                        Size of the arrays to hold the intPA, extPA and power

                    num_entries - 
                        Pointer to the number of entries in the power table

                    intPA - 
                        Pointer to an array of unsigned 8 bit integers of size 
                        num_entries containing the intPA entries for the power table. 
                    
                    extPA - 
                        Pointer to an array of unsigned 8 bit integers of size 
                        num_entries containing the extPA entries for the power table.
 
                    power - 
                        Pointer to an array of signed integers of size num_entries 
                        containing the Output power entries, in dBm, for the power 
                        table. 
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the PSKEY_LC_POWER_TABLE

*************************************************************************************/
TESTENGINE_API(int32) psReadPowerTable(uint32 handle, int32 maxSize, 
                                        int32 * num_entries, uint8 * intPA, 
                                        uint8 * extPA, int32 * power);

/*************************************************************************************
    
    Function :      int32 psWriteVmDisable(uint32 handle, uint8 vmDisable)

    Parameters :    handle - 
                        Handle to the device

                    vmDisable - 
                        Boolean value where non-zero indicates that the VM will be 
                        disabled after the next reset.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes to PSKEY_VM_DISABLE

*************************************************************************************/
TESTENGINE_API(int32) psWriteVmDisable(uint32 handle, uint8 vmDisable);

/*************************************************************************************
    
    Function :      int32 psReadVmDisable(uint32 handle, uint8 * vmDisable)

    Parameters :    handle - 
                        Handle to the device

                    vmDisable - 
                        Boolean value where non-zero indicates that the VM is 
                        disabled.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Writes to PSKEY_VM_DISABLE

*************************************************************************************/
TESTENGINE_API(int32) psReadVmDisable(uint32 handle, uint8 * vmDisable);

/*************************************************************************************
    HCI FUNCTION SET
*************************************************************************************/

/*************************************************************************************
    
    Function :      int32 hciSlave(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Performs four steps:
                    1. Sets the event filter to auto accept a connection request. 
                    Implements the command HCI_Set_Event_Filter with the following
                    parameters:
                    <table>
                        <tr><td> 0x02 - Connection  setup filter type
                        <tr><td> 0x00 - Allow connections from all devices
                        <tr><td> 0x02 - Auto accept the connection
                    </table>
                    
                    2. Implements the command HCI_WriteScan_Enable with the following
                    parameter:
                        0x03 - Inquiry scan enabled, Page scan enabled.

    Example :
        
        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            int32 iResetError = 0;  

            do
            {
                cout << "Connected!" << endl;

                int32 iSuccess = psWriteVmDisable(iHandle, 1);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

                iSuccess = hciSlave(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciSlave error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmSlave instead" 
                             << endl;
                    }
                    break;
                }

                iSuccess = hciEnableDeviceUnderTestMode(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciEnableDeviceUnderTestMode error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmEnableDeviceUnderTestMode instead"
                             << endl;
                    }
                    break;
                }

                // PERFORM TESTING HERE.....
                cout << "TESTING.........." << endl;

                iSuccess = psWriteVmDisable(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

            }while(0);

            if(!iResetError)
            {
                closeTestEngine(iHandle);
            }
        }


*************************************************************************************/
TESTENGINE_API(int32) hciSlave(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hciSetAfhHostChannelClass(uint32 handle, 
                                                    uint8 const * const cClass)

    Parameters :    handle - 
                        Handle to the BlueCore device.

                    cClass - 
                        Pointer to a 10 byte array containing channel classification 
                        information. The most significant bit must be 0. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function allow the Bluetooth host to specify a channel
                    classification based on its local information.
                    <p>When called this function will block until it receives a 
                    command complete event or until a preset time has elapsed 
                    (~ 3 secs).

*************************************************************************************/
TESTENGINE_API(int32) hciSetAfhHostChannelClass(uint32 handle, 
                                                uint8 const * const cClass);

/*************************************************************************************
    
    Function :      int32 hciReadAfhChannelMap(uint32 handle, uint16 aclHandle, 
                                                uint8* mode, uint8* cMap)

    Parameters :    handle - 
                        Handle to the BlueCore device.

                    aclHandle - 
                        Valid ACL connection handle.

                    mode - 
                        Pointer to a single byte which the function will set to 
                        define whether AFH is enabled or disabled for the given 
                        connection.

                    cMap - 
                        Pointer to a 10 byte array that will contain channel map 
                        information for the given connection.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to indicate the state of the hop
                    sequence specified by the most recent LMP_Set_AFH message for a 
                    given connection handle.
                    <p>When called this function will block until it receives a
                    command complete event or until a preset time has elapsed
                    (~ 3 secs).

*************************************************************************************/
TESTENGINE_API(int32) hciReadAfhChannelMap(uint32 handle, uint16 aclHandle, uint8* mode, 
                                            uint8* cMap);

/*************************************************************************************
    
    Function :      int32 hciSetEventFilterAutoacceptConnection(uint32 handle)

    Parameters :    handle - 
                        Handle to the device
    

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the event filter to auto accept a connection request. 
                    Implements the command HCI_Set_Event_Filter with the following
                    parameters:
                    <table>
                        <tr><td> 0x02 - Connection  setup filter type
                        <tr><td> 0x00 - Allow connections from all devices
                        <tr><td> 0x02 - Auto accept the connection
                    </table>

*************************************************************************************/
TESTENGINE_API(int32) hciSetEventFilterAutoacceptConnection(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hciWriteInquiryScanActivity(uint32 handle, 
                                                    uint16 inquiryscan_interval, 
                                                    uint16 inquiryscan_window)

    Parameters :    handle - 
                        Handle to the device

                    inquiryscan_interval - 
                        Defines the amount of time between consecutive inquiry scans.

                    inquiryscan_window - 
                        Defines the amount of time for the duration of the inquiry 
                        scan.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command HCI_Write_Inquiryscan_activity.

*************************************************************************************/
TESTENGINE_API(int32) hciWriteInquiryScanActivity(uint32 handle, 
                                                uint16 inquiryscan_interval, 
                                                uint16 inquiryscan_window);

/*************************************************************************************
    
    Function :      int32 hciWritePageScanActivity(uint32 handle, 
                                                    uint16 pagescan_interval, 
                                                    uint16 pagescan_window)

    Parameters :    handle - 
                        Handle to the device

                    pagescan_interval - 
                        Defines the amount of time between consecutive page scans.

                    pagescan_window - 
                        Defines the amount of time for the duration of the page scan.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command HCI_Write_Pagescan_activity.

*************************************************************************************/
TESTENGINE_API(int32) hciWritePageScanActivity(uint32 handle, uint16 pagescan_interval, 
                                                uint16 pagescan_window);

/*************************************************************************************
    
    Function :      int32 hciWriteScanEnable(uint32 handle, uint8 scan_enable)

    Parameters :    handle - 
                        Handle to the device

                    scan_enable - 
                        Controls whether or not the Bluetooth device will periodically 
                        scan for page attempts and/or inquiry requests from other 
                        Bluetooth devices.
                        <table>
                            <tr><td> 0x00 = No Scans enabled.
                            <tr><td> 0x01 = Inquiry Scan enabled. Page Scan disabled.
                            <tr><td> 0x02 = Inquiry Scan disabled. Page Scan enabled.
                            <tr><td> 0x03 = Inquiry Scan enabled. Page Scan enabled.
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command HCI_WriteScan_Enable.

*************************************************************************************/
TESTENGINE_API(int32) hciWriteScanEnable(uint32 handle, uint8 scan_enable);

/*************************************************************************************
    
    Function :      int32 hciInquiry(uint32 handle, uint8 inquiryLength, 
                                    uint8 numResponses, uint32 lap)

    Parameters :    handle - 
                        Handle to the device

                    inquiryLength - 
                        Maximum amount of time before the inquiry is halted, as 
                        defined by the BT spec.

                    numResponses - 
                        Maximum number of responses before inquiry is halted, as 
                        defined in the BT spec. 0x00 is unlimited.

                    lap - 
                        The LAP from which the inquiry access code is derived, as 
                        defined in the BT spec.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to enter inquiry mode. 
                    This function will NOT wait for any inquiry results.

*************************************************************************************/
TESTENGINE_API(int32) hciInquiry(uint32 handle, uint8 inquiryLength, uint8 numResponses, 
                                uint32 lap);

/*************************************************************************************
    
    Function :      int32 hciGetInquiryResults(uint32 handle, uint32 * lap, 
                                                uint8 * uap, uint16 * nap, 
                                                uint16 * clockOffset, uint32 maxLen, 
                                                uint32 * len)

    Parameters :    handle - 
                        Handle to the device

                    lap - 
                        Pointer to the array of LAP's as defined by the returned 
                        value.

                    uap - 
                        Pointer to the array of UAP's as defined by the returned 
                        value.

                    nap - 
                        Pointer to the array of NAP's as defined by the returned 
                        value.

                    clockOffset - 
                        Pointer to the array of clock offsets as defined by the 
                        returned value.

                    maxLen - 
                        Size of the arrays allocated for the LAP, UAP and NAP.

                    len - 
                        Pointer to a variable to hold the actual data returned with 
                        the LAP, UAP and NAP.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Retrieves the bd_addrs and clock offsets of the devices found 
                    during the last completed inquiry. If an inquiry is in progress 
                    then this function will wait for a time based on the inquiryLength 
                    parameter passed to hciInquiry().

*************************************************************************************/
TESTENGINE_API(int32) hciGetInquiryResults(uint32 handle, uint32 * lap, uint8 * uap, 
                                            uint16 * nap, uint16 * clockOffset, 
                                            uint32 maxLen, uint32 * len);

/*************************************************************************************
    
    Function :      int32 hciInquiryCancel(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to exit inquiry mode. The DLL will check if the 
                    device is currently in inquiry before issuing this. Failure on 
                    invalid handle or DM commands enabled.

*************************************************************************************/
TESTENGINE_API(int32) hciInquiryCancel(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hciCreateConnection(uint32 handle, uint32 lap, uint8 uap, 
                                            uint16 nap, uint16 pkt_type, 
                                            uint16 * connectionHandle)

    Parameters :    handle - 
                        Handle to the device

                    lap - 
                        LAP of BD_ADDR. Must be one of the BD_ADDR's returned by 
                        get_inquiry_results.

                    uap - 
                        UAP of BD_ADDR. Must be one of the BD_ADDR's returned by 
                        get_inquiry_results.

                    nap - 
                        NAP of BD_ADDR. Must be one of the BD_ADDR's returned by 
                        get_inquiry_results.

                    pkt_type - 
                        Or-ed combination of packet types as defined in the Bluetooth 
                        specification. A bit being set may indicate that a packet type
                        is allowed or disallowed, as indicated in the following table:
                        <table>
                            <tr><td>0x0002 = 2-DH1 may not be used
                            <tr><td>0x0004 = 3-DH1 may not be used
                            <tr><td>0x0008 = DM1 may be used
                            <tr><td>0x0010 = DH1 may be used
                            <tr><td>0x0100 = 2-DH3 may not be used
                            <tr><td>0x0200 = 3-DH3 may not be used
                            <tr><td>0x0400 = DM3 may be used
                            <tr><td>0x0800 = DH3 may be used
                            <tr><td>0x1000 = 2-DH5 may not be used
                            <tr><td>0x2000 = 3-DH5 may not be used
                            <tr><td>0x4000 = DM5 may be used
                            <tr><td>0x8000 = DH5 may be used
                        </table>
                        <p>Normally this value is set to allow all packet types, which
                        allows BlueCore to use it's own algorithm to select the best
                        rate depending on the link quality.
                        The value which would allow all of the above packets to be 
                        used is: 0xCC18 = (DM1 | DH1 | DM3 | DH3 | DM5 | DH5).

                    connectionHandle - 
                        Connection handle to a Bluetooth connection. 
                        NOTE: The connection handle is local to a device and not a 
                        link. Therefore when you create a connection each member could 
                        have a different connection handle for that link so the value 
                        returned by this function should only be used for this device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to attempt to create a connection with a device 
                    discovered during the inquiry phase. This function will wait for a 
                    connection complete event with a 3 second time out. Success is 
                    determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful connection complete event
                        <li> Invalid BD_ADDR (not part of the inquiry results list)
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciCreateConnection(uint32 handle, uint32 lap, uint8 uap, 
                                        uint16 nap, uint16 pkt_type, 
                                        uint16 * connectionHandle);

/*************************************************************************************
    
    Function :      int32 hciCreateConnectionNoInquiry(uint32 handle, uint32 lap, 
                                                        uint8 uap, uint16 nap, 
                                                        uint16 pkt_type, 
                                                        uint8 page_scan_rep_mode, 
                                                        uint8 page_scan_mode, 
                                                        uint16 clock_offset, 
                                                        uint8 allow_role_switch, 
                                                        uint16 * connectionHandle)

    Parameters :    handle - 
                        Handle to the device

                    lap - 
                        LAP of BD_ADDR.

                    uap - 
                        UAP of BD_ADDR.

                    nap - 
                        NAP of BD_ADDR.

                    pkt_type - 
                        As for hciCreateConnection(...)

                    page_scan_repetition_mode - 
                        Page scan repetition mode supported by the slave device 
                        (0 = R0, 1 = R1, 2 = R2).

                    page_scan_mode - 
                        BT Spec. requires this to be set to 0.

                    clock_offset - 
                        The clock offset between the devices.

                    allow_role_switch - 
                        Specifies if the local device accepts the request of a 
                        master/slave role switch. 0 to not allow, 1 to allow.

                    connectionHandle - 
                        Connection handle to a Bluetooth connection. 
                        NOTE: The connection handle is local to a device and not a 
                        link. Therefore when you create a connection each member could 
                        have a different connection handle for that link so the value 
                        returned by this function should only be used for this device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to attempt to create a connection with a remote 
                    device. This function will wait for a connection complete event 
                    with a 5 second time out. Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful connection complete event
                        <li> Invalid BD_ADDR (not part of the inquiry results list)
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciCreateConnectionNoInquiry(uint32 handle, uint32 lap, uint8 uap, 
                                                    uint16 nap, uint16 pkt_type, 
                                                    uint8 page_scan_rep_mode, 
                                                    uint8 page_scan_mode, 
                                                    uint16 clock_offset, 
                                                    uint8 allow_role_switch, 
                                                    uint16 * connectionHandle);

/*************************************************************************************
    
    Function :      int32 hciCreateScoConnection(uint32 handle, 
                                                    uint16 aclConnectionHandle, 
                                                    uint16 pkt_type, 
                                                    uint16 * scoConnectionHandle)

    Parameters :    handle - 
                        Handle to the device

                    aclConnectionHandle - 
                        ACL connection handle.

                    pkt_type - 
                        Or-ed combination of packet types as defined in the Bluetooth 
                        specification for the "Add_Sco_Connection" HCI command. A bit 
                        being set indicates that a packet type is allowed. The 
                        following table lists the packet types which can be specified:
                        <table>
                            <tr><td>0x0020 = HV1 may be used
                            <tr><td>0x0040 = HV2 may be used
                            <tr><td>0x0080 = HV3 may be used
                        </table>
                        <p>Normally this value is set to allow all packet types, which
                        allows BlueCore to use it's own algorithm to select the best
                        rate depending on the link quality.
                        The value which would allow all of the above packets to be 
                        used is: 0x00E0 = (HV1 | HV2 | HV3).

                    scoConnectionHandle - 
                        Pointer to a variable to hold the returned SCO connection 
                        handle.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command will cause the link manager to create a SCO 
                    connection using the ACL connection specified by the 
                    aclConnectionHandle command parameter. Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful connection complete event
                    </ol>
                    <p>This function is deprecated due to the HCI_Add_Sco_Connection
                    command being deprecated in the BT spec from v1.2. Use 
                    hciSetupScoConnection instead for v1.2 onwards.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) hciCreateScoConnection(uint32 handle, uint16 aclConnectionHandle, 
                                                uint16 pkt_type, 
                                                uint16 * scoConnectionHandle);

/*************************************************************************************
    
    Function :      int32 hciSetupScoConnection(uint32 handle, 
                                                uint16 aclConnectionHandle, 
                                                uint32 txBandwidth,
                                                uint32 rxBandwidth,
                                                uint16 maxLatency,
                                                uint16 voiceSetting,
                                                uint8 retransEffort,
                                                uint16 pktType, 
                                                uint16 * scoConnectionHandle)

    Parameters :    handle - 
                        Handle to the device

                    aclConnectionHandle - 
                        ACL connection handle.

                    txBandwidth - 
                        The transmit bandwidth requirement in octets per second.

                    rxBandwidth - 
                        The receive bandwidth requirement in octets per second.

                    maxLatency - 
                        A value, in milli-seconds, representing the upper limit of the
                        sum of the synchronous interval and the size of the eSCO 
                        window. 

                    voiceSetting - 
                        Also referred to as the content format. Specifies settings for
                        voice connections. Setting this parameter to anything other than
                        the current device(s) voice setting will result in failure. Use
                        hciReadVoiceSetting to get the setting, and hciWriteVoiceSetting 
                        to alter it, if required, before calling this function.

                    retransEffort - 
                        Specifies the retransmission behaviour as follows:
                        <table>
                            <tr><td>0x00 = No retransmissions
                            <tr><td>0x01 = At least 1 retransmission, optimise for 
                                           power consumption
                            <tr><td>0x02 = At least 1 retransmission, optimise for 
                                           link quality
                            <tr><td>0xFF = Don't care
                            <tr><td>0x03 -> 0xFE = Reserved
                        </table>

                    pktType - 
                        Or-ed combination of packet types. A bit being set may 
                        indicate that a packet type is allowed or disallowed, as 
                        indicated in the following table:
                        <table>
                            <tr><td>0x0001 = HV1 may be used
                            <tr><td>0x0002 = HV2 may be used
                            <tr><td>0x0004 = HV3 may be used
                            <tr><td>0x0008 = EV3 may be used
                            <tr><td>0x0010 = EV4 may be used
                            <tr><td>0x0020 = EV5 may be used
                            <tr><td>0x0040 = 2-EV3 may not be used
                            <tr><td>0x0080 = 3-EV3 may not be used
                            <tr><td>0x0100 = 2-EV5 may not be used
                            <tr><td>0x0200 = 3-EV5 may not be used
                        </table>
                        <p>Normally this value is set to allow all packet types, which
                        allows BlueCore to use it's own algorithm to select the best
                        rate depending on the link quality.
                        The value which would allow all of the above packets to be 
                        used is: 0x003F = (HV1 | HV2 | HV3 | EV3 | EV4 | EV5).

                    scoConnectionHandle - 
                        Pointer to a variable to hold the returned SCO connection 
                        handle.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command will cause the link manager to setup a SCO/eSCO
                    connection using the ACL connection specified by the 
                    aclConnectionHandle command parameter. Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful connection complete event
                    </ol>
                    <p>For more information about the parameters and their ranges, 
                    see the BlueTooth specification for the 
                    "Setup_Synchronous_Connection" HCI command.
                    <p>Note that this function does not support the modification 
                    of an existing SCO/eSCO connection.

    Example :

        static const uint16 SETUP_SCO_ALL_PKTS = 0x003F;
        static const int32 MAX_TIMEOUT = 5000;

	    uint32 masterHandle;
	    uint32 slaveHandle;
        int32 timeout = 0;
        
        do
        {
            cout << "Trying to connect to master..." << endl;
            masterHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            timeout+=1000;
        }while(masterHandle == 0 && timeout<MAX_TIMEOUT);

        timeout = 0;

        do
        {
            cout << "Trying to connect to slave..." << endl;
            slaveHandle = openTestEngine(USB, "\\\\.\\csr0", 0, 1000, 500);
            timeout+=1000;
        }while(slaveHandle == 0 && timeout<MAX_TIMEOUT);

        if((masterHandle != 0) && (slaveHandle != 0))
	    {
		    // Enable the slave device to auto accept incoming connections
		    // Implement the command HCI_WriteScan_Enable with the parameter 0x03, 
            // Inquiry scan enabled, Page scan enabled.
		    hciSlave(slaveHandle);

		    // Create an ACL connection to the Slave device from the master
		    uint16 aclHandle;
		    if(hciCreateConnectionNoInquiry(masterHandle, 0xf502, 0x5b, 0x2, 0xcc18, 0, 0, 0, 1, &aclHandle) != TE_OK)
		    {
			    std::cout << "Failed to connect to device" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;
		    }
		    std::cout << "ACL Handle = " << aclHandle << std::endl;

		    // Check connection status
		    if(hciConnectionStatus(masterHandle, aclHandle) != TE_OK)
		    {
			    std::cout << "Failed connection status" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;
		    }

		    // Check link quality
		    uint8 linkQuality;
		    if(hciGetLinkQuality(masterHandle, aclHandle, &linkQuality) != TE_OK)
		    {
			    std::cout << "Failed link quality" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;

		    }
		    std::cout << "link quality = " << (uint16)linkQuality << std::endl;

            // Map SCO over PCM for audio test
            if(bccmdSetMapScoPcm(masterHandle, 1) != TE_OK || bccmdSetMapScoPcm(slaveHandle, 1) != TE_OK)
            {
			    std::cout << "Failed to map SCO over PCM" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;
            }

            // Set the voice setting if required
            const uint16 voiceSetting(0x0060);
            if(hciWriteVoiceSetting(masterHandle, voiceSetting) != TE_OK || hciWriteVoiceSetting(slaveHandle, voiceSetting) != TE_OK)
            {
			    std::cout << "Failed to set voice settings" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;
            }

		    // Setup SCO connection
		    uint16 scoConnectionHandle;
		    if(hciSetupScoConnection(masterHandle, aclHandle, 8000, 8000, 32, voiceSetting, 0, SETUP_SCO_ALL_PKTS, &scoConnectionHandle) != TE_OK)
		    {
			    std::cout << "Failed to establish SCO link" << std::endl;
			    closeTestEngine(masterHandle);
			    closeTestEngine(slaveHandle);
			    return -1;
		    }
            else
		    {
			    // Created SCO connection successfully

			    //...
                // Do SCO testing here
                //...

			    hciDisconnect(masterHandle, scoConnectionHandle);
		    }

		    // Disconnect ACL link
            hciDisconnect(masterHandle, aclHandle);
            
		    // Close handles to master and slave devices
            closeTestEngine(masterHandle);
            closeTestEngine(slaveHandle);
        }
        else
        {
            std::cout << "Failed to get handles to one or both devices" << std::endl;
            return -1;
        }

        return 0;

*************************************************************************************/
TESTENGINE_API(int32) hciSetupScoConnection(uint32 handle, uint16 aclConnectionHandle, 
                                            uint32 txBandwidth, uint32 rxBandwidth,
                                            uint16 maxLatency, uint16 voiceSetting,
                                            uint8 retransEffort, uint16 pktType, 
                                            uint16 * scoConnectionHandle);

/*************************************************************************************
    
    Function :      int32 hciReadVoiceSetting(uint32 handle, uint16* voiceSetting)

    Parameters :    handle - 
                        Handle to the device

                    voiceSetting - 
                        The location where the value specifying the settings for voice 
                        connections will be stored. Also referred to as the content 
                        format. 

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command will read the Voice_Setting configuration parameter. 
                    Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful command complete event
                    </ol>
                    <p>For more information about the parameter, see the BlueTooth 
                    specification for the "HCI_Read_Voice_Setting" HCI command.
                    <p>This function can be used to get the default or current voice
                    setting to be passed as a parameter in the hciSetupScoConnection 
                    function.

*************************************************************************************/
TESTENGINE_API(int32) hciReadVoiceSetting(uint32 handle, uint16* voiceSetting);

/*************************************************************************************
    
    Function :      int32 hciWriteVoiceSetting(uint32 handle, uint16 voiceSetting)

    Parameters :    handle - 
                        Handle to the device

                    voiceSetting - 
                        Also referred to as the content format. Specifies settings for
                        voice connections.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This command will write the Voice_Setting configuration parameter. 
                    Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Successful command complete event
                    </ol>
                    <p>For more information about the parameter, see the BlueTooth 
                    specification for the "HCI_Write_Voice_Setting" HCI command.
                    <p>Note that hciSetupScoConnection will fail if the voice setting
                    parameter is different to that set on the devices to be connected. 
                    Therefore, if a voice setting other than the default is required, 
                    this function must be called to write the voice setting to the 
                    devices before hciSetupScoConnection is called.

*************************************************************************************/
TESTENGINE_API(int32) hciWriteVoiceSetting(uint32 handle, uint16 voiceSetting);

/*************************************************************************************
    
    Function :      int32 hciWriteLinkPolicySettings(uint32 handle, 
                                                        uint16 connection_handle, 
                                                        uint16 link_policy_settings)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device

                    link_policy_settings - 
                        Determines the behavior of the local Link Manager when it 
                        receives a request from a remote device or it determines 
                        itself to change the master-slave role or to enter the hold, 
                        sniff, or park mode.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to attempt to write the link policy settings.
                    Success determined by:
                    <ol>
                        <li> Valid handle
                        <li> Valid connection handle
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciWriteLinkPolicySettings(uint32 handle, 
                                                    uint16 connection_handle, 
                                                    uint16 link_policy_settings);

/*************************************************************************************
    
    Function :      int32 hciSendAclFile(uint32 handle, uint16 connHandle, 
                                            const char* fileName)

    Parameters :    handle - 
                        Handle to the device that will send the contents of the given 
                        file

                    connHandle - 
                        ACL connection handle as returned by hciCreateConnection or 
                        hciCreateConnectionNoInquiry.

                    fileName - 
                        Name of the file whose contents will be transmitted as ACL 
                        packets.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to transmit the contents of a file as ACL 
                    data to another device. Before this function is called a valid 
                    ACL connection must have been established with a call to either 
                    hciCreateConnection or hciCreateConnectionNoInquiry.
                    <p>Before transmitting data a call to hciResetAclState must be
                    made on the receiving device.
                    <p>When a file is sent the receiving device must be polled using
                    the hciGetAclState function for a result of ACL_RECEIVED (3) or
                    ACL_ERROR (4) at which point the entire file should have been
                    received unless there was a problem.
                    <p>The received file will be saved in the applications run
                    directory regardless of where it was transmitted from. The
                    received file name may be different to the original. The 
                    hciGetAclFileName function should be used to find the name
                    that the received data was saved to.
                    <p>This function has been tested with file sizes up to 1 Mbyte,
                    though typically smaller file sizes should be used (< 100 Kbyte)
                    to preserve memory resources required by the software in
                    transmitting the file.

*************************************************************************************/
TESTENGINE_API(int32) hciSendAclFile(uint32 handle, uint16 connHandle, 
                                        const char* fileName);

/*************************************************************************************
    
    Function :      int32 hciSendAclData(uint32 handle, uint16 connHandle, 
                                            const uint8* data, const uint16 length)

    Parameters :    handle - 
                        Handle to the device that will send the contents of the given 
                        file.

                    connHandle - 
                        ACL connection handle as returned by hciCreateConnection or 
                        hciCreateConnectionNoInquiry.

                    data - 
                        Pointer to array of bytes to be transmitted.

                    length - 
                        Length of the array (in bytes) pointed to by data. Maximum 
                        length is 65535.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to transmit an array of bytes as ACL data 
                    to another device. Before this function is called a valid ACL 
                    connection must have been established with a call to either 
                    hciCreateConnection or hciCreateConnectionNoInquiry.
                    <p>Before transmitting data a call to hciResetAclState must be
                    made on the receiving device.
                    <p>When data is sent the receiving device must be polled using
                    the hciGetAclState function for a result of ACL_RECEIVED (3) at
                    which point all data should have been received.
                    <p>The received data can be read using the hciGetAclData
                    function.
               
*************************************************************************************/
TESTENGINE_API(int32) hciSendAclData(uint32 handle, uint16 connHandle, 
                                        const uint8* data, const uint16 length);

/*************************************************************************************

    Function :      int32 hciGetAclData(uint32 handle, uint8* data, uint32* numBytes)

    Parameters :    handle - 
                        Handle to the device that received ACL data.

                    data - 
                        Pointer to a pre-allocated byte array to receive ACL data 
                        packet. If set to 0 no data will be copied.

                    numBytes - 
                        Pointer to a value where the number of bytes in the packet
                        will be stored.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to retrieve the contents of a received 
                    ACL packet.

*************************************************************************************/
TESTENGINE_API(int32) hciGetAclData(uint32 handle, uint8* data, uint32* numBytes);

/*************************************************************************************

    Function :      int32 hciGetAclBytesRead(uint32 handle, uint32* numBytes)

    Parameters :    handle - 
                        Handle to the device receiving ACL data.

                    numBytes - 
                        Pointer to a value where the number of bytes read so far will
                        be stored.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to retrieve the the number of bytes 
                    read so far by the recipient of some ACL data transferred as a 
                    result of a call to hciSendAclData or hciSendAclFile.

*************************************************************************************/
TESTENGINE_API(int32) hciGetAclBytesRead(uint32 handle, uint32* numBytes);

/*************************************************************************************
    
    Function :      int32 hciGetAclFileName(uint32 handle, char * fileName, 
                                            uint32* length)

    Parameters :    handle - 
                        A valid handle to the device.

                    fileName - 
                        A pre-allocated string to receive the name of the file. This 
                        parameter can be set to 0, in which case the function will 
                        just set the length parameter to the number of characters 
                        (NOT including a null terminator) required to hold the file 
                        name.

                    length -
                        Pointer to a value where the file name length (not including
                        null terminator) will be stored.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   The file name of received ACL data is not necessarily the same as 
                    the one transmitted, because of one of the following reasons:
                    <ol>
                        <li> File already exists
                        <li> File was not in the applications run folder
                    </ol>
                    This function is therefore used to find the name of the file
                    containing received ACL data. The name is returned in the 
                    fileName argument which should be a pointer to a preallocated 
                    character string large enough to accept the file name. The 
                    required string size can be found by calling this function with 
                    the fileName argument set to 0.

*************************************************************************************/
TESTENGINE_API(int32) hciGetAclFileName(uint32 handle, char * fileName, 
                                        uint32* length);

/*************************************************************************************
    
    Function :      int32 hciGetAclState(uint32 handle, int32* state)

    Parameters :    handle - 
                        A valid handle to the device.

                    state -
                        Pointer to a value where the state will be stored. This can 
                        be one of the following values:
                        <table>
                            <tr><td> ACL_IDLE<td>0<td>No transfer taking place.
                            <tr><td> ACL_FILENAME<td>1<td>Receiving file name packet.
                            <tr><td> ACL_RECEIVING<td>2<td>Receiving data packets.
                            <tr><td> ACL_RECEIVED<td>3<td>File received.
                            <tr><td> ACL_ERROR<td>4<td>Error occured.
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to check the state of a trasmission in
                    progress. It should be called for the device that is RECEIVING 
                    data.

*************************************************************************************/
TESTENGINE_API(int32) hciGetAclState(uint32 handle, int32* state);

/*************************************************************************************
    
    Function :      int32 hciResetAclState(uint32 handle)

    Parameters :    handle - 
                        A valid handle to the device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function should be called by the RECEIVING device before a 
                    call to hclSendAclData.

*************************************************************************************/
TESTENGINE_API(int32) hciResetAclState(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hciReset(uint32 handle)

    Parameters :    handle - 
                        A valid handle to the device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function issues an HCI_Reset and waits for the appropriate 
                    command complete.

*************************************************************************************/
TESTENGINE_API(int32) hciReset(uint32 handle);

/*************************************************************************************
    
    Function :      int32 bccmdGetAnaXtalFtrim(uint32 handle, uint16 * ftrim)

    Parameters :    handle - 
                        A valid handle to the device.

                    ftrim - 
                        Variable to receive value of ftrim.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to retrieve the BlueCore crystal trim value.
                    It can be used to get the crystal trim value on devices where the
                    firmware does not have radiotest functionality.
                    It can only be used on firmware that supports 
                    BCCMD_ANA_FTRIM_READWRITE.
                   
*************************************************************************************/
TESTENGINE_API(int32) bccmdGetAnaXtalFtrim(uint32 handle, uint16 * ftrim);

/*************************************************************************************
    
    Function :      int32 bccmdSetAnaXtalFtrim(uint32 handle, uint16 ftrim)

    Parameters :    handle - 
                        A valid handle to the device.

                    ftrim - 
                        Value of ftrim to set

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to set the BlueCore crystal trim value.
                    It is equivalent to radiotestCfgXtalFtrim but can be used to set
                    the trim value on devices where the firmware does not have 
                    radiotest functionality.
                    It can only be used on firmware that supports 
                    BCCMD_ANA_FTRIM_READWRITE.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetAnaXtalFtrim(uint32 handle, uint16 ftrim);

/*************************************************************************************
    
    Function :      int32 hciSniffMode(uint32 handle, uint16 connection_handle, 
                                        uint16 sniff_max_interval, 
                                        uint16 sniff_min_interval, 
                                        uint16 sniff_attempt, uint16 sniff_timeout)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

                    sniff_max_interval - 
                        Maximum acceptable number of Baseband slots between each sniff 
                        period.

                    sniff_min_interval - 
                        Minimum acceptable number of Baseband slots between each sniff 
                        period.

                    sniff_attempt - 
                        Number of Baseband receive slots for sniff attempt.

                    sniff_timeout - 
                        Number of Baseband receive slots for sniff timeout.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to attempt to enter Sniff Mode with a remote 
                    device. Success determined by:
                    <ol>
                        <li> Valid handle
                        <li> Valid connection handle
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciSniffMode(uint32 handle, uint16 connection_handle, 
                                    uint16 sniff_max_interval, 
                                    uint16 sniff_min_interval, 
                                    uint16 sniff_attempt, uint16 sniff_timeout);

/*************************************************************************************
    
    Function :      int32 hciExitSniff(uint32 handle, uint16 connection_handle)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to attempt to exit Sniff Mode with a remote 
                    device. Success determined by:
                    <ol>
                        <li> Valid handle
                        <li> Valid connection handle
                        <li> DM commands not in use
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciExitSniff(uint32 handle, uint16 connection_handle);

/*************************************************************************************
    
    Function :      int32 hciDisconnect(uint32 handle, uint16 connection_handle)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to request a disconnect from a remote Bluetooth.
                    The DLL will wait, with a 5 second timeout, for the disconnect 
                    event to be returned. The memory for all disconnected devices will 
                    be freed for this handle. If the remote device is also connected 
                    via TestEngine it is important to run 
                    hciPollDisconnectComplete(..) to confirm the correct operation 
                    and to free any memory on that handle. Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Valid active connection_handle
                        <li> Successful completion of disconnect.
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) hciDisconnect(uint32 handle, uint16 connection_handle);

/*************************************************************************************
    
    Function :      int32 hciGetConnectionHandle(uint32 handle, uint32 lap, uint8 uap, 
                                                uint16 nap, uint16 * connectionHandle)

    Parameters :    handle - 
                        Handle to the device.

                    lap - 
                        LAP of BD_ADDR. 

                    uap - 
                        UAP of BD_ADDR. 

                    nap - 
                        NAP of BD_ADDR. 

                    connectionHandle - 
                        Pointer to a variable which will hold the returned connection 
                        handle.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Returns the connection handle, if existing, between the device 
                    identified by the "handle" parameter and the device identified by 
                    the Bluetooth address. Failure on invalid handle and DM commands 
                    enabled.

*************************************************************************************/
TESTENGINE_API(int32) hciGetConnectionHandle(uint32 handle, uint32 lap, uint8 uap, 
                                            uint16 nap, uint16 * connectionHandle);

/*************************************************************************************
    
    Function :      int32 hciConnectionStatus(uint32 handle, uint16 connection_handle)


    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Checks whether the connection handle points to a valid, active 
                    Bluetooth connection. Failure on invalid handle and DM commands 
                    enabled.

*************************************************************************************/
TESTENGINE_API(int32) hciConnectionStatus(uint32 handle, uint16 connection_handle);

/*************************************************************************************
    
    Function :      int32 hciEnableDeviceUnderTestMode(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to send the enable device under test mode.
                    Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Command complete within timeout
                    </ol>

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            int32 iResetError = 0;  

            do
            {
                cout << "Connected!" << endl;

                int32 iSuccess = psWriteVmDisable(iHandle, 1);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

                iSuccess = hciSlave(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciSlave error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmSlave instead" 
                             << endl;
                    }
                    break;
                }

                iSuccess = hciEnableDeviceUnderTestMode(iHandle);

                if(iSuccess != TE_OK)
                {
                    cout << "hciEnableDeviceUnderTestMode error" << endl;
                    if(iSuccess == TE_UNSUPPORTED_FUNCTION)
                    {
                        cout << "Unsupported command - if using RFCOMM build use dmEnableDeviceUnderTestMode instead"
                             << endl;
                    }
                    break;
                }

                // PERFORM TESTING HERE.....
                cout << "TESTING.........." << endl;

                iSuccess = psWriteVmDisable(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    cout << "psWriteVmDisable error" << endl;
                    break;
                }

                iSuccess = bccmdSetColdReset(iHandle, 0);

                if(iSuccess != TE_OK)
                {
                    iResetError = 1;
                    cout << "bccmdSetColdReset error" << endl;
                    break;
                }

            }while(0);

            if(!iResetError)
            {
                closeTestEngine(iHandle);
            }
        }

*************************************************************************************/
TESTENGINE_API(int32) hciEnableDeviceUnderTestMode(uint32 handle);

/*************************************************************************************
    
    Function :      int32 hciGetLinkQuality(uint32 handle, uint16 connection_handle, 
                                            uint8 * quality)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

                    quality - 
                        Pointer to the link quality parameter returned by this 
                        function.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the link quality

*************************************************************************************/
TESTENGINE_API(int32) hciGetLinkQuality(uint32 handle, uint16 connection_handle, 
                                        uint8 * quality);

/*************************************************************************************
    
    Function :      int32 hciReadRemoteVersionInformation(uint32 handle, 
                                                            uint16 connection_handle, 
                                                            uint32 * r)

    Parameters :    handle - 
                        Handle to the device

                    connection_handle - 
                        Bluetooth connection handle to remote device.

                    r - 
                        A 4 member array of unsigned 32 bit integers. On success r is:
                        <table>
                            <tr><td> r[0] = connection_handle
                            <tr><td> r[1] = lmp_version, as defined by the Bluetooth spec.
                            <tr><td> r[2] = Manufacturer name.
                            <tr><td> r[3] = lmp_subversion
                        </table>
                        With BlueCore the lmp_subversion is the firmware build ID.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the remote version information.

*************************************************************************************/
TESTENGINE_API(int32) hciReadRemoteVersionInformation(uint32 handle, 
                                                        uint16 connection_handle, 
                                                        uint32 * r);

/*************************************************************************************
    
    Function :      int32 hciReadRemoteNameRequest(uint32 handle, uint32 lap, uint8 uap, 
                                                    uint16 nap, 
                                                    uint8 page_scan_repetition_mode, 
                                                    uint8 page_scan_offset, 
                                                    uint16 clock_offset, char * r)

    Parameters :    handle - 
                        Handle to the device

                    lap - 
                        LAP of BD_ADDR. 

                    uap - 
                        UAP of BD_ADDR. 

                    nap - 
                        NAP of BD_ADDR. 
                    
                    page_scan_repetition_mode - 
                        Page scan mode supported by the remote device.

                    page_scan_offset - 
                        Page scan mode supported by the remote device.

                    clock_offset - 
                        The clock offset between the devices.

                    r - 
                        Pointer to the remote name returned by the command. This 
                        needs to be preallocated to at least 249 bytes.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Reads the remote name.

*************************************************************************************/
TESTENGINE_API(int32) hciReadRemoteNameRequest(uint32 handle, uint32 lap, uint8 uap, 
                                                uint16 nap, 
                                                uint8 page_scan_repetition_mode, 
                                                uint8 page_scan_offset, 
                                                uint16 clock_offset, char * r);

/*************************************************************************************
    DM ONLY COMMAND SET
*************************************************************************************/

/*************************************************************************************
    
    Function :      int32 dmRegisterReq(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to register the application manager queue  
                    command. No other DM commands will function unless this is called.
                    Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Command complete within timeout
                        <li> non-HCI firmware
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) dmRegisterReq(uint32 handle);

/*************************************************************************************
    
    Function :      int32 dmSlave(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Performs two steps:
                    <ol>
                        <li>Sets the event filter to auto accept a connection request. 
                            Implements the command DM_HCI_Set_Event_Filter with the 
                            following parameters:
                            <table>
                                <tr><td> 0x02 - Connection setup filter type
                                <tr><td> 0x00 - Allow connections from all devices
                                <tr><td> 0x02 - Auto accept the connection
                            </table>
                    
                        <li>Implements the command DM_HCI_WriteScan_Enable with the 
                            following parameter:
                            <p>0x03 - Inquiry scan enabled, Page scan enabled.
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) dmSlave(uint32 handle);

/*************************************************************************************
    
    Function :      int32 dmEnableDeviceUnderTestMode(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Causes the device to send the enable device under test mode.
                    Success is determined by:
                    <ol>
                        <li> Valid handle
                        <li> Command complete within timeout
                    </ol>

*************************************************************************************/
TESTENGINE_API(int32) dmEnableDeviceUnderTestMode(uint32 handle);

/*************************************************************************************
    
    Function :      int32 dmWritePageScanActivity(uint32 handle, 
                                                    uint16 pagescan_interval, 
                                                    uint16 pagescan_window)

    Parameters :    handle - 
                        Handle to the device

                    pagescan_interval - 
                        Defines the amount of time between consecutive page scans.

                    pagescan_window - 
                        Defines the amount of time for the duration of the page scan.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command DM_HCI_Write_Pagescan_activity.

*************************************************************************************/
TESTENGINE_API(int32) dmWritePageScanActivity(uint32 handle, uint16 pagescan_interval, 
                                                uint16 pagescan_window);

/*************************************************************************************
    
    Function :      int32 dmWriteInquiryScanActivity(uint32 handle, 
                                                        uint16 inquiryscan_interval, 
                                                        uint16 inquiryscan_window)

    Parameters :    handle - 
                        Handle to the device

                    inquiryscan_interval - 
                        Defines the amount of time between consecutive inquiry scans.

                    inquiryscan_window - 
                        Defines the amount of time for the duration of the inquiry 
                        scan.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command DM_HCI_Write_Inquiryscan_activity.

*************************************************************************************/
TESTENGINE_API(int32) dmWriteInquiryScanActivity(uint32 handle, 
                                                    uint16 inquiryscan_interval, 
                                                    uint16 inquiryscan_window);

/*************************************************************************************
    
    Function :      int32 dmWriteScanEnable(uint32 handle, uint8 scan_enable)

    Parameters :    handle - 
                        Handle to the device

                    scan_enable - 
                        Controls whether or not the Bluetooth device will periodically 
                        scan for page attempts and/or inquiry requests from other 
                        Bluetooth devices.
                        <table>
                            <tr><td> 0x00 = No Scans enabled.
                            <tr><td> 0x01 = Inquiry Scan enabled. Page Scan disabled.
                            <tr><td> 0x02 = Inquiry Scan disabled. Page Scan enabled.
                            <tr><td> 0x03 = Inquiry Scan enabled. Page Scan enabled.
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Implements the command DM_HCI_WriteScan_Enable.

*************************************************************************************/
TESTENGINE_API(int32) dmWriteScanEnable(uint32 handle, uint8 scan_enable);

/*************************************************************************************
    
    Function :      int32 dmSetEventFilterAutoacceptConnection(uint32 handle)

    Parameters :    handle - 
                        Handle to the device

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   Sets the event filter to auto accept a connection request. 
                    Implements the command DM_HCI_Set_Event_Filter with the following
                    parameters:
                    <table>
                        <tr><td> 0x02 - Connection setup filter type
                        <tr><td> 0x00 - Allow connections from all devices
                        <tr><td> 0x02 - Auto accept the connection
                    </table>

*************************************************************************************/
TESTENGINE_API(int32) dmSetEventFilterAutoacceptConnection(uint32 handle);

/*************************************************************************************
 
    Function :      int32 bccmdBc3PsuTrim(uint32 handle, uint16 data)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    data - 
                        Data to send with the PDU.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a BCCMD with VARID of BCCMDVARID_BC3PSU 
                    and a function of BCCMDPDU_BC3PSU_PSU_TRIM to the connected 
                    BlueCore device.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) bccmdBc3PsuTrim(uint32 handle, uint16 data);

/*************************************************************************************
 
    Function :      int32 bccmdChargerPsuTrim(uint32 handle, uint16 trim)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    trim - 
                        Charger trim value to be written.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to modify the charger trim register in 
                    order to alter the behaviour of BlueCore's built-in battery 
                    charger.

*************************************************************************************/
TESTENGINE_API(int32) bccmdChargerPsuTrim(uint32 handle, uint16 trim);

/*************************************************************************************
 
    Function :      int32 bccmdBc3BuckReg(uint32 handle, uint16 data)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    data - 
                        Data to send with the PDU.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a BCCMD with VARID of BCCMDVARID_BC3PSU 
                    and a function of BCCMDPDU_BC3PSU_BUCK_REG to the connected 
                    BlueCore device.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) bccmdBc3BuckReg(uint32 handle, uint16 data);

/*************************************************************************************
 
    Function :      int32 bccmdPsuBuckReg(uint32 handle, uint16 reg)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    reg - 
                        Value to enable (1) or disable the switch-mode regulator.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to enable or disable the switch-mode 
                    regulator.

*************************************************************************************/
TESTENGINE_API(int32) bccmdPsuBuckReg(uint32 handle, uint16 reg);

/*************************************************************************************
 
    Function :      int32 bccmdBc3MicEn(uint32 handle, uint16 data)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    data - 
                        Data to send with the PDU.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a BCCMD with VARID of BCCMDVARID_BC3PSU 
                    and a function of BCCMDPDU_BC3PSU_MIC_EN to the connected 
                    BlueCore device.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) bccmdBc3MicEn(uint32 handle, uint16 data);

/*************************************************************************************
 
    Function :      int32 bccmdPsuMicEn(uint32 handle, uint16 reg)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    reg - 
                        Value to enable (1) or disable the linear regulator.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to enable or disable the linear regulator.
                    <p>For BC5 chips, use bccmdSetMicBias instead.

*************************************************************************************/
TESTENGINE_API(int32) bccmdPsuMicEn(uint32 handle, uint16 reg);

/*************************************************************************************
 
    Function :      int32 bccmdBc3Led0(uint32 handle, uint16 data)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    data - 
                        Data to send with the PDU.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a BCCMD with VARID of BCCMDVARID_BC3PSU 
                    and a function of BCCMDPDU_BC3PSU_LED0 to the connected BlueCore 
                    device.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) bccmdBc3Led0(uint32 handle, uint16 data);

/*************************************************************************************
 
    Function :      int32 bccmdLed0(uint32 handle, uint16 state)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    state - 
                        Value to enable (1) or disable (0) LED0

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to set the state of LED0.

*************************************************************************************/
TESTENGINE_API(int32) bccmdLed0(uint32 handle, uint16 state);

/*************************************************************************************
 
    Function :      int32 bccmdBc3Led1(uint32 handle, uint16 data)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    data - 
                        Data to send with the PDU.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write a BCCMD with VARID of BCCMDVARID_BC3PSU 
                    and a function of BCCMDPDU_BC3PSU_LED1 to the connected BlueCore 
                    device.

    Deprecated :

*************************************************************************************/
TESTENGINE_API(int32) bccmdBc3Led1(uint32 handle, uint16 data);

/*************************************************************************************
 
    Function :      int32 bccmdLed1(uint32 handle, uint16 state)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    state - 
                        Value to enable (1) or disable (0) LED1

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to set the state of LED1.

*************************************************************************************/
TESTENGINE_API(int32) bccmdLed1(uint32 handle, uint16 state);

/*************************************************************************************
 
    Function :      int32 bccmdChargerStatus(uint32 handle, uint16 * state)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    state - 
                        Pointer to a 16-bit unsigned integer to hold the returned 
                        state of the charger.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to read the state of built-in battery 
                    charger.

*************************************************************************************/
TESTENGINE_API(int32) bccmdChargerStatus(uint32 handle, uint16 * state);

/*************************************************************************************
 
    Function :      int32 bccmdChargerDisable(uint32 handle, uint16 state)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    state - 
                        Value to disable (1) or enable (0) the built-in battery 
                        charger.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to disable or enable the built-in battery 
                    charger.

*************************************************************************************/
TESTENGINE_API(int32) bccmdChargerDisable(uint32 handle, uint16 state);

/*************************************************************************************
 
    Function :      int32 bccmdChargerSupressLed0(uint32 handle, uint16 state)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    state - 
                        Value to supress (0) or allow (1) LED0 to light when charging.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to enable or disable LED0 on changing.

*************************************************************************************/
TESTENGINE_API(int32) bccmdChargerSupressLed0(uint32 handle, uint16 state);


/*************************************************************************************
 
    Function :      int32 hciCreateConnectionNoWait(uint32 handle, uint32 lap, 
                                                    uint8 uap, uint16 nap, 
                                                    uint16 pktType, 
                                                    uint8 pageScanRepMode, 
                                                    uint8 pageScanMode, 
                                                    uint16 clockOffset, 
                                                    uint8 allowRoleSwitch)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    lap - 
                        The LAP portion of the Bluetooth address of the device to 
                        request a connection with.

                    uap - 
                        The UAP portion of the Bluetooth address of the device to 
                        request a connection with.

                    nap - 
                        The NAP portion of the Bluetooth address of the device to
                        request a connection with.

                    pktType - 
                        As for hciCreateConnection(...)
                   
                    pageScanRepMode - 
                        The page scan repetition mode supported by the remote device 
                        with the given Bluetooth address.
                  
                    pageScanMode - 
                        Indicates the page scan mode that is used for default page 
                        scan.
                  
                    clockOffset - 
                        Difference between the clocks of the local device and the 
                        remote device with the given Bluetooth address.
                                          
                    allowRoleSwitch - 
                        Specifies whether the local device accepts or rejects the 
                        request of master-slave role switch.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to request a connection with a given 
                    device.<br>
                    It is similar to the hciCreateConnectionNoInquiry function, the 
                    difference being that this function does not wait for a 
                    'Command Complete' or a 'Connection Complete' event.<br>
                    This is useful for creating connections involving encryption and 
                    link-key requests where other operations need to be performed 
                    before these events are generated.

*************************************************************************************/
TESTENGINE_API(int32) hciCreateConnectionNoWait(uint32 handle, uint32 lap, uint8 uap, 
                                                uint16 nap, uint16 pktType, 
                                                uint8 pageScanRepMode, 
                                                uint8 pageScanMode, 
                                                uint16 clockOffset, 
                                                uint8 allowRoleSwitch);

/*************************************************************************************
 
    Function :      int32 hciWriteAuthenticationEnable(uint32 handle, uint8 enable)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    enable - 
                        Set to 1 when authentication required for connections, 
                        otherwise 0.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will write the value for the Authentication_Enable 
                    configuration parameter.

*************************************************************************************/
TESTENGINE_API(int32) hciWriteAuthenticationEnable(uint32 handle, uint8 enable);

/*************************************************************************************
 
    Function :      int32 hciLinkKeyRequestNegativeReply(uint32 handle, uint32 lap, 
                                                        uint8 uap, uint16 nap)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    lap - 
                        The LAP portion of the Bluetooth address of the device that 
                        there is no stored link key for.

                    uap - 
                        The UAP portion of the Bluetooth address of the device that 
                        there is no stored link key for.

                    nap - 
                        The NAP portion of the Bluetooth address of the device that 
                        there is no stored link key for.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to reply to a Link Key Request from the
                    controller if the local device does not have a stored link key 
                    for the device with the given Bluetooth address.

*************************************************************************************/
TESTENGINE_API(int32) hciLinkKeyRequestNegativeReply(uint32 handle, uint32 lap, 
                                                        uint8 uap, uint16 nap);

/*************************************************************************************
 
    Function :      int32 hciWaitForConnectionComplete(uint32 handle, 
                                                        uint16* connHandle)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    connHandle - 
                        Pointer to connection handle of the device if successful. Set 
                        to 0xffff on failure.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to wait for a 'Connection Complete' 
                    event.<br>
                    If the event is not triggered within 2 seconds of invocation it 
                    will return failure and the value of the connHandle parameter 
                    will be set to 0xffff.

*************************************************************************************/
TESTENGINE_API(int32) hciWaitForConnectionComplete(uint32 handle, uint16* connHandle);

/*************************************************************************************
 
    Function :      int32 hciWaitForLinkKeyRequest(uint32 handle, uint32* lap, 
                                                    uint8* uap, uint16* nap)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    lap - 
                        The LAP portion of the Bluetooth address of the device for 
                        which a stored link key is being requested.

                    uap - 
                        The UAP portion of the Bluetooth address of the device for 
                        which a stored link key is being requested.

                    nap - 
                        The NAP portion of the Bluetooth address of the device for 
                        which a stored link key is being requested.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to wait for a 'Link Key Request' event. 
                    A link key request event will be generated when setting up a 
                    connection when authentication has been enabled (see
                    hciWriteAuthenticationEnable) prior to calling 
                    hciCreateConnectionNoWait.<br>
                    If the event is not triggered within 2 seconds of invocation it 
                    will return failure.

*************************************************************************************/
TESTENGINE_API(int32) hciWaitForLinkKeyRequest(uint32 handle, uint32* lap, uint8* uap, 
                                                uint16* nap);

/*************************************************************************************
 
    Function :      int32 hciWaitForPinCodeRequest(uint32 handle, uint32* lap, 
                                                    uint8* uap, uint16* nap)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    lap - 
                        The LAP portion of the Bluetooth address of the device which 
                        a new link key is being created for.

                    uap - 
                        The UAP portion of the Bluetooth address of the device which 
                        a new link key is being created for.

                    nap - 
                        The NAP portion of the Bluetooth address of the device which 
                        a new link key is being created for.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to wait for a 'PIN Code Request' event. 
                    A PIN code request event will be generated when setting up a 
                    connection when authentication has been enabled (see
                    hciWriteAuthenticationEnable) prior to calling
                    hciCreateConnectionNoWait.<br>
                    The recipient must respond by calling hciPinCodeRequestReply.<br>
                    If the event is not triggered within 2 seconds of invocation it 
                    will return failure.

*************************************************************************************/
TESTENGINE_API(int32) hciWaitForPinCodeRequest(uint32 handle, uint32* lap, uint8* uap, 
                                                uint16* nap);

/*************************************************************************************
 
    Function :      int32 hciWaitForEncryptionChange(uint32 handle, uint8* enable)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    enable - 
                        Defines whether encryption is enabled/disabled.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to wait for an 'Encryption Change' 
                    event. An encryption change event will be generated following a
                    call to hciSetConnectionEncryption. The event will be generated 
                    for both devices on the connection<br>
                    If the event is not triggered within 2 seconds of invocation it 
                    will return failure.

*************************************************************************************/
TESTENGINE_API(int32) hciWaitForEncryptionChange(uint32 handle, uint8* enable);

/*************************************************************************************
 
    Function :      int32 hciPinCodeRequestReply(uint32 handle, uint32 lap, uint8 uap, 
                                                    uint16 nap, uint8 pinCodeLength, 
                                                    uint8* pinCode)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    lap - 
                        The LAP portion of the Bluetooth address of the device for 
                        which the PIN code is for.

                    uap - 
                        The UAP portion of the Bluetooth address of the device for 
                        which the PIN code is for.

                    nap - 
                        The NAP portion of the Bluetooth address of the device for 
                        which the PIN code is for.

                    pinCodeLength - 
                        The length (in bytes) of the PIN code to be used. Range 1-16.

                    pinCode - 
                        PIN code for the device that is to be connected. This should 
                        be an array of bytes equal in length to the given 
                        pinCodeLength parameter.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to respond to a 'PIN Code Request' event 
                    (see hciWaitForPinCodeRequest) and specifies the PIN code to use 
                    for a connection.

*************************************************************************************/
TESTENGINE_API(int32) hciPinCodeRequestReply(uint32 handle, uint32 lap, uint8 uap, 
                                                uint16 nap, uint8 pinCodeLength, 
                                                uint8* pinCode);

/*************************************************************************************
 
    Function :      int32 hciSetConnectionEncryption(uint32 handle, uint16 connHandle, 
                                                        uint8 enable)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    connHandle - 
                        Connection handle to enable/disable link layer encryption.

                    enable - 
                        Set to 1 to enable link layer encryption, otherwise set to 0.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to enable or disable link level 
                    encryption.

*************************************************************************************/
TESTENGINE_API(int32) hciSetConnectionEncryption(uint32 handle, uint16 connHandle, 
                                                    uint8 enable);

/*************************************************************************************
 
    Function :      int32 vmWrite(uint32 handle, const uint16 * data)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    data - 
                        A pointer to an array of 16-bit unsigned integers. The first 
                        element must be the length of the array.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to write to a VM application. It uses BCSP 
                    channel 13, which is tunnelled, using manufacturer specific 
                    extensions, through the other supported host transports. 
                    Therefore, this function can be used over BCSP, H4, H4DS, H5 and 
                    USB. A successful status does NOT indicate that the packet has 
                    reached BlueCore. The user should implement the appropriate hand 
                    shaking both in the on-chip application and the application 
                    sitting above TestEngine.

*************************************************************************************/
TESTENGINE_API(int32) vmWrite(uint32 handle, const uint16 * data);

/*************************************************************************************
 
    Function :      int32 vmRead(uint32 handle, uint16 * data, uint16 timeout)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    data - 
                        A pointer to an array of 16-bit unsigned integers. The first 
                        element must be the length of the array.

                    timeout - 
                        A timeout, in ms, of the time to wait for the packet. A value 
                        of 0 means the function will just poll.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to read any data returned from a VM 
                    application using BCSP channel 13. BCSP channel 13 is tunnelled, 
                    using manufacturer specific extensions, through the other 
                    supported host transports. Therefore, this function can be used 
                    over BCSP, H4, H4DS, H5 and USB. The function will fetch VM 
                    printf() messages as well as other VM host messages.

*************************************************************************************/
TESTENGINE_API(int32) vmRead(uint32 handle, uint16 * data, uint16 timeout);

/*************************************************************************************
 
    Function :      int32 bccmdSetSingleChan(uint32 handle, uint16 channel)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    channel - 
                        The single hop channel the radio should use.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to configure the BlueCore radio to transmit 
                    and receive on a single hop channel. This disables hopping and 
                    whitening.
                    <p>To re-enable hopping use the bccmdSetHoppingOn function.
                    <p>For more information see the BlueCore BCCMD commands document.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetSingleChan(uint32 handle, uint16 channel);

/*************************************************************************************
 
    Function :      int32 bccmdSetHoppingOn(uint32 handle)

    Parameters :    handle - 
                        A valid BlueCore device handle.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to enable hopping after previously being 
                    disabled as a result of calling bccmdSetSingleChan.
                    <p>For more information see the BlueCore BCCMD commands document.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetHoppingOn(uint32 handle);

/*************************************************************************************
 
    Function :      int32 bccmdFmSwitchPower(uint32 handle, uint8 powerOn)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    powerOn - 
                        Power state to set. Set to 1 to power on, 0 to power off.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to switch on power to the FM radio part of
                    the chip.
                    <p>If powerOn is set to 1, RDS reception is switched on after 
                    the radio is powered up.
                    <p>NOTE: radiotest and FM control functions should be run either
                    side of a warm reset to ensure that the firmware is in the correct
                    state in each case.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            do
            {
                int32 iSuccess = bccmdFmSwitchPower(iHandle, 1);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmSwitchPower error" << endl;
                    break;
                }

                uint8 status;
                iSuccess = bccmdFmGetStatus(iHandle, &status);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmGetStatus error" << endl;
                    break;
                }
                if(((status & 0x07) >> 3) != 1)
                {
                    cout << "bccmdFmGetStatus indicates incorrect tuner state" << endl;
                    break;
                }

                iSuccess = bccmdFmSetFreq(iHandle, 103000);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmSetFreq error" << endl;
                    break;
                }

                int8 rssi;
                iSuccess = bccmdFmGetRssi(iHandle, &rssi);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmGetRssi error" << endl;
                    break;
                }
                cout << "RSSI = " << (int16)rssi << endl;

                int16 snr;
                iSuccess = bccmdFmGetSnr(iHandle, &snr);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmGetSnr error" << endl;
                    break;
                }
                cout << "SNR = " << snr << endl;

                int16 offset;
                iSuccess = bccmdFmGetIfOffset(iHandle, &offset);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmGetIfOffset error" << endl;
                    break;
                }
                cout << "IF offset = " << offset << endl;

                iSuccess = bccmdFmSetupAudio(iHandle, 0, 16, 2);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmSetupAudio error" << endl;
                    break;
                }

                // TEST AUDIO HERE

                // If using BC7FM or later, should disconnect audio
                iSuccess = bccmdDisconnectAudio(iHandle);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdDisconnectAudio error" << endl;
                    break;
                }

                // May need a delay to ensure the RDS PI has been received
                Sleep(3000);
                uint8 matched;
                iSuccess = bccmdFmVerifyRDSPi(iHandle, 0xCF86, 3000, &matched);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmVerifyRDSPi error" << endl;
                    break;
                }
                if(matched != 0x01)
                {
                    cout << "RDS PI did not match within the timeout period" << endl;
                }
            }
            while(0);

            closeTestEngine(iHandle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmSwitchPower(uint32 handle, uint8 powerOn);

/*************************************************************************************
 
    Function :      int32 bccmdFmSetFreq(uint32 handle, uint32 freqKHz)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    freqKHz - 
                        Frequency to set in KHz.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to tune the FM radio part of the chip to 
                    the desired frequency.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Valid frequency (freqKHz in range)
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmSetFreq(uint32 handle, uint32 freqKHz);

/*************************************************************************************
 
    Function :      int32 bccmdFmGetRssi(uint32 handle, int8* rssi)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    rssi - 
                        The measured RSSI value will be stored at this address

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function gets an RSSI (Recieved Signal Strength Indiction)
                    measurement from the radio, measured in dBm.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Radio powered up and tuned to a valid frequency
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmGetRssi(uint32 handle, int8* rssi);

/*************************************************************************************
 
    Function :      int32 bccmdFmGetSnr(uint32 handle, int16* snr)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    snr - 
                        The measured SNR value will be stored at this address

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>
                    
    Description :   This function gets a SNR (Signal to Noise Ratio) measurement 
                    from the radio, measured in dB.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Radio powered up and tuned to a valid frequency                             
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmGetSnr(uint32 handle, int16* snr);

/*************************************************************************************
 
    Function :      int32 bccmdFmGetIfOffset(uint32 handle, int16* offset)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    offset - 
                        The measured IF offset value will be stored at this address

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>
                    
    Description :   This function gets an IF (Intermediate Frequency) offset
                    measurement from the radio, measured in KHz.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Radio powered up and tuned to a valid frequency                             
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmGetIfOffset(uint32 handle, int16* offset);

/*************************************************************************************
 
    Function :      int32 bccmdFmGetStatus(uint32 handle, uint8* status)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    status - 
                        The fetched status value will be stored at this address. 
                        <p>The status is a bit field with the following format:
                        <ol>
                            <li>bit[0] = TS[0] (tuner state bit 0 *)
                            <li>bit[1] = TS[1] (tuner state bit 1 *)
                            <li>bit[2] = TS[2] (tuner state bit 2 *)
                            <li>bit[3] = RDA   (RDS Data Available indicator)
                            <li>bit[4] = STR   (Stereo signal detected indicator)
                            <li>bit[5] = IFR   (IF frequency in range indicator)
                            <li>bit[6] = RSR   (RSSI in range indicator)
                            <li>bit[7] = Not used
                        </ol>
                        <p>Values for TS indicate:
                        <ol>
                            <li>0 = Radio is not powered up, or has been reset
                            <li>1 = Radio is not tuned to a valid frequency
                            <li>2 = A tuning process is in progress
                            <li>3 = Radio is tuned but RDS Rx. is powered down
                            <li>4 = Radio is tuned but no RDS is present
                            <li>5 = Radio is tuned but RDS is not synchronised
                            <li>6 = Radio is tuned and RDS is being received
                        </ol>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function reads the status register of the FM radio.

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmGetStatus(uint32 handle, uint8* status);

/*************************************************************************************
 
    Function :      int32 bccmdFmSetupAudio(uint32 handle, uint8 route, uint8 gain, 
                                            uint8 channel)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    route - 
                        Audio output route for FM RX, where:
                        <table>
                            <tr><td>0 = Route audio to CODEC
                            <tr><td>1 = Route audio to PCM port 1 (PCM Mode)
                            <tr><td>2 = Route audio to PCM port 2 (PCM Mode)
                            <tr><td>3 = Route audio to PCM port 1 (I2S Master Mode)
                            <tr><td>4 = Route audio to PCM port 2 (I2S Master Mode)
                        </table>
                        <p>Note: Default voice slots are used in each case

                    gain - 
                        Audio gain value to set, between 0 and maxValue, where 
                        maxValue depends on the "route" parameter as follows:
                        <table>
                            <tr><td>route = 0, maxValue = 22
                            <tr><td>route = 1-4, maxValue = 7 (BC5FM only)
                        </table>
                        <p>The gain parameter for PCM audio is ignored for chips 
                        other than BC5FM.

                    channel - 
                        Channel selection for CODEC routing, where:
                        <table>
                            <tr><td>0 = Left only
                            <tr><td>1 = Right only
                            <tr><td>2 = Left and Right channels
                        </table>
                        <p>If a single channel is selected, the other channel will be 
                        muted.
                        <p>Channel selection is for CODEC output only (PCM outputs 
                        always use both channels).

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to setup the audio routing and gain for
                    the FM RX output.

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmSetupAudio(uint32 handle, uint8 route, uint8 gain, 
                                        uint8 channel);

/*************************************************************************************
 
    Function :      int32 bccmdFmVerifyRDSPi(uint32 handle, uint16 pi, uint16 timeoutMs,
                                            uint8* matched)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    pi - 
                        PI (Program Identity) value to match against.

                    timeoutMs - 
                        Timeout value in milliseconds. The function will poll for a 
                        matching PI value for the duration of the timeout before 
                        returning.

                    matched - 
                        The status of the match check will be stored at this address,
                        where 0 = not matched, and 1 = matched.

  Returns :         <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to verify that an expected PI value has 
                    been received and decoded by the FM radio.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Radio powered up and tuned to a valid frequency
                            <li> RDS data being received
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmVerifyRDSPi(uint32 handle, uint16 pi, uint16 timeoutMs,
                                        uint8* matched);

/*************************************************************************************
 
    Function :      int32 bccmdFmTxSwitchPower(uint32 handle, uint8 powerOn)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    powerOn - 
                        Power state to set. Set to 1 to power on, 0 to power off.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to switch on power to the FM TX part of
                    the chip.
                    <p>If powerOn is set to 1, FM TX with RDS is switched on.                    
                    <p>NOTE: FM TX and RX testing cannot run in parallel.

    Example :

        uint32 iHandle = 0;
        int32 iTimeout = 0;

        do
        {
            cout << "Trying to connect..." << endl;
            iHandle = openTestEngine(BCSP, "COM1", 115200, 1000, 0);
            iTimeout += 1000;
        }while(iHandle == 0 && iTimeout < MAX_TIMEOUT);

        if(iHandle != 0)
        {
            do
            {
                int32 iSuccess = bccmdFmTxSwitchPower(iHandle, 1);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmTxSwitchPower error" << endl;
                    break;
                }

                iSuccess = bccmdFmTxSetFreq(iHandle, 103000);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmTxSetFreq error" << endl;
                    break;
                }

                iSuccess = bccmdFmTxSetPowerLevel(iHandle, -1);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmTxSetPowerLevel error" << endl;
                    break;
                }

                iSuccess = bccmdFmTxSetupAudio(iHandle, 0, 10);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdFmTxSetupAudio error" << endl;
                    break;
                }

                // TEST AUDIO HERE

                iSuccess = bccmdDisconnectAudio(iHandle);
                if(iSuccess != TE_OK)
                {
                    cout << "bccmdDisconnectAudio error" << endl;
                    break;
                }
            }
            while(0);

            closeTestEngine(iHandle);
        }

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmTxSwitchPower(uint32 handle, uint8 powerOn);

/*************************************************************************************
 
    Function :      int32 bccmdFmTxSetFreq(uint32 handle, uint32 freqKHz)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    freqKHz - 
                        Frequency to set in KHz.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to tune the FM TX part of the chip to 
                    the desired frequency.
                    <p>Preconditions for success are:
                        <ol>
                            <li> Valid handle
                            <li> Valid frequency (freqKHz in range)
                        </ol>

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmTxSetFreq(uint32 handle, uint32 freqKHz);

/*************************************************************************************
 
    Function :      int32 bccmdFmTxSetPowerLevel(uint32 handle, int16 powerLevel)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    powerLevel - 
                        FM power output value to set, between -24 and 0, where 0 
                        gives the highest output power. 0 is the default value.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to set the FM power level for FM TX.
                    
*************************************************************************************/
TESTENGINE_API(int32) bccmdFmTxSetPowerLevel(uint32 handle, int16 powerLevel);

/*************************************************************************************
 
    Function :      int32 bccmdFmTxSetupAudio(uint32 handle, uint8 route, 
                                                uint16 audioGain)

    Parameters :    handle - 
                        A valid BlueCore device handle.

                    route - 
                        Audio output route for FM TX, where:
                        <table>
                            <tr><td>0 = Route audio from CODEC
                            <tr><td>1 = Route audio from PCM port 1 (PCM Mode)
                            <tr><td>2 = Route audio from PCM port 2 (PCM Mode)
                            <tr><td>3 = Route audio from PCM port 1 (I2S Master Mode)
                            <tr><td>4 = Route audio from PCM port 2 (I2S Master Mode)
                        </table>
                        <p>Note: Default voice slots are used in each case

                    audioGain - 
                        Analogue audio input gain value to set, between 0 and 22.
                        <p>The following key-points are given for approximately 80% 
                        full-scale on the audio input:
                        <table>
                            <tr><td>100mV input = 12
                            <tr><td>200mV input = 10
                            <tr><td>300mV input = 9
                        </table>

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to setup the audio routing and input level 
                    for FM TX.

*************************************************************************************/
TESTENGINE_API(int32) bccmdFmTxSetupAudio(uint32 handle, uint8 route, 
                                          uint16 audioGain);

/*************************************************************************************
 
    Function :      int32 bccmdDisconnectAudio(uint32 handle)

    Parameters :    handle - 
                        A valid BlueCore device handle.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to disconnect any connected audio streams, 
                    such as may be created when calling bccmdFmSetupAudio with BC7 
                    and later chips.
                    <p>Connections are disconnected, and the stream endpoints are 
                    closed.

*************************************************************************************/
TESTENGINE_API(int32) bccmdDisconnectAudio(uint32 handle);

/*************************************************************************************
 
    Function :      int32 bccmdDirectChargerPsuTrim(uint32 handle, uint16 trim)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    trim - 
                        Charger trim value to be written.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to modify the charger trim register in 
                    order to alter the behaviour of BlueCore's built-in battery 
                    charger. The function is for use initially with BC4 and BC5 
                    devices, in order to bypass routines in the firmware which cause 
                    a delay in setting the register value.

*************************************************************************************/
TESTENGINE_API(int32) bccmdDirectChargerPsuTrim(uint32 handle, uint16 trim);

/*************************************************************************************
 
    Function :      int32 teSupportsHq(uint32 handle, uint8* hqSupported)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    hqSupported -
                        If HQCMDs are supported by the transport used for the 
                        specified handle, this value is set to 1. Otherwise the value
                        is set to 0.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                    </table>

    Description :   This function gets the status of HQCMD support. Host transports
                    such as BCSP/H4/H4DS/H5 (UART) and USB support HQCMDs. SPI 
                    transports may support HQCMD if the BlueCore firmware supports HQ 
                    over SPI.
                    <p>Certain radiotests and other BCCMDs generate HQCMDs. This 
                    function can be used to determine whether using such commands
                    is appropriate for the transport used. 
                    <p>All functions with names beginning hq... require HQCMD support.
                    The following radiotest / bccmd functions also require HQ support:
                    <table>
                        <tr><td>radiotestRxstart2
                        <tr><td>radiotestBer1
                        <tr><td>radiotestBer2
                        <tr><td>radiotestBerLoopback
                        <tr><td>radiotestRadioStatus
                        <tr><td>radiotestRadioStatusArray
                        <tr><td>radiotestRxdata1
                        <tr><td>radiotestRxdata2
                        <tr><td>radiotestRxLoopback (deprecated)
                        <tr><td>radiotestSettle (deprecated)
                        <tr><td>bccmdProvokeFault
                    </table>
                    <p>See the BCCMD specification document for further information.

*************************************************************************************/
TESTENGINE_API(int32) teSupportsHq(uint32 handle, uint8* hqSupported);

/*************************************************************************************
 
    Function :      int32 bccmdSetAuxDac(uint32 handle, uint8 enable, uint8 level)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    enable - 
                        Flag defining whether the aux dac output should be enabled or
                        disabled. Set the value to 1 to enable, 0 to disable.

                    level - 
                        Value to write to the AUX_DAC output (if enable = 1). 
                        Range is 0...255.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to control the AUX_DAC line on BlueCore.
                    The line can be enabled with a given level written, or be 
                    disabled.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetAuxDac(uint32 handle, uint8 enable, uint8 level);

/*************************************************************************************
 
    Function :      int32 bccmdSetMicBias(uint32 handle, uint8 enable, uint8 voltage,
                                            uint8 current, uint8 enableLowPowerMode)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    enable - 
                        Flag defining whether the mic bias output should be enabled 
                        or disabled. Set the value to 1 to enable, 0 to disable.

                    voltage - 
                        Value which maps to a voltage set for the line, where for 
                        BC5-MM, the mapping is:
                        <table>
	                        <tr><td>0 = 1.72 v
	                        <tr><td>1 = 1.77 v
	                        <tr><td>2 = 1.83 v
	                        <tr><td>3 = 1.89 v
	                        <tr><td>4 = 1.97 v
	                        <tr><td>5 = 2.03 v
	                        <tr><td>6 = 2.12 v
	                        <tr><td>7 = 2.20 v
	                        <tr><td>8 = 2.34 v
	                        <tr><td>9 = 2.44 v
	                        <tr><td>10 = 2.58 v
	                        <tr><td>11 = 2.71 v
	                        <tr><td>12 = 2.92 v
	                        <tr><td>13 = 3.10 v
	                        <tr><td>14 = 3.34 v
	                        <tr><td>15 = 3.60 v
                        </table>

                    current - 
                        Value which maps to a current set for the line, where for 
                        BC5-MM, the mapping is:
                        <table>
	                        <tr><td>0 = 0.32 mA 
	                        <tr><td>1 = 0.40 mA
	                        <tr><td>2 = 0.48 mA
	                        <tr><td>3 = 0.56 mA
	                        <tr><td>4 = 0.64 mA
	                        <tr><td>5 = 0.72 mA
	                        <tr><td>6 = 0.80 mA
	                        <tr><td>7 = 0.88 mA
	                        <tr><td>8 = 0.97 mA
	                        <tr><td>9 = 1.05 mA
	                        <tr><td>10 = 1.13 mA
	                        <tr><td>11 = 1.21 mA
	                        <tr><td>12 = 1.29 mA
	                        <tr><td>13 = 1.37 mA
	                        <tr><td>14 = 1.45 mA
	                        <tr><td>15 = 1.53 mA
                        </table>

                    enableLowPowerMode - 
                        Flag defining whether the mic bias circuit low power mode 
                        is enabled or disabled. Set the value to 1 to enable, 0 to 
                        disable.
                        <p>NOTE: Whilst saving power, the low power mode does result
                        in greater noise on the line. For most production test 
                        purposes, this should be irrelevant.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to control the MIC_BIAS line on BlueCore.
                    The line can be enabled with specified voltage and current levels, 
                    or be disabled. The built in low power mode for mic_bias can also
                    be controlled.
                    <p>This function and the associated BCCMD in the firmware has
                    been implemented initially for BC5-MM.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetMicBias(uint32 handle, uint8 enable, uint8 voltage,
                                        uint8 current, uint8 enableLowPowerMode);


/*************************************************************************************
 
    Function :      int32 teGetAvailableSpiPorts(uint16* maxLen, char* ports, 
                                                    char* trans, uint16* count)

    Parameters :    maxLen - 
                        Size of the arrays pointed to by the ports and trans 
                        parameters. If this parameter indicates that the ports or 
                        trans arrays are too small to store the complete strings, 
                        then the value is set to the size required (and error is 
                        returned).
                        If any other error occurs, this value is set to zero.

                    ports -
                        Pointer to an array of ASCII chars where the comma separated
                        list of available SPI port names will be stored. These are
                        readable names which could be used for user selection.

                    trans -
                        Pointer to an array of ASCII chars where the comma separated
                        list of SPI transport options for each of the available ports
                        will be stored. The transport options for a port can be 
                        passed directly into the openTestEngineSpiTrans function to 
                        open the port.

                    count -
                        This value is set to the number of available ports found.

    Returns :       <table>
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                    </table>

    Description :   This function is used to get a list of available SPI ports. A 
                    char array, pointed to by the ports parameter, is filled with 
                    a comma separated list of port names. A further char array, 
                    pointed to by the trans parameter, is filled with a comma 
                    separated list of the relevant transport option strings that
                    specify each available SPI port.
                    <p>If the maxLen parameter indicates that either char array is not
                    large enough to contain the strings, Error is returned and the
                    maxLen parameter is set to the size required for the arrays.
                    <p>If any other error occurs, the maxLen parameter is set to 0.
                    <p>This function can be used by an application to get a list
                    of available SPI ports with which to populate a drop down list
                    or other means of selection. The strings returned in the ports
                    parameter are in human readable format for display / selection.
                    The strings returned in the trans parameter can be passed directly
                    to the openTestEngineSpiTrans function to open the port.
                    <p>NOTE: When using SPI over LPT, the driver only supports
                    "traditional" parallel ports, i.e. LPT1 connected to the 
                    Super I/O chip on the PC motherboard. Some users have reported 
                    success with some PCI LPT cards, but this is not supported by 
                    CSR. This function will return all available LPT ports regardless 
                    of whether or not they are supported by the SPI over LPT driver. 
                    The USB SPI device can be used where an LPT port is not available, 
                    or if more than one SPI port is required.

    Example :

        uint32 iHandle(0);
        uint16 maxLen(256);
        uint16 count(0);
        char* portsStr = new char[maxLen];
        char* transStr = new char[maxLen];
        vector<string> ports; // The human readable port strings (e.g. "LPT1")
        vector<string> trans; // The transport option strings (e.g. "SPITRANS=LPT SPIPORT=1")

        int32 status = teGetAvailableSpiPorts(&maxLen, portsStr, transStr, &count);
        if( status != TE_OK && maxLen != 0 )
        {
            // Not enough space - resize the storage
            portsStr = new char[maxLen];
            transStr = new char[maxLen];
            status = teGetAvailableSpiPorts(&maxLen, portsStr, transStr, &count);
        }
        if( status != TE_OK || count == 0 )
        {
            cout << "Error getting SPI ports, or none found" << endl;
            delete[] portsStr;
            delete[] transStr;
            return;
        }

        // Split up the comma separated strings of ports / transport options
        split(ports, portsStr, ','); // Use these for user selection (e.g. drop down list)
        split(trans, transStr, ','); // Use these to open a transport

        // Open the SPI port using the trans string
        // For the purposes of this example, we're just using the first in the list
        iHandle = openTestEngineSpiTrans(trans.at(0).c_str(), 0);

        if(iHandle != 0)
        {
            cout << "Connected!" << endl;

            // Perform all your testing here

            closeTestEngine(iHandle);
        }

        delete[] portsStr;
        delete[] transStr;

        return;

*************************************************************************************/
TESTENGINE_API(int32) teGetAvailableSpiPorts(uint16* maxLen, char* ports, char* trans, 
                                                uint16* count);

/*************************************************************************************
 
    Function :      int32 bccmdProvokeFault(uint32 handle, uint16 faultCode)

    Parameters :    handle - 
                        A valid BlueCore device handle.
  
                    faultCode - 
                        Fault code to provoke.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function provokes a fault within the chip. Once the command 
                    is issued BlueCore will need to be polled for the fault report 
                    using the hqGetFaultReport(..) function.
                    <p>This function will clear any existing logged fault reports
                    before sending the BCCMD to provode a fault. Therefore, it may be
                    appropriate to fetch any existing fault reports first (using 
                    hqGetFaultReports).
                    <p>Fault reports can be disabled by PSKEY, in which case
                    hqGetFaultReport() will timeout and fail, despite the fact that
                    bccmdProvokeFault succeeded.
                    <p>This function is only supported over a SPI connection if HQ
                    over SPI is supported by the device firmware.

*************************************************************************************/
TESTENGINE_API(int32) bccmdProvokeFault(uint32 handle, uint16 faultCode);

/*************************************************************************************
    
    Function :      int32 hqGetFaultReports(uint32 handle, uint16 maxReports,
                                            uint16 * codes, uint32 * timestamps,
                                            uint16 * repeats, uint16 * numReports,
                                            int32 timeout)

    Parameters :    handle - 
                        Handle to the device

                    maxReports - 
                        Maximum number of fault reports to fetch. The size of the
                        arrays pointed to by the codes, timeStamps and repeats
                        parameters must be >= maxReports.

                    codes - 
                        Pointer to an array where the fault codes will be stored. The
                        size of the array should be >= maxReports. 

                    timestamps - 
                        Pointer to an array where the fault timestamps (in 
                        milliseconds) will be stored. The size of the array should 
                        be >= maxReports. This parameter can be set to zero if the
                        timestamps are not required.

                    repeats - 
                        Pointer to an array where the fault repeat counts will be 
                        stored. The repeat count specifies how many fault with the 
                        same code were stored by the firmware before sending the fault
                        report. The size of the array should be >= maxReports. This 
                        parameter can be set to zero if the timestamps are not 
                        required.

                    numReports - 
                        Pointer to a value where the actual number of fault reports 
                        fetched will be stored.

                    timeout - 
                        Used to specify a timeout, in ms, which the software will wait 
                        for HQ fault reports to be returned from BlueCore.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function returns a number of fault reports. If there are
                    existing logged faults, the function will immediately fetch the 
                    fault report fields. If there are no logged fault reports, then 
                    the function will wait for the specified timeout period for a 
                    fault report from BlueCore. 
                    <p>Fault reports can be received at any time after communications
                    are established with BlueCore. This function can be called to
                    check for logged faults.
                    <p>Faults are stored in the order in which they were received,
                    i.e. codes[0] will contain the first fault code in the list.
                    The most recent fault being codes[numReports - 1].
                    <p>This function can be called following a call to 
                    bccmdProvokeFault, which will clear any existing fault reports
                    and provoke a specified fault. In this case, it is possible that
                    a fault report other than that provoked is received prior to 
                    receiving the provoked fault. This will cause the function to
                    return reports without the inclusion the provoked fault. In this
                    case, calling hqGetFaultReports again after a small delay should
                    return reports including the provoked fault.
                    <p>Fault reports can be disabled by PSKEY, in which case
                    hqGetFaultReport() will timeout and fail.

*************************************************************************************/
TESTENGINE_API(int32) hqGetFaultReports(uint32 handle, uint16 maxReports,
                                        uint16 * codes, uint32 * timestamps,
                                        uint16 * repeats, uint16 * numReports,
                                        int32 timeout);

/*************************************************************************************
    
    Function :      int32 teGetFaultDesc(uint32 handle, uint16 faultCode, char * desc)

    Parameters :    handle - 
                        Handle to the device

                    faultCode - 
                        Fault code to get the description for.

                    desc - 
                        Pointer to a char array where the fault description for the
                        code will be stored. The array should be at least 128 bytes
                        long.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                    </table>

    Description :   This function gets a fault description string for the 
                    specified BlueCore fault code.
                    <p>Fault reports sent from BlueCore can be fetched using 
                    hqGetFaultReports.
                
*************************************************************************************/
TESTENGINE_API(int32) teGetFaultDesc(uint32 handle, uint16 faultCode, char * desc);

/*************************************************************************************
 
    Function :      int32 bccmdSetMapScoPcm(uint32 handle, uint8 enable)

    Parameters :    handle - 
                        Handle to the device
  
                    enable - 
                        Value to enable (1) or disable (0) SCO over PCM.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to enable or disable SCO over PCM. The 
                    value set with this command overrides PSKEY_MAP_SCO_PCM for the 
                    next SCO connection created using hciSetupScoConnection.

*************************************************************************************/
TESTENGINE_API(int32) bccmdSetMapScoPcm(uint32 handle, uint8 enable);

/*************************************************************************************
 
    Function :      int32 bccmdGetVrefConstant(uint32 handle, uint16* vref)

    Parameters :    handle - 
                        Handle to the device
  
                    vref - 
                        Pointer to a variable to hold the returned Vref voltage 
                        (milli-volts).

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function is used to get the VREF constant voltage for the 
                    chip. This value can be used when calculating voltages from ADC
                    values (see the help for bccmdGetVrefAdc).

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetVrefConstant(uint32 handle, uint16* vref);

/*************************************************************************************
    
    Function :      int32 bccmdGetVrefAdc(uint32 handle, uint16 * result, 
                                            uint8 * numBits)

    Parameters :    handle - 
                        Handle to the device

                    result - 
                        Holds the result of the ADC reading. This value is from 0 to 
                        ADC_MAX, where ADC_MAX = 255 or 1023 for 8 and 10 bit results 
                        respectively (the numBits parameter indicates 8 or 10 bit 
                        result).

                    numBits - 
                        Holds the number of bits used for the result. In BlueCore 
                        firmware from v24, the ADC readings have been changed to return 
                        a 10 bit result (8 previously). This output parameter indicates 
                        how many bits in the result are valid, 8 or 10, removing the 
                        need to check the firmware version when using this function.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the vref ADC. This function supports both 
                    the old ADC reading method (8 bit results) and the new (10 bit 
                    results). BlueCore devices prior to BC5 have 8 bit ADCs. With 
                    these devices, and where the firmware used uses the new method 
                    (returns 10 bit results), the readings are shifted up by 2 bits.
                    <p>The vref ADC reading can be used to calculate VDD, which can 
                    then be used when calculating voltages from other ADC readings, 
                    e.g. from bccmdGetAio. Calculate VDD (in milli-volts) as follows:
                    <p>VDD = (vref_constant * ADC_MAX) / result
                    <p>The vref_constant can be found using bccmdGetVrefConstant if 
                    supported. Otherwise use the fixed value 1250mV.

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetVrefAdc(uint32 handle, uint16 * result, uint8 * numBits);

/*************************************************************************************
    
    Function :      int32 bccmdBC5FMGetI2CState(uint32 handle, uint8 * sda, 
                                                uint8 * scl)

    Parameters :    handle - 
                        Handle to the device

                    sda - 
                        Pointer to 8 bit unsigned int, used to store the state of the 
                        SDA (I2C_DATA) line. The value will be either 0 (low) or 
                        1 (high).

                    scl - 
                        Pointer to 8 bit unsigned int, used to store the state of the 
                        SCL (I2C_CLK) line. The value will be either 0 (low) or 
                        1 (high).

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the status of the I2C interface lines 
                    on a BC5-FM BlueCore chip. It can therefore be used to test for 
                    open and short circuits on the I2C interface using appropriate 
                    external hardware.
                    <p>The lines are pulled up, therefore with nothing connected, 
                    the values of sda and scl will be 1.
                    <p>This function is for use with BC5-FM only. The results of 
                    calling this function with other chip types is undefined.

*************************************************************************************/
TESTENGINE_API(int32) bccmdBC5FMGetI2CState(uint32 handle, uint8 * sda, uint8 * scl);

/*************************************************************************************
 
    Function :      int32 refEpGetRssiDbm(uint32 handle, uint16 freqMHz, 
                                            float64 rssiChip, float64 * rssiDbm)

    Parameters :    handle - 
                        Handle to the device.

                    freqMHz -
                        The frequency in MHz for which the RSSI value is required.

                    rssiChip -
                        The rssi value, returned from the chip (using hqGetRssi) to 
                        be converted into an RSSI value in dBm. The value is a 
                        floating point value because the values returned from the chip 
                        are usually averaged (as are the calibration values of the 
                        reference endpoint).

                    rssiDbm -
                        Pointer to a value where the RSSI value in dBm will be stored.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used with reference endpoint hardware to 
                    convert RSSI values obtained from the chip to a dBm value, using 
                    the calibration values stored in the reference endpoint.
                    <p>The reference endpoint is calibrated at the following 
                    frequencies:
                    <table>
                        <tr><td>2402 MHz
                        <tr><td>2441 MHz
                        <tr><td>2480 MHz
                    </table>
                    If the freqMHz parameter value is not equal to one of these 
                    frequencies, the closest of the above frequencies will be used. 
                    Frequencies outside of the BlueTooth range produce result in an 
                    error being returned.
                    <p>If the rssiChip parameter value is outside of the range 
                    covered by the reference endpoint's calibration table, an error 
                    code will be returned.
                    <p>Linear interpolation is performed on the calibration data 
                    when calculating the rssiDbm value.
                    <p>Note that if the calibration data has not been pre-loaded 
                    using refEpLoadCalDataFile, the first call to this function will 
                    take a few seconds to complete. This is due to reading the 
                    calibration data from the device. Subsequent calls will not incur 
                    this overhead.

*************************************************************************************/
TESTENGINE_API(int32) refEpGetRssiDbm(uint32 handle, uint16 freqMHz, float64 rssiChip, 
                                        float64 * rssiDbm);

/*************************************************************************************
 
    Function :      int32 refEpGetPaLevel(uint32 handle, uint16 freqMHz, 
                                            float64 targetPowerDbm, uint16 * intPa, 
                                            float64 * powerDbm)

    Parameters :    handle - 
                        Handle to the device.

                    freqMHz -
                        The frequency in MHz for which the internal PA drive level is 
                        required.

                    targetPowerDbm -
                        The power in dBm for which the internal PA drive level is 
                        required.

                    intPa -
                        Pointer to a value where the internal PA drive level will be 
                        stored.

                    powerDbm -
                        Pointer to a value where the actual power in dBm for the 
                        internal PA drive level will be stored.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used with reference endpoint hardware to 
                    convert a required output power, specified in dBm, to the 
                    corresponding internal PA drive level (taken from 
                    the calibration values stored in the reference endpoint). The 
                    intPa value is used with radiotestTx... functions.
                    <p>Some TestEngine transmit functions take both internal and 
                    external PA drive levels as parameters. The external PA drive 
                    level is irrelevant for the reference endpoint hardware (set to 
                    zero).
                    <p>The reference endpoint is calibrated at the following 
                    frequencies:
                    <table>
                        <tr><td>2402 MHz
                        <tr><td>2441 MHz
                        <tr><td>2480 MHz
                    </table>
                    If the freqMHz parameter value is not equal to one of these 
                    frequencies, the closest of the above frequencies will be used. 
                    Frequencies outside of the BlueTooth range produce result in an 
                    error being returned.
                    <p>The internal PA gain value for the power closest to the 
                    targetPowerDbm parameter value will be returned, along with the 
                    actual recorded power for that gain value. If the targetPowerDbm 
                    parameter value is outside of the range covered by the reference 
                    endpoint's calibration table, an error code will be returned.
                    <p>Note that if the calibration data has not been pre-loaded 
                    using refEpLoadCalDataFile, the first call to this function will 
                    take a few seconds to complete. This is due to reading the 
                    calibration data from the device. Subsequent calls will not incur 
                    this overhead.

*************************************************************************************/
TESTENGINE_API(int32) refEpGetPaLevel(uint32 handle, uint16 freqMHz, 
                                        float64 targetPowerDbm, uint16 * intPa, 
                                        float64 * powerDbm);

/*************************************************************************************
 
    Function :      int32 refEpWriteCalDataFile(uint32 handle, const char * filePath)

    Parameters :    handle - 
                        Handle to the device.

                    filePath -
                        This specifies the name of the save file.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used with reference endpoint hardware to 
                    save the calibration data to a file. For subsequent tests, 
                    this file can be loaded using the refEpLoadCalDataFile function 
                    before refEpGetPaLevel or refEpGetRssiDbm are used. This saves 
                    time due to not having to read the calibration data from the 
                    reference endpoint.
                    <p>If filePath is NULL, or if the file specified cannot be 
                    written, an error is returned. An error is also returned if the 
                    data cannot be read from the reference endpoint.
                    <p>If the calibration data has already been loaded, e.g. due to a 
                    call to refEpGetPaLevel or refEpGetRssiDbm, this function will 
                    simply write the data to the file. If the calibration data hasn't 
                    been loaded, it must be read from the reference endpoint, which 
                    will take a few seconds to complete.

*************************************************************************************/
TESTENGINE_API(int32) refEpWriteCalDataFile(uint32 handle, const char * filePath);

/*************************************************************************************
 
    Function :      int32 refEpLoadCalDataFile(uint32 handle, const char * filePath)

    Parameters :    handle - 
                        Handle to the device.

                    filePath -
                        This specifies the name of the file containing the calibration 
                        data.

    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function can be used to pre-load reference endpoint 
                    calibration data from a file previously created using 
                    refEpWriteCalDataFile. This saves time due to not having to read 
                    the calibration data from the reference endpoint when 
                    refEpGetPaLevel or refEpGetRssiDbm are called.
                    <p>If filePath is NULL, or if the file specified cannot be 
                    read, an error is returned. An error is also returned if the file 
                    is not correctly formatted (i.e. if it has been changed from what 
                    was written by refEpWriteCalDataFile).
                    <p>If the calibration data has already been loaded, e.g. due to a 
                    call to refEpGetPaLevel or refEpGetRssiDbm, this function will 
                    overwrite the calibration data.

*************************************************************************************/
TESTENGINE_API(int32) refEpLoadCalDataFile(uint32 handle, const char * filePath);

/*************************************************************************************
    
    Function :      int32 bccmdGetVmStatus(uint32 handle, uint16 * status, 
                                            uint16 * exitCode)

    Parameters :    handle - 
                        Handle to the device

                    status - 
                        Pointer to a value where the VM status will be stored. The 
                        following status values may be returned:
                        <table>
                            <tr><td>0 = VM_STATUS_BOOT (Not initialised yet)
                            <tr><td>1 = VM_STATUS_FAIL (Failed to initialise)
                            <tr><td>2 = VM_STATUS_RUN (Running)
                            <tr><td>3 = VM_STATUS_PANIC (A panic occurred)
                            <tr><td>4 = VM_STATUS_EXIT (Normal termination)
                        </table>

                    exitCode -
                        Pointer to a value where the VM application exit code will 
                        be stored, if the application terminates normally (status 
                        is VM_STATUS_EXIT).
                        <p>If the application is the CVC license checker application, 
                        then the exit codes are as follows:
                        <table>
                            <tr><td>1 = CVC_PRODTEST_PASS
                            <tr><td>2 = CVC_PRODTEST_FAIL
                            <tr><td>3 = CVC_PRODTEST_NO_CHECK
                            <tr><td>4 = CVC_PRODTEST_FILE_NOT_FOUND
                        </table>
                    
    Returns :       <table>
                        <tr><td>-1 = Invalid handle
                        <tr><td>0 = Error
                        <tr><td>1 = Success
                        <tr><td>2 = Unsupported function
                    </table>

    Description :   This function will read the VM status. The exit code from a VM 
                    application is returned if such an application terminated 
                    normally. This function can be used to perform a CVC license check.

    Example :

        // Perform CVC license check

        static const uint16 PSKEY_INITIAL_BOOTMODE = 973;
        static const uint16 BOOT_MODE_CVC_TEST = 4;  // boot mode for CVC license key testing
        static const uint16 BOOT_MODE_NORMAL = 1;    // normal boot mode

        uint32 dutHandle = openTestEngineSpi(1, 0, SPI_LPT);
        if(dutHandle == 0)
        {
            cout << "Failed to connect to device under test" << endl;
            return;
        }

        // Set boot mode for CVC
        uint16 bootMode = BOOT_MODE_CVC_TEST;
        if(psWrite(dutHandle, PSKEY_INITIAL_BOOTMODE, PS_STORES_I, 1, &bootMode) != TE_OK)
        {
            cout << "Failed to set boot mode" << endl;
            closeTestEngine(dutHandle);
            return;
        }
     
        if(bccmdSetColdReset(dutHandle, 0) != TE_OK)
        {
            cout << "Cold reset failed" << endl;
            closeTestEngine(dutHandle);
            return;
        }

        // CVC license key check
        uint16 status;
        uint16 exitCode;
        static const uint16 MAX_ATTEMPTS = 100; // With loop delay 100ms, gives approx 10s timeout
        uint16 attempts(0);
        do
        {
            if (bccmdGetVmStatus(dutHandle, &status, &exitCode) != TE_OK)
            {
                cout << "bccmdGetVmStatus failed" << endl;
                closeTestEngine(dutHandle);
                return;
            }
            Sleep(100);
            ++attempts;
        
        }while((status == VM_STATUS_BOOT || status == VM_STATUS_RUN) && (attempts < MAX_ATTEMPTS));

        // Check returned status and exit code
        if ((status == VM_STATUS_EXIT) && (exitCode == CVC_PRODTEST_PASS))
        {
            cout << "CVC Production Test PASS" << endl;
        }
        else
        {
            cout << "CVC Production Test FAIL" << endl;
        }

        // Set boot mode back to normal
        bootMode = BOOT_MODE_NORMAL;
        if(psWrite(dutHandle, PSKEY_INITIAL_BOOTMODE, PS_STORES_I, 1, &bootMode) != TE_OK)
        {
            cout << "Failed to set boot mode" << endl;
            closeTestEngine(dutHandle);
            return;
        }

        closeTestEngine(dutHandle);

*************************************************************************************/
TESTENGINE_API(int32) bccmdGetVmStatus(uint32 handle, uint16 * status, 
                                        uint16 * exitCode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCTestEngineh */
