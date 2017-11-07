/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFDriver3,
    0x864be2cc,0xce91,0x4373,0x9f,0x47,0x55,0x2f,0xe7,0x52,0x71,0x05);
// {864be2cc-ce91-4373-9f47-552fe7527105}
