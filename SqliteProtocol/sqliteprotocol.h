#ifndef SQLITEPROTOCOL_H
#define SQLITEPROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sp_device_list sp_device_list;

#define SP_DEVICE_TYPE_TEMPERATURE_SENSOR 0x01
#define SP_DEVICE_TYPE_RELAY 0x02

typedef struct sp_device sp_device;
struct sp_device {
    char *m_Address;
    int m_Type;
};

//Return count of devices
//Return pointer to device list
//For free device list call sp_close_device_list
int sp_get_device_list(sp_device_list **);
//return device on specific position
//return null if error
sp_device* sp_get_device_from_list(sp_device_list *, int iPosition);
//close device list
void sp_close_device_list(sp_device_list *);

#ifdef __cplusplus
}
#endif


#endif // SQLITEPROTOCOL_H
