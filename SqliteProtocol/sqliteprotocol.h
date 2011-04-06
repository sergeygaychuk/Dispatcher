#ifndef SQLITEPROTOCOL_H
#define SQLITEPROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#define SP_DEVICE_TYPE_TEMPERATURE_SENSOR 0x01
#define SP_DEVICE_TYPE_RELAY 0x02
#define SP_DEVICE_TYPE_UNKNOWN 0xFF

typedef struct sp_device sp_device;
struct sp_device {
  char *m_Address;
  int m_Type;
};

void sp_devices_clear();
int sp_devices_length();
void sp_devices_enumerate(int (*)(void *aUserData, sp_device *aDevice), void *aUserData);

sp_device* sp_device_add(const char *aAddress, int aType);
void sp_device_free(sp_device *aDevice);

typedef struct sp_device_data sp_device_data;
struct sp_device_data {
    int m_Date;
    char *m_State;
};

void sp_device_add_data(sp_device *, int aDate, const char *aState);
sp_device_data * sp_device_get_last_data(sp_device *);
void sp_device_data_free(sp_device_data *);

#ifdef __cplusplus
}
#endif


#endif // SQLITEPROTOCOL_H
