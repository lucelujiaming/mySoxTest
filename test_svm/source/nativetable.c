//
// Generated by sedonac 0.1.0
// 08-Jul-2019 18:30
//

#include "sedona.h"

////////////////////////////////////////////////////////////////
// sys (kitId=0)
////////////////////////////////////////////////////////////////

// sys::Str Sys.platformType()
Cell sys_Sys_platformType(SedonaVM* vm, Cell* params);

// void Sys.copy(byte[], int, byte[], int, int)
Cell sys_Sys_copy(SedonaVM* vm, Cell* params);

// byte[] Sys.malloc(int)
Cell sys_Sys_malloc(SedonaVM* vm, Cell* params);

// void Sys.free(sys::Obj)
Cell sys_Sys_free(SedonaVM* vm, Cell* params);

// sys::Str Sys.intStr(int)
Cell sys_Sys_intStr(SedonaVM* vm, Cell* params);

// sys::Str Sys.hexStr(int)
Cell sys_Sys_hexStr(SedonaVM* vm, Cell* params);

// sys::Str Sys.longStr(long)
Cell sys_Sys_longStr(SedonaVM* vm, Cell* params);

// sys::Str Sys.longHexStr(long)
Cell sys_Sys_longHexStr(SedonaVM* vm, Cell* params);

// sys::Str Sys.floatStr(float)
Cell sys_Sys_floatStr(SedonaVM* vm, Cell* params);

// sys::Str Sys.doubleStr(double)
Cell sys_Sys_doubleStr(SedonaVM* vm, Cell* params);

// int Sys.floatToBits(float)
Cell sys_Sys_floatToBits(SedonaVM* vm, Cell* params);

// long Sys.doubleToBits(double)
Cell sys_Sys_doubleToBits(SedonaVM* vm, Cell* params);

// float Sys.bitsToFloat(int)
Cell sys_Sys_bitsToFloat(SedonaVM* vm, Cell* params);

// double Sys.bitsToDouble(long)
Cell sys_Sys_bitsToDouble(SedonaVM* vm, Cell* params);

// long Sys.ticks()
Cell sys_Sys_ticks(SedonaVM* vm, Cell* params);

// void Sys.sleep(long)
Cell sys_Sys_sleep(SedonaVM* vm, Cell* params);

// int Sys.compareBytes(byte[], int, byte[], int, int)
Cell sys_Sys_compareBytes(SedonaVM* vm, Cell* params);

// void Sys.setBytes(int, byte[], int, int)
Cell sys_Sys_setBytes(SedonaVM* vm, Cell* params);

// void Sys.andBytes(int, byte[], int, int)
Cell sys_Sys_andBytes(SedonaVM* vm, Cell* params);

// void Sys.orBytes(int, byte[], int, int)
Cell sys_Sys_orBytes(SedonaVM* vm, Cell* params);

// byte[] Sys.scodeAddr()
Cell sys_Sys_scodeAddr(SedonaVM* vm, Cell* params);

// int Sys.rand()
Cell sys_Sys_rand(SedonaVM* vm, Cell* params);

// void Component.invokeVoid(sys::Slot)
Cell sys_Component_invokeVoid(SedonaVM* vm, Cell* params);

// void Component.invokeBool(sys::Slot, bool)
Cell sys_Component_invokeBool(SedonaVM* vm, Cell* params);

// void Component.invokeInt(sys::Slot, int)
Cell sys_Component_invokeInt(SedonaVM* vm, Cell* params);

// void Component.invokeLong(sys::Slot, long)
Cell sys_Component_invokeLong(SedonaVM* vm, Cell* params);

// void Component.invokeFloat(sys::Slot, float)
Cell sys_Component_invokeFloat(SedonaVM* vm, Cell* params);

// void Component.invokeDouble(sys::Slot, double)
Cell sys_Component_invokeDouble(SedonaVM* vm, Cell* params);

