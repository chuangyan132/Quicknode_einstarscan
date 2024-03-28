#ifndef SN3DERRORCODE_H
#define SN3DERRORCODE_H

typedef int SN3DERROR;

#define  EC_SUCCESS										0x00000000 // 成功								No error
#define  EC_NOTINITIALIZED								0x00000001  // 没有初始化						No initialization
#define  EC_INITIALIZEFIAILZED							0x00000002  // 初始化失败						Initialization failed.
#define  EC_AlREADYINITIALIZED							0x00000003  // 重复初始化						Initialization repeat.
#define  EC_SAVEFAILED_TYPEERROR						0x00000004  // 保存失败，格式错误				Format error，failed to save files.
#define  EC_CHECKDEVICEFAILED_PLENOTRIGHT				0x00000005  // 连接设备失败，ple不正确			PLE error, failed to connect device.
#define  EC_CHECKDEVICEFAILED_NODEVICEFOUND				0x00000006  // 连接设备失败，未发现设备			Device no found, connection failed.
#define  EC_OPENORCREATSLNFAILED						0x00000007   // 创建或打开工程失败				Failed to create or open project.
#define  EC_ENTERSCANFAILED								0x00000008        // 进入扫描失败				Failed to enter scan mode.
#define  EC_SCANFAILED									0x00000009        // 扫描中失败					Scanning failed.
#define  EC_ENDSCANFAILED								0x0000000A        // 生成失败					Generating failed.
#define  EC_MESHFAILED									0x0000000B        // 网格化失败					Meshing failed.
#define  EC_EXITSCANFAILED								0x0000000C        // 退出扫描					Quitting scan mode failed.
#define  EC_CANCELSCANFAILED							0x0000000D        // 取消扫描					Canceling scan failed.
#define  EC_SAVEFAILED									0x0000000E        // 保存失败					Saving failed.
#define  EC_CREATNEWPROJECTFAILED						0x00000010        // 新建工程失败				New project failed.
#define  EC_WORKRANGE									0x00000011	//工作距离范围不符合要求			The working range dose not meet the requirements

#define  EC_PROCESSERROR								0x10000000   // 流程调用错误					Process call error
#endif // SN3DERRORCODE_H
