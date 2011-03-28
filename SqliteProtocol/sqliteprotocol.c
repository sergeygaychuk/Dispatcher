
#include <stdlib.h>
#include <string.h>


void* sp_zalloc(size_t szCount) {
    void *buffer = malloc(szCount);
    if (buffer) {
        memset(buffer, 0, szCount);
    }
    return buffer;
}

void sp_free(void *aBuffer) {
    free(aBuffer);
}

#define SP_DEVICE_TYPE_TEMPERATURE_SENSOR 0x01
#define SP_DEVICE_TYPE_RELAY 0x02

typedef struct sp_device sp_device;
struct sp_device {
    char *m_Address;
    int m_Type;
};

typedef struct sp_device_list sp_device_list;
struct sp_device_list {
    int m_Length;
    sp_device *m_Devices;
};

//Return count of devices
//Return pointer to device list
//For free device list call sp_close_device_list
int sp_get_device_list(sp_device_list **aList) {
    if (!aList) return -1;
    *aList = sp_zalloc(sizeof(sp_device_list));
    if (!*aList) return (*aList = 0),-1;

    //TEST FILL
    (*aList)->m_Length = 2;
    (*aList)->m_Devices = sp_zalloc(sizeof(sp_device) * 2);
    if (!(*aList)->m_Devices) return -1; //Memory leak
    (*aList)->m_Devices[0].m_Address = sp_zalloc(strlen("AAFF1234") + 1);
    strcpy((*aList)->m_Devices[0].m_Address, "AAFF1234");
    (*aList)->m_Devices[0].m_Type = SP_DEVICE_TYPE_TEMPERATURE_SENSOR;
    (*aList)->m_Devices[1].m_Address = sp_zalloc(strlen("1234AAFF") + 1);
    strcpy((*aList)->m_Devices[1].m_Address, "1234AAFF");
    (*aList)->m_Devices[1].m_Type = SP_DEVICE_TYPE_RELAY;
    //TEST FILL
    return (*aList)->m_Length;
}

//return device on specific position
//return null if error
sp_device* sp_get_device_from_list(sp_device_list *aList, int aPosition) {
    if (!aList || aList->m_Length <= aPosition || aPosition < 0)
        return 0;
    return &aList->m_Devices[aPosition];
}

//close device list
void sp_close_device_list(sp_device_list *aList) {
    if (aList) {
        sp_free(aList);
    }
}