// void Component.invokeBuf(sys::Slot, sys::Buf)
Cell sys_Component_invokeBuf(SedonaVM* vm, Cell* params);

// bool Component.getBool(sys::Slot)
Cell sys_Component_getBool(SedonaVM* vm, Cell* params);

// int Component.getInt(sys::Slot)
Cell sys_Component_getInt(SedonaVM* vm, Cell* params);

// long Component.getLong(sys::Slot)
Cell sys_Component_getLong(SedonaVM* vm, Cell* params);

// float Component.getFloat(sys::Slot)
Cell sys_Component_getFloat(SedonaVM* vm, Cell* params);

// double Component.getDouble(sys::Slot)
Cell sys_Component_getDouble(SedonaVM* vm, Cell* params);

// sys::Buf Component.getBuf(sys::Slot)
Cell sys_Component_getBuf(SedonaVM* vm, Cell* params);

// bool Component.doSetBool(sys::Slot, bool)
Cell sys_Component_doSetBool(SedonaVM* vm, Cell* params);

// bool Component.doSetInt(sys::Slot, int)
Cell sys_Component_doSetInt(SedonaVM* vm, Cell* params);

// bool Component.doSetLong(sys::Slot, long)
Cell sys_Component_doSetLong(SedonaVM* vm, Cell* params);

// bool Component.doSetFloat(sys::Slot, float)
Cell sys_Component_doSetFloat(SedonaVM* vm, Cell* params);

// bool Component.doSetDouble(sys::Slot, double)
Cell sys_Component_doSetDouble(SedonaVM* vm, Cell* params);

// sys::Obj Type.malloc()
Cell sys_Type_malloc(SedonaVM* vm, Cell* params);

// bool StdOutStream.doWrite(int)
Cell sys_StdOutStream_doWrite(SedonaVM* vm, Cell* params);

// bool StdOutStream.doWriteBytes(byte[], int, int)
Cell sys_StdOutStream_doWriteBytes(SedonaVM* vm, Cell* params);

// void StdOutStream.doFlush()
Cell sys_StdOutStream_doFlush(SedonaVM* vm, Cell* params);

// int FileStore.doSize(sys::Str)
Cell sys_FileStore_doSize(SedonaVM* vm, Cell* params);

// sys::Obj FileStore.doOpen(sys::Str, sys::Str)
Cell sys_FileStore_doOpen(SedonaVM* vm, Cell* params);

// int FileStore.doRead(sys::Obj)
Cell sys_FileStore_doRead(SedonaVM* vm, Cell* params);

// int FileStore.doReadBytes(sys::Obj, byte[], int, int)
Cell sys_FileStore_doReadBytes(SedonaVM* vm, Cell* params);

// bool FileStore.doWrite(sys::Obj, int)
Cell sys_FileStore_doWrite(SedonaVM* vm, Cell* params);

// bool FileStore.doWriteBytes(sys::Obj, byte[], int, int)
Cell sys_FileStore_doWriteBytes(SedonaVM* vm, Cell* params);

// int FileStore.doTell(sys::Obj)
Cell sys_FileStore_doTell(SedonaVM* vm, Cell* params);

// bool FileStore.doSeek(sys::Obj, int)
Cell sys_FileStore_doSeek(SedonaVM* vm, Cell* params);

// void FileStore.doFlush(sys::Obj)
Cell sys_FileStore_doFlush(SedonaVM* vm, Cell* params);

// bool FileStore.doClose(sys::Obj)
Cell sys_FileStore_doClose(SedonaVM* vm, Cell* params);

// bool FileStore.rename(sys::Str, sys::Str)
Cell sys_FileStore_rename(SedonaVM* vm, Cell* params);

// int Test.doMain()
Cell sys_Test_doMain(SedonaVM* vm, Cell* params);

// sys::Str Str.fromBytes(byte[], int)
Cell sys_Str_fromBytes(SedonaVM* vm, Cell* params);

// sys::Str PlatformService.doPlatformId()
Cell sys_PlatformService_doPlatformId(SedonaVM* vm, Cell* params);

