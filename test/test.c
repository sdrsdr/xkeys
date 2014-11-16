/***************************************************
 X-Keys Test Program

 The code may be used by anyone for any purpose,
 and can serve as a starting point for developing
 applications using the X-Keys libary.
 
 Alan Ott
 Signal 11 Software
***************************************************/

#include "PieHid32.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void print_buf(char *data, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		printf("%02hhx ", data[i]);
		if ((i+1) % 8 == 0)
			printf("  ");
		if ((i+1) % 16 == 0)
			printf("\n");
	}
	printf("\n\n");

}
void PIE_HID_CALL onHIDEvent (unsigned char *pData, int datalen, void *uData, unsigned int deviceID, unsigned int error) {
	printf ("\nEVENT! devid:%d err:%d datalen:%d data[0]:%d data[1]:%d data[2]:%d ",deviceID,error,datalen,(int)(pData[0]),(int)(pData[1]),(int)(pData[2]));
	fflush (stdout);
}

int main(void)
{
	TEnumHIDInfo info[128];
	long count;
	int i;
	long handle = -1;
	
	unsigned res = EnumeratePIE(PI_VID, info, &count);
	
	for (i = 0; i < count; i++) {
		TEnumHIDInfo *dev = &info[i];
		printf("Found XKeys Device:\n");
		printf("\tPID: %04x\n", dev->PID);
		printf("\tUsage Page: %04x\n", dev->UP);
		printf("\tUsage:      %04x\n", dev->Usage);
		printf("\tVersion: %d\n", dev->Version);
		printf("\tDevicePath %s\n\n",dev->DevicePath);

		handle = dev->Handle;
		unsigned int res = SetupInterfaceEx(handle);
		if (res != 0) {
			printf("Unabe to open device. err: %d\n", res);
		}
		break;
	}
	
	if (handle < 0) {
		printf("Unable to open device\n");
		exit(1);
	}
	

	SetDataCallbackEx (handle,onHIDEvent,NULL,1);

	while (1) {
		sleep(2); putchar('.');fflush (stdout);
	}


	return 0;
}
