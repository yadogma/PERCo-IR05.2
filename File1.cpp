#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif


#include <stdio.h>

#include <windows.h>
#include <iostream>
using namespace std;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

HANDLE hPort; // структура для управления com-port (h_port - адрес)
LPCTSTR sPortName = "COM5";
DCB dcbSerialParams = { 0 };


void WriteComPort(void);
void ReadComPort(void);

string seriya = "";
string nomer = "";
string karta = "";
string karta16;

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Begin:" << '\n';
	while (1)
	{
		WriteComPort();
		ReadComPort();
		CloseHandle(hPort);
	}
    CloseHandle(hPort);
	return 0;
}

void WriteComPort(void) {
	byte data_for_controller1[] = {242, 255, 1, 2, 3};
	byte data_for_controller[] = { 242, 255, 3, 1, 0, 26, 24 };
	DWORD dwSize = sizeof(data_for_controller);
	DWORD dwSize1 = sizeof(data_for_controller1);
	DWORD dwBytesWritten;

	hPort = CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hPort == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist \n";
		}
		else
		{                                                        \

			cout << "other error \n";
		}
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hPort, &dcbSerialParams))
	{
		cout << "getting state error\n";
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hPort, &dcbSerialParams))
	{
			cout << "error setting serial port state \n";
	}
	WriteFile(hPort, data_for_controller, dwSize, &dwBytesWritten, NULL);
	WriteFile(hPort, data_for_controller1, dwSize1, &dwBytesWritten, NULL);
}

void ReadComPort(void)
{
	DWORD iSize;
	byte buffer[32];
	int len =  sizeof(buffer);
	memset(buffer,0,len);

		ReadFile(hPort, &buffer, len, &iSize, 0);
		if (iSize > 0)
		{
			for (int i = 2; i < sizeof(buffer); i = i + 8)
			{
				if (buffer[i] == 8)
				{
					int nomk = (buffer[i + 5] >> 1) + (buffer[i + 6] & 1) * 128;
					int ss = (buffer[i + 6] >> 1) + (buffer[i + 7] & 1) * 128;
					nomk = nomk + ss * 256;
					int serk = (buffer[i + 7] >> 1) + (buffer[i + 8] & 1) * 128;
					cout << serk << " " << nomk;
					cout << "\n";

				}
			}
		}

}

