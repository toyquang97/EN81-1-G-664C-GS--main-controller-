
#include "typedef.h"
#include "parameter.h"
#include "version.h"
#include "bootloader.h"

//�汾��¼
//V3.0C: ����İ汾

// ���������� bootloader �����ڰ汾У�� �� ������������ڹ̼����
// ����������Ĵ�С��˳������ɾ������
// edited by fhm

#pragma arm section code = "ER_IROM2", rodata = "ER_IROM2"

const DWORD identity [ID_NUMS] = 
{
	0x000000B5,								// vendor ID
	0x00000000,								// product code
	0x00000000,								// revision number
	0x00000000,								// serial number
};

const BYTE version_hse [SW_SIZE] = "V3.1C";		// actual software version of MCU

const BYTE hardware_hse [HW_SIZE] = "V1.3";		// actual hardware version of MCU
const BYTE device_name [DV_SIZE] = "G-664";		// name of PCB is G-664
const WORD version_fver = 0x5AA5;
const DWORD version_chksum = VERSION_CONTENT;

#pragma arm section