// sys::Str PlatformService.getPlatVersion()
Cell sys_PlatformService_getPlatVersion(SedonaVM* vm, Cell* params);

// long PlatformService.getNativeMemAvailable()
Cell sys_PlatformService_getNativeMemAvailable(SedonaVM* vm, Cell* params);

// void PlatformService.doReboot()
Cell sys_PlatformService_doReboot(SedonaVM* vm, Cell* params);

// void Runtime.load()
Cell sys_Runtime_load(SedonaVM* vm, Cell* params);

// void Runtime.save()
Cell sys_Runtime_save(SedonaVM* vm, Cell* params);

// int Runtime.timeOpen(sys::Str)
Cell sys_Runtime_timeOpen(SedonaVM* vm, Cell* params);

// int Runtime.timeClose(int)
Cell sys_Runtime_timeClose(SedonaVM* vm, Cell* params);

// int Runtime.timeRead(int, float[])
Cell sys_Runtime_timeRead(SedonaVM* vm, Cell* params);

// int Runtime.timeWrite(int, float[])
Cell sys_Runtime_timeWrite(SedonaVM* vm, Cell* params);

// int Runtime.dataOpen(sys::Str)
Cell sys_Runtime_dataOpen(SedonaVM* vm, Cell* params);

// int Runtime.dataClose(int)
Cell sys_Runtime_dataClose(SedonaVM* vm, Cell* params);

// int Runtime.dataRead(int, float[])
Cell sys_Runtime_dataRead(SedonaVM* vm, Cell* params);

// int Runtime.dataWrite(int, float[])
Cell sys_Runtime_dataWrite(SedonaVM* vm, Cell* params);

// int Runtime.tableA()
Cell sys_Runtime_tableA(SedonaVM* vm, Cell* params);

// int Runtime.tableD(int)
Cell sys_Runtime_tableD(SedonaVM* vm, Cell* params);

// int Runtime.tableC(int, float[], float[])
Cell sys_Runtime_tableC(SedonaVM* vm, Cell* params);

// float Runtime.tableE(int, float)
Cell sys_Runtime_tableE(SedonaVM* vm, Cell* params);

// int Runtime.mathSq(float[])
Cell sys_Runtime_mathSq(SedonaVM* vm, Cell* params);

// int Runtime.mathSqrt(float[])
Cell sys_Runtime_mathSqrt(SedonaVM* vm, Cell* params);

// int Runtime.mathLn(float[])
Cell sys_Runtime_mathLn(SedonaVM* vm, Cell* params);

// int Runtime.mathExp(float[])
Cell sys_Runtime_mathExp(SedonaVM* vm, Cell* params);

// int Runtime.mathExpt(float[])
Cell sys_Runtime_mathExpt(SedonaVM* vm, Cell* params);

// long Runtime.now()
Cell sys_Runtime_now(SedonaVM* vm, Cell* params);

// void Runtime.setTime(long)
Cell sys_Runtime_setTime(SedonaVM* vm, Cell* params);

