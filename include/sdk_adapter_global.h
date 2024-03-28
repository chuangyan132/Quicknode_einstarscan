#ifndef SN3D_SDK_ADAPTER_GLOBAL_H
#define SN3D_SDK_ADAPTER_GLOBAL_H

#include <QtGlobal>
#include <QByteArray>
#include <QVariantMap>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QtDebug>
#include <QLoggingCategory>

#if defined(SDK_ADAPTER_LIBRARY)
#define SDK_ADAPTER_EXPORTS Q_DECL_EXPORT
#else
#define SDK_ADAPTER_EXPORTS Q_DECL_IMPORT
#endif

Q_DECLARE_LOGGING_CATEGORY(log_sdk)
Q_DECLARE_LOGGING_CATEGORY(log_sdk_replier)

#define BEGIN_SDK_ADAPTER_NAMESPACE namespace Sn3D{namespace SDK{
#define END_SDK_ADAPTER_NAMESPACE }}

#define SDK_MAX_MSG_FRAME_SIZE          1048576 //1MB

#define SDK_API_VERSION             "v3.0"  // liuyu01 ??¡À?¨¢??-?¨¨¨º??¨² CMakeList.txt ?¡§¨°?¦Ì?¡ê??a¨¢??2¨¬??¨¬2a¨ª¡§1y¡ê??¡§¨°?¦Ì??a¨¤?¡ê??- CMakeList ¦Ì?¡Á¡é¨º¨ª

#define SDK_MSG_TYPE_EVENT          "event" //event
#define SDK_MSG_TYPE_METHOD_INVOKE  "invoke" //invoke
#define SDK_MSG_TYPE_PROP_GET       "propGet" //property
#define SDK_MSG_TYPE_PROP_SET       "propSet" //property
#define SDK_MSG_TYPE_PROP_CHANGED   "propChanged" //property
#define SDK_MSG_TYPE_PING           "ping" //ping

#define SDK_SYSTEM_OBJECT_NAME      "$SYS"

#define SDK_MSG_TOKEN_DATA          "data"
#define SDK_MSG_TOKEN_OBJECT_NAME   "object"
#define SDK_MSG_TOKEN_INVOKE_STATUS "status"
#define SDK_MSG_TOKEN_ERROR_CODE    "error"
#define SDK_MSG_TOKEN_WHY           "why"
#define SDK_MSG_TOKEN_ACTION_TYPE   "action"
#define SDK_MSG_TOKEN_API_PROPERTY  "properties"
#define SDK_MSG_TOKEN_API_EVENT     "events"
#define SDK_MSG_TOKEN_API_METHOD    "methods"

#define SDK_PROP_ACTION_CHANGED "changed"
#define SDK_PROP_ACTION_GET     "get"
#define SDK_PROP_ACTION_SET     "set" 

#define SDK_SYSTEM_ACTION_OBJECT_API    "api"

#define SDK_ERR_NO_ERROR                    0x00
#define SDK_ERR_NET_CTX_CREATE_ERROR        0x01
#define SDK_ERR_REPLY_BIND_ERROR            0x02
#define SDK_ERR_API_VERSION_INVALID         0x03
#define SDK_ERR_ENVELOPE_FORMAT_INVALID     0x04
#define SDK_ERR_PAYLOAD_FORMAT_INVALID          0x05
#define SDK_ERR_REPLY_ERROR                 0x06
#define SDK_ERR_OBJECT_ALREADY_ADDED        0x07
#define SDK_ERR_OBJECT_NOT_EXIST            0x08
#define SDK_ERR_REPLIER_RECV_FAILED         0x09
#define SDK_ERR_REQUEST_FRAMES_INVALID      0x0A
#define SDK_ERR_PROPERTY_NOT_EXIST          0x0B
#define SDK_ERR_METHOD_NOT_EXIST            0x0C
#define SDK_ERR_METHOD_PARAMETERS_MISMATCH  0x0D
#define SDK_ERR_METHOD_INVOKE_FAILED        0x0E
#define SDK_ERR_PROPERTY_READONLY           0x0F
#define SDK_ERR_PROPERTY_WRITE_FAILED       0x10
#define SDK_ERR_UNKNOWN_REQUEST_TYPE        0x11
#define SDK_ERR_DUPLICATE_METHODS           0x12
#define SDK_ERR_ADAPTER_ALREADY_RUNNING     0x13
#define SDK_ERR_PUB_PORT_BIND_ERROR         0x14
#define SDK_ERR_MSG_FRAME_COUNT_INVALID     0x15

#endif//SN3D_SDK_ADAPTER_GLOBAL_H
