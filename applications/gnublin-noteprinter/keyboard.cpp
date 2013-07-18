#include "keyboard.h"

usb_dev_handle* keyboard_init(){
	usb_dev_handle *udev;
	struct usb_bus *bus;
	struct usb_device *dev;

	// search for the scanner, open the device and claim interface
	usb_init();
	usb_find_busses();
	usb_find_devices();

	// find device with vendor and product id 0x24ae 0x1001
	for( bus=usb_get_busses(); bus; bus = bus->next )
		for( dev = bus->devices; dev; dev = dev->next )
			if( dev->descriptor.idVendor==0x24ae && dev->descriptor.idProduct==0x1001 ){
				udev = usb_open(dev);

				//remove all drivers from this device
				usb_detach_kernel_driver_np(udev, 0);
				usb_claim_interface(udev, 0);
				return udev;
			}
}

// close the file handle and release the device
void keyboard_close(usb_dev_handle* udev){
	usb_release_interface(udev, 0);
	usb_close(udev);
}

// get the keyboard input
void getInput(usb_dev_handle* udev, char *buffer){

	usb_interrupt_read(udev,1,buffer,8,400);
}