// native table for kit 0
NativeMethod kitNatives0[] = 
{
  sys_Sys_platformType,           // 0::0
  sys_Sys_copy,                   // 0::1
  sys_Sys_malloc,                 // 0::2
  sys_Sys_free,                   // 0::3
  sys_Sys_intStr,                 // 0::4
  sys_Sys_hexStr,                 // 0::5
  sys_Sys_longStr,                // 0::6
  sys_Sys_longHexStr,             // 0::7
  sys_Sys_floatStr,               // 0::8
  sys_Sys_doubleStr,              // 0::9
  sys_Sys_floatToBits,            // 0::10
  sys_Sys_doubleToBits,           // 0::11
  sys_Sys_bitsToFloat,            // 0::12
  sys_Sys_bitsToDouble,           // 0::13
  sys_Sys_ticks,                  // 0::14
  sys_Sys_sleep,                  // 0::15
  sys_Sys_compareBytes,           // 0::16
  sys_Sys_setBytes,               // 0::17
  sys_Sys_andBytes,               // 0::18
  sys_Sys_orBytes,                // 0::19
  sys_Sys_scodeAddr,              // 0::20
  sys_Sys_rand,                   // 0::21
  sys_Component_invokeVoid,       // 0::22
  sys_Component_invokeBool,       // 0::23
  sys_Component_invokeInt,        // 0::24
  sys_Component_invokeLong,       // 0::25
  sys_Component_invokeFloat,      // 0::26
  sys_Component_invokeDouble,     // 0::27
  sys_Component_invokeBuf,        // 0::28
  sys_Component_getBool,          // 0::29
  sys_Component_getInt,           // 0::30
  sys_Component_getLong,          // 0::31
  sys_Component_getFloat,         // 0::32
  sys_Component_getDouble,        // 0::33
  sys_Component_getBuf,           // 0::34
  sys_Component_doSetBool,        // 0::35
  sys_Component_doSetInt,         // 0::36
  sys_Component_doSetLong,        // 0::37
  sys_Component_doSetFloat,       // 0::38
  sys_Component_doSetDouble,      // 0::39
  sys_Type_malloc,                // 0::40
  sys_StdOutStream_doWrite,       // 0::41
  sys_StdOutStream_doWriteBytes,  // 0::42
  sys_StdOutStream_doFlush,       // 0::43
  sys_FileStore_doSize,           // 0::44
  sys_FileStore_doOpen,           // 0::45
  sys_FileStore_doRead,           // 0::46
  sys_FileStore_doReadBytes,      // 0::47
  sys_FileStore_doWrite,          // 0::48
  sys_FileStore_doWriteBytes,     // 0::49
  sys_FileStore_doTell,           // 0::50
  sys_FileStore_doSeek,           // 0::51
  sys_FileStore_doFlush,          // 0::52
  sys_FileStore_doClose,          // 0::53
  sys_FileStore_rename,           // 0::54
  sys_Test_doMain,                // 0::55
  sys_Str_fromBytes,              // 0::56
  sys_PlatformService_doPlatformId,  // 0::57
  sys_PlatformService_getPlatVersion,  // 0::58
  sys_PlatformService_getNativeMemAvailable,  // 0::59
  sys_PlatformService_doReboot,   // 0::60
  sys_Runtime_load,               // 0::61
  sys_Runtime_save,               // 0::62
  sys_Runtime_timeOpen,           // 0::63
  sys_Runtime_timeClose,          // 0::64
  sys_Runtime_timeRead,           // 0::65
  sys_Runtime_timeWrite,          // 0::66
  sys_Runtime_dataOpen,           // 0::67
  sys_Runtime_dataClose,          // 0::68
  sys_Runtime_dataRead,           // 0::69
  sys_Runtime_dataWrite,          // 0::70
  sys_Runtime_tableA,             // 0::71
  sys_Runtime_tableD,             // 0::72
  sys_Runtime_tableC,             // 0::73
  sys_Runtime_tableE,             // 0::74
  sys_Runtime_mathSq,             // 0::75
  sys_Runtime_mathSqrt,           // 0::76
  sys_Runtime_mathLn,             // 0::77
  sys_Runtime_mathExp,            // 0::78
  sys_Runtime_mathExpt,           // 0::79
  sys_Runtime_now,                // 0::80
  sys_Runtime_setTime,            // 0::81
};

////////////////////////////////////////////////////////////////
// inet (kitId=2)
////////////////////////////////////////////////////////////////

// bool TcpSocket.connect(inet::IpAddr, int)
Cell inet_TcpSocket_connect(SedonaVM* vm, Cell* params);

// bool TcpSocket.finishConnect()
Cell inet_TcpSocket_finishConnect(SedonaVM* vm, Cell* params);

// int TcpSocket.write(byte[], int, int)
Cell inet_TcpSocket_write(SedonaVM* vm, Cell* params);

