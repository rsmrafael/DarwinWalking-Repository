#ifndef USB_H_
#define USB_H_

void USB_Init(void);
void USBWaitForConfig(void);
unsigned char USBWaitForConfigTimeout(unsigned int timeout);
unsigned int USBRead(char *pData, unsigned int maxLength);
unsigned int USBWrite(const char *pData, unsigned int length);
void USBDisconnect(void);
void USB_print_ascii(const char *buffer);
void USB_print_char(const unsigned char ch);
void USB_send(const char* buffer, unsigned int length);
int USBRead_nonblocking(char *buf, unsigned int len);

#endif /*USB_H_*/
