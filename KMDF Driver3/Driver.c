#include <ntddk.h>

typedef struct
{
	PDEVICE_OBJECT 	NextLayerDeviceObject;
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

NTSTATUS
SIMPLE_AddDevice
(
	IN PDRIVER_OBJECT DriverObject,
	IN PDEVICE_OBJECT PhysicalDeviceObject
)
{
	NTSTATUS returnStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = NULL;
	PDEVICE_EXTENSION deviceExtension;

	returnStatus = IoCreateDevice
	(
		DriverObject,
		sizeof(DEVICE_EXTENSION),
		NULL,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&DeviceObject
	);

	deviceExtension = DeviceObject->DeviceExtension;

	deviceExtension->NextLayerDeviceObject =
		IoAttachDeviceToDeviceStack(
			DeviceObject,
			PhysicalDeviceObject
		);

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	return returnStatus;
}

NTSTATUS
SIMPLE_PnpDispatch
(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	PIO_STACK_LOCATION pStack;
	PDEVICE_EXTENSION deviceExtension;
	NTSTATUS returnStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT NextLayerDeviceObject;

	pStack = IoGetCurrentIrpStackLocation(Irp);
	deviceExtension = DeviceObject->DeviceExtension;
	NextLayerDeviceObject = deviceExtension->NextLayerDeviceObject;

	switch (pStack->MinorFunction)
	{
	case IRP_MN_REMOVE_DEVICE:
	{
		IoDetachDevice(NextLayerDeviceObject);
		IoDeleteDevice(DeviceObject);
	}
	break;
	}

	IoSkipCurrentIrpStackLocation(Irp);
	returnStatus = IoCallDriver(NextLayerDeviceObject, Irp);

	return returnStatus;
}

VOID
SIMPLE_Unload
(
	IN PDRIVER_OBJECT DriverObject
)
{
	DriverObject;
}

NTSTATUS
DriverEntry
(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath

)
{
	NTSTATUS returnStatus = STATUS_SUCCESS;

	DriverObject->DriverUnload = SIMPLE_Unload;
	DriverObject->DriverExtension->AddDevice = SIMPLE_AddDevice;
	DriverObject->MajorFunction[IRP_MJ_PNP] = SIMPLE_PnpDispatch;

	RegistryPath;

	return returnStatus;
}