// int TcpSocket.read(byte[], int, int)
Cell inet_TcpSocket_read(SedonaVM* vm, Cell* params);

// void TcpSocket.close()
Cell inet_TcpSocket_close(SedonaVM* vm, Cell* params);

// bool TcpServerSocket.bind(int)
Cell inet_TcpServerSocket_bind(SedonaVM* vm, Cell* params);

// bool TcpServerSocket.accept(inet::TcpSocket)
Cell inet_TcpServerSocket_accept(SedonaVM* vm, Cell* params);

// void TcpServerSocket.close()
Cell inet_TcpServerSocket_close(SedonaVM* vm, Cell* params);

// bool UdpSocket.open()
Cell inet_UdpSocket_open(SedonaVM* vm, Cell* params);

// bool UdpSocket.bind(int)
Cell inet_UdpSocket_bind(SedonaVM* vm, Cell* params);

// bool UdpSocket.send(inet::UdpDatagram)
Cell inet_UdpSocket_send(SedonaVM* vm, Cell* params);

// bool UdpSocket.receive(inet::UdpDatagram)
Cell inet_UdpSocket_receive(SedonaVM* vm, Cell* params);

// void UdpSocket.close()
Cell inet_UdpSocket_close(SedonaVM* vm, Cell* params);

// int UdpSocket.maxPacketSize()
Cell inet_UdpSocket_maxPacketSize(SedonaVM* vm, Cell* params);

// int UdpSocket.idealPacketSize()
Cell inet_UdpSocket_idealPacketSize(SedonaVM* vm, Cell* params);

// void Crypto.sha1(byte[], int, int, byte[], int)
Cell inet_Crypto_sha1(SedonaVM* vm, Cell* params);

// bool UdpSocket.join()
Cell inet_UdpSocket_join(SedonaVM* vm, Cell* params);

// bool UdpSocket.join()
Cell inet_UdpSocket_getBacnetDeviceList(SedonaVM* vm, Cell* params);

// native table for kit 2
NativeMethod kitNatives2[] = 
{
  inet_TcpSocket_connect,         // 2::0
  inet_TcpSocket_finishConnect,   // 2::1
  inet_TcpSocket_write,           // 2::2
  inet_TcpSocket_read,            // 2::3
  inet_TcpSocket_close,           // 2::4
  inet_TcpServerSocket_bind,      // 2::5
  inet_TcpServerSocket_accept,    // 2::6
  inet_TcpServerSocket_close,     // 2::7
  inet_UdpSocket_open,            // 2::8
  inet_UdpSocket_bind,            // 2::9
  inet_UdpSocket_send,            // 2::10
  inet_UdpSocket_receive,         // 2::11
  inet_UdpSocket_close,           // 2::12
  inet_UdpSocket_maxPacketSize,   // 2::13
  inet_UdpSocket_idealPacketSize,  // 2::14
  inet_Crypto_sha1,               // 2::15
  inet_UdpSocket_join,            // 2::16
  inet_UdpSocket_getBacnetDeviceList,            // 2::17
};

////////////////////////////////////////////////////////////////
// ModBus (kitId=3)
////////////////////////////////////////////////////////////////

// int rtuMaster.dO(int, int, int, int, int, int, int)
Cell ModBus_rtuMaster_dO(SedonaVM* vm, Cell* params);

// int rtuMaster.dC(int)
Cell ModBus_rtuMaster_dC(SedonaVM* vm, Cell* params);

// int rtuMaster.dA(int, int, int, int, int, int)
Cell ModBus_rtuMaster_dA(SedonaVM* vm, Cell* params);

// int rtuMaster.dR(int, int, int, int, float[])
Cell ModBus_rtuMaster_dR(SedonaVM* vm, Cell* params);

// int rtuMaster.dW(int, int, int, int, float[])
Cell ModBus_rtuMaster_dW(SedonaVM* vm, Cell* params);

