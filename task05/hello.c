#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>


static int __keyboard_probe(struct usb_interface *intf, const struct usb_device_id *id) 
{
	dev_dbg(&intf->dev, "USB keyboard attached");
	return 0;
}

static void __keyboard_disconect(struct usb_interface *intf) 
{
	dev_dbg(&intf->dev, "USB keybaord detached");
}

static const struct usb_device_id keyboard_table[] = { 
	{
		USB_INTERFACE_INFO(
			USB_INTERFACE_CLASS_HID, 
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD)
	},
	{},
};

MODULE_DEVICE_TABLE(usb, keyboard_table);

static struct usb_driver keyboard_driver = {
	.name = "keyboard_driver",
	.probe = __keyboard_probe,
	.disconnect = __keyboard_disconect,
	.id_table = keyboard_table
};


static int __init keyboard_init(void)
{
	return usb_register(&keyboard_driver);
}

static void __exit keyboard_exit(void)
{
	usb_deregister(&keyboard_driver);
}


module_init(keyboard_init);
module_exit(keyboard_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c140882fdd04");
MODULE_DESCRIPTION("Keyboard hotplug module");
