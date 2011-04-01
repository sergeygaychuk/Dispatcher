#include <stdlib.h>
#include <string.h>

typedef struct sp_disposable sp_disposable;
struct sp_disposable {
    void *m_This;
    void (*dispose)(sp_disposable*);
};

void init_sp_disposable(sp_disposable *aObj, void *aThis, void (*aDispose)(sp_disposable*)) {
  aObj->dispose = aDispose;
  aObj->m_This = aThis;
}

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

#define NEW(struct_name) (struct_name##_constructor())
#define NEW_ARR(struct_name, length) (struct_name##_constructor(length))
#define DELETE(obj) (obj->m_Disposer.dispose(((sp_disposable*)&obj->m_Disposer)), sp_free(obj))
#define DELETE_ARR(obj, length) \
{ \
  int i = 0; \
  for(i = 0; i < length; ++ i) { \
    obj[i].m_Disposer.dispose((sp_disposable*)&obj[i].m_Disposer); \
  } \
  sp_free(obj); \
}


typedef struct sp_device_data sp_device_data;
struct sp_device_data {
    int m_Date;
    char *m_State;
};

#define SP_DEVICE_TYPE_UNKNOWN 0xFF
#define SP_DEVICE_TYPE_TEMPERATURE_SENSOR 0x01
#define SP_DEVICE_TYPE_RELAY 0x02

typedef struct sp_device sp_device;
struct sp_device {
    char *m_Address;
    int m_Type;
};

typedef struct sp_device_i sp_device_i;
struct sp_device_i {
    sp_device m_Device;
    sp_disposable m_Disposer;
    sp_device_data m_Data;
    void (*set_device)(sp_device_i *aThis, const char *aAddress, int aType);
    sp_device_data* (*get_data)(sp_device_i *aThis);
    void (*add_data)(sp_device_i *aThis, int aDate, const char *aState);
};

void sp_device_i_destructor(sp_disposable *aThis) {
  sp_device_i *th = (sp_device_i*)aThis->m_This;
  if (th->m_Device.m_Address) {
    sp_free(th->m_Device.m_Address);
  }
  if (th->m_Data.m_State) {
      sp_free(th->m_Data.m_State);
  }
}

void sp_device_i_set_device(sp_device_i *aThis, const char *aAddress, int aType) {
  if (aThis && !aThis->m_Device.m_Address) {
    aThis->m_Device.m_Address = (char *)sp_zalloc(sizeof(char) * (strlen(aAddress) + 1));
    strcpy(aThis->m_Device.m_Address, aAddress);
    aThis->m_Device.m_Type = aType;
  }
}

sp_device_data* sp_device_i_get_data(sp_device_i *aThis) {
    if (aThis) {
        return &aThis->m_Data;
    }
    return 0;
}

void sp_device_i_add_data(sp_device_i *aThis, int aDate, const char *aState) {
    if (aThis) {
        if (aThis->m_Data.m_State) sp_free(aThis->m_Data.m_State);
        aThis->m_Data.m_Date = aDate;
        aThis->m_Data.m_State = sp_zalloc(strlen(aState) + 1);
        strcpy(aThis->m_Data.m_State, aState);
    }
}

sp_device_i* sp_device_i_constructor(int aLength) {
    int i = 0;
    sp_device_i *obj = (sp_device_i *)sp_zalloc(sizeof(sp_device_i) * aLength);
    if (obj) {
        for (i = 0; i < aLength; ++ i) {
          init_sp_disposable(&obj[i].m_Disposer, &obj[i], sp_device_i_destructor);
          obj[i].set_device = &sp_device_i_set_device;
          obj[i].get_data = &sp_device_i_get_data;
          obj[i].add_data = &sp_device_i_add_data;
          obj[i].m_Device.m_Address = 0;
          obj[i].m_Device.m_Type = SP_DEVICE_TYPE_UNKNOWN;
          obj[i].m_Data.m_Date = 0;
          obj[i].m_Data.m_State = 0;
        }
    }
    return obj;
}

typedef struct sp_device_list sp_device_list;
struct sp_device_list {
    sp_disposable m_Disposer;
    int m_Length;
    sp_device_i *m_Devices;
    void (*add_device)(sp_device_list *aThis, const char *aAddress, int aType);
};

void sp_device_list_destructor(sp_disposable* aThis) {
  sp_device_list *th = (sp_device_list *)aThis->m_This;
  if (th->m_Devices) {
    DELETE_ARR(th->m_Devices, th->m_Length);
    th->m_Length = 0;
    th->m_Devices = 0;
  }
}

void sp_device_list_init_device_list(sp_device_list *aThis, int aLength) {
  if (!aThis->m_Devices) {
    aThis->m_Length = aLength;
    aThis->m_Devices = NEW_ARR(sp_device_i, aLength);
  }
}

void sp_device_list_set_device(sp_device_list *aThis, int aIdx, const char *aAddress, int aType) {
  if (aThis->m_Devices && aThis->m_Length > aIdx) {
    aThis->m_Devices[aIdx].set_device(&aThis->m_Devices[aIdx], aAddress, aType);
  }
}

void sp_device_list_add_device(sp_device_list *aThis, const char *aAddress, int aType) {
    if (aThis) {
        int aOldLength = 0;
        sp_device_i *aOldDevices = 0;
        if (aThis->m_Devices) {
            aOldDevices = aThis->m_Devices;
            aOldLength = aThis->m_Length;
            aThis->m_Length = 0;
            aThis->m_Devices = 0;
        }
        sp_device_list_init_device_list(aThis, aOldLength + 1);
        memcpy(aThis->m_Devices, aOldDevices, sizeof(sp_device) * aOldLength);
        sp_device_list_set_device(aThis, aOldLength, aAddress, aType);
    }
}

sp_device_list *sp_device_list_constructor() {
    sp_device_list *obj = (sp_device_list *)sp_zalloc(sizeof(sp_device_list));
    if (obj) {
        init_sp_disposable(&obj->m_Disposer, obj, sp_device_list_destructor);
        obj->add_device = &sp_device_list_add_device;
        obj->m_Devices = 0;
        obj->m_Length = 0;
    }
    return obj;
}

//Return count of devices
//Return pointer to device list
//For free device list call sp_close_device_list
int sp_get_device_list(sp_device_list **aList) {
    if (!aList) return -1;
    *aList = NEW(sp_device_list);
    if (!*aList) return -1;
    return (*aList)->m_Length;
}

void sp_add_device_to_list(sp_device_list *aList, const char *aAddress, int aType) {
    if (aList) {
        aList->add_device(aList, aAddress, aType);
    }
}

int sp_get_device_list_length(sp_device_list *aList) {
    if (aList) return aList->m_Length;
    return 0;
}

//return device on specific position
//return null if error
sp_device* sp_get_device_from_list(sp_device_list *aList, int aPosition) {
    if (!aList || aList->m_Length <= aPosition || aPosition < 0)
        return 0;
    return &aList->m_Devices[aPosition].m_Device;
}

//close device list
void sp_close_device_list(sp_device_list *aList) {
    if (aList) {
        DELETE(aList);
    }
}

//////////////////////////////////////////////////////////////////////////////////

sp_device_data* sp_get_device_last_data(sp_device *aDevice) {
    if (aDevice) return ((sp_device_i*)aDevice)->get_data((sp_device_i*)aDevice);
    return 0;
}

void sp_device_add_data(sp_device *aDevice, int aDate, const char *aState) {
    if (aDevice) {
        ((sp_device_i *)aDevice)->add_data(((sp_device_i *)aDevice), aDate, aState);
    }
}