// int rtuSlave.dO(int, int, int, int, int)
Cell ModBus_rtuSlave_dO(SedonaVM* vm, Cell* params);

// int rtuSlave.dC(int)
Cell ModBus_rtuSlave_dC(SedonaVM* vm, Cell* params);

// int rtuSlave.dA(int, int, int, int)
Cell ModBus_rtuSlave_dA(SedonaVM* vm, Cell* params);

// int rtuSlave.dR(int, int, int, int, float[])
Cell ModBus_rtuSlave_dR(SedonaVM* vm, Cell* params);

// int rtuSlave.dW(int, int, int, int, float[])
Cell ModBus_rtuSlave_dW(SedonaVM* vm, Cell* params);

// int tcpClient.dO(sys::Str, int)
Cell ModBus_tcpClient_dO(SedonaVM* vm, Cell* params);

// int tcpClient.dC(int)
Cell ModBus_tcpClient_dC(SedonaVM* vm, Cell* params);

// int tcpClient.dA(int, int, int, int, int)
Cell ModBus_tcpClient_dA(SedonaVM* vm, Cell* params);

// int tcpClient.dR(int, int, int, int, float[])
Cell ModBus_tcpClient_dR(SedonaVM* vm, Cell* params);

// int tcpClient.dW(int, int, int, int, float[])
Cell ModBus_tcpClient_dW(SedonaVM* vm, Cell* params);

// int tcpServer.dO(int)
Cell ModBus_tcpServer_dO(SedonaVM* vm, Cell* params);

// int tcpServer.dC(int)
Cell ModBus_tcpServer_dC(SedonaVM* vm, Cell* params);

// int tcpServer.dA(int, int, int, int)
Cell ModBus_tcpServer_dA(SedonaVM* vm, Cell* params);

// int tcpServer.dR(int, int, int, int, float[])
Cell ModBus_tcpServer_dR(SedonaVM* vm, Cell* params);

// int tcpServer.dW(int, int, int, int, float[])
Cell ModBus_tcpServer_dW(SedonaVM* vm, Cell* params);

// native table for kit 3
NativeMethod kitNatives3[] = 
{
  ModBus_rtuMaster_dO,            // 3::0
  ModBus_rtuMaster_dC,            // 3::1
  ModBus_rtuMaster_dA,            // 3::2
  ModBus_rtuMaster_dR,            // 3::3
  ModBus_rtuMaster_dW,            // 3::4
  ModBus_rtuSlave_dO,             // 3::5
  ModBus_rtuSlave_dC,             // 3::6
  ModBus_rtuSlave_dA,             // 3::7
  ModBus_rtuSlave_dR,             // 3::8
  ModBus_rtuSlave_dW,             // 3::9
  ModBus_tcpClient_dO,            // 3::10
  ModBus_tcpClient_dC,            // 3::11
  ModBus_tcpClient_dA,            // 3::12
  ModBus_tcpClient_dR,            // 3::13
  ModBus_tcpClient_dW,            // 3::14
  ModBus_tcpServer_dO,            // 3::15
  ModBus_tcpServer_dC,            // 3::16
  ModBus_tcpServer_dA,            // 3::17
  ModBus_tcpServer_dR,            // 3::18
  ModBus_tcpServer_dW,            // 3::19
};

////////////////////////////////////////////////////////////////
// BacNet (kitId=4)
////////////////////////////////////////////////////////////////

// int BIP.dO(int, int, int)
Cell BacNet_BIP_dO(SedonaVM* vm, Cell* params);

// int BIP.dC(int)
Cell BacNet_BIP_dC(SedonaVM* vm, Cell* params);

// int BIP.dA(int, int, int, int, int, int)
Cell BacNet_BIP_dA(SedonaVM* vm, Cell* params);

// int BIP.dR(int, int, int, int, float[])
Cell BacNet_BIP_dR(SedonaVM* vm, Cell* params);

// int BIP.dW(int, int, int, int, float[])
Cell BacNet_BIP_dW(SedonaVM* vm, Cell* params);

// int MSTP.dO(int, int, int, int, int)
Cell BacNet_MSTP_dO(SedonaVM* vm, Cell* params);

// int MSTP.dC(int)
Cell BacNet_MSTP_dC(SedonaVM* vm, Cell* params);

// int MSTP.dA(int, int, int, int, int, int)
Cell BacNet_MSTP_dA(SedonaVM* vm, Cell* params);

// int MSTP.dR(int, int, int, int, float[])
Cell BacNet_MSTP_dR(SedonaVM* vm, Cell* params);

// int MSTP.dW(int, int, int, int, float[])
Cell BacNet_MSTP_dW(SedonaVM* vm, Cell* params);

// int BIP.dS(int, byte[], int[], float[])
Cell BacNet_BIP_dS(SedonaVM* vm, Cell* params);

// int BIP.dT(int, int, int[], float[])
Cell BacNet_BIP_dT(SedonaVM* vm, Cell* params);

// int MSTP.dS(int, byte[], int[], float[])
Cell BacNet_MSTP_dS(SedonaVM* vm, Cell* params);

// int MSTP.dT(int, int, int[], float[])
Cell BacNet_MSTP_dT(SedonaVM* vm, Cell* params);

// int BIP.dD(int, byte[], int[], float[])
Cell BacNet_BIP_dD(SedonaVM* vm, Cell* params);

// int BIP.dF(int, int, int[], float[])
Cell BacNet_BIP_dF(SedonaVM* vm, Cell* params);

// int BIP.dD(int, byte[], int[], float[])
Cell BacNet_BIP_eD(SedonaVM* vm, Cell* params);

// int BIP.dF(int, int, int[], float[])
Cell BacNet_BIP_eF(SedonaVM* vm, Cell* params);

// native table for kit 4
NativeMethod kitNatives4[] = 
{
  BacNet_BIP_dO,                  // 4::0
  BacNet_BIP_dC,                  // 4::1
  BacNet_BIP_dA,                  // 4::2
  BacNet_BIP_dR,                  // 4::3
  BacNet_BIP_dW,                  // 4::4
  BacNet_MSTP_dO,                 // 4::5
  BacNet_MSTP_dC,                 // 4::6
  BacNet_MSTP_dA,                 // 4::7
  BacNet_MSTP_dR,                 // 4::8
  BacNet_MSTP_dW,                 // 4::9
  BacNet_BIP_dS,                  // 4::10
  BacNet_BIP_dT,                  // 4::11
  BacNet_MSTP_dS,                 // 4::12
  BacNet_MSTP_dT,                 // 4::13
  BacNet_BIP_dD,                  // 4::14
  BacNet_BIP_dF,                  // 4::15
  BacNet_BIP_eD,                  // 4::16
  BacNet_BIP_eF,                  // 4::17
};

////////////////////////////////////////////////////////////////
// PID (kitId=10)
////////////////////////////////////////////////////////////////

// int Pid.a()
Cell PID_Pid_a(SedonaVM* vm, Cell* params);

// float Pid.f(int, float, float, float, float, float, float, float, float)
Cell PID_Pid_f(SedonaVM* vm, Cell* params);

// int Pid.d(int)
Cell PID_Pid_d(SedonaVM* vm, Cell* params);

// native table for kit 10
NativeMethod kitNatives10[] = 
{
  NULL,                           // 10::0
  NULL,                           // 10::1
  NULL,                           // 10::2
  PID_Pid_a,                      // 10::3
  PID_Pid_f,                      // 10::4
  PID_Pid_d,                      // 10::5
};

////////////////////////////////////////////////////////////////
// Native Table
////////////////////////////////////////////////////////////////

NativeMethod* nativeTable[] = 
{
  kitNatives0,     // 0
  NULL,            // 1
  kitNatives2,     // 2
  kitNatives3,     // 3
  kitNatives4,     // 4
  NULL,            // 5
  NULL,            // 6
  NULL,            // 7
  NULL,            // 8
  NULL,            // 9
  kitNatives10,    // 10
};

////////////////////////////////////////////////////////////////
// Native Id Check
////////////////////////////////////////////////////////////////

#ifdef SCODE_DEBUG
int isNativeIdValid(int kitId, int methodId)
{
  switch(kitId)
  {
    case 0:
      if (methodId >= 82) return 0;
      else return kitNatives0[methodId] != NULL;
    case 2:
      if (methodId >= 18) return 0;
      else return kitNatives2[methodId] != NULL;
    case 3:
      if (methodId >= 20) return 0;
      else return kitNatives3[methodId] != NULL;
    case 4:
      // jiaming.lu adjust it @03/20/2023
      // if (methodId >= 14) return 0;
      if (methodId >= 18) return 0;
      else return kitNatives4[methodId] != NULL;
    case 10:
      if (methodId >= 6) return 0;
      else return kitNatives10[methodId] != NULL;
    default:
       return 0;
  }
}
#endif


#ifdef WIN32
//  //  Cell sys_Sys_platformType(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_copy(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_malloc(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_free(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_intStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_hexStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_longStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_longHexStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_floatStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_doubleStr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_floatToBits(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_doubleToBits(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_bitsToFloat(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_bitsToDouble(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_Sys_ticks(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_Sys_sleep(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_compareBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_setBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_andBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_orBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_scodeAddr(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Sys_rand(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeVoid(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeBool(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeInt(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeLong(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeFloat(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeDouble(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_invokeBuf(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getBool(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getInt(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getLong(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getFloat(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getDouble(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_getBuf(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_doSetBool(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_doSetInt(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_doSetLong(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_doSetFloat(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Component_doSetDouble(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Type_malloc(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_StdOutStream_doWrite(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_StdOutStream_doWriteBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_StdOutStream_doFlush(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doSize(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doOpen(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doRead(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doReadBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doWrite(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doWriteBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doTell(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doSeek(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doFlush(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_doClose(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_FileStore_rename(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Test_doMain(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Str_fromBytes(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_PlatformService_doPlatformId(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_PlatformService_getPlatVersion(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_PlatformService_getNativeMemAvailable(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  sCell sys_PlatformService_doReboot(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_load(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_save(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_timeOpen(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_timeClose(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_timeRead(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_timeWrite(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_dataOpen(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_dataClose(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_dataRead(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_dataWrite(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_tableA(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_tableD(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_tableC(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_tableE(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_mathSq(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_mathSqrt(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_mathLn(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_mathExp(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell sys_Runtime_mathExpt(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_Runtime_now(SedonaVM* vm, Cell* params){   return nullCell; }
//  //  Cell sys_Runtime_setTime(SedonaVM* vm, Cell* params){   return nullCell; }

//	Cell inet_TcpSocket_connect(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpSocket_finishConnect(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpSocket_write(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpSocket_read(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpSocket_close(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpServerSocket_bind(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpServerSocket_accept(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_TcpServerSocket_close(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_open(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_bind(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_send(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_receive(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_close(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_maxPacketSize(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_idealPacketSize(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_Crypto_sha1(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_join(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell inet_UdpSocket_getBacnetDeviceList(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuMaster_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuMaster_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuMaster_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuMaster_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuMaster_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuSlave_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuSlave_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuSlave_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuSlave_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_rtuSlave_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpClient_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpClient_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpClient_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpClient_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpClient_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpServer_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpServer_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpServer_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpServer_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//		Cell ModBus_tcpServer_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dO(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dC(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dA(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dR(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dW(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dS(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dT(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dS(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_MSTP_dT(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dD(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_dF(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_eD(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell BacNet_BIP_eF(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell PID_Pid_a(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell PID_Pid_f(SedonaVM* vm, Cell* params){   return nullCell; }
//	Cell PID_Pid_d(SedonaVM* vm, Cell* params){   return nullCell; }

#endif


