#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!sqliteprotocol.h!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!end sqliteprotocol.h!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


typedef struct sp_disposable sp_disposable;
struct sp_disposable {
    void *m_This;
    void (*dispose)(sp_disposable*);
};

void sp_disposable_init(sp_disposable *aObj, void *aThis, void (*aDispose)(sp_disposable*)) {
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
#define DELETE(obj) \
if (obj) { \
    obj->m_Disposer.dispose(((sp_disposable*)&obj->m_Disposer)); \
    sp_free(obj); \
}

//////////////////DB////////////////////////////////////////////////////////
typedef struct sp_db sp_db;
struct sp_db {
    sp_disposable m_Disposer;
    sqlite3 *m_DB;
    int (*connect)(sp_db *aThis);
    void (*disconnect)(sp_db *aThis);
};

void sp_db_destructor(sp_disposable *aThis) {
    if (aThis && aThis->m_This) {
        sp_db *th = (sp_db *)aThis->m_This;
        if (th->m_DB) {
            th->disconnect(th);
        }
    }
}

int sp_db_connect(sp_db *aThis) {
    if (aThis && !aThis->m_DB) {
        if (sqlite3_open("../test.sqlite", &aThis->m_DB) == SQLITE_OK) {
            if ((SQLITE_OK != sqlite3_exec(aThis->m_DB,
                  "CREATE TABLE IF NOT EXISTS \
                   devices (id INTEGER PRIMARY KEY AUTOINCREMENT, \
                   address CHAR(100) UNIQUE NOT NULL, type INTEGER NOT NULL)",
                   0, 0, 0)) ||
                (SQLITE_OK != sqlite3_exec(aThis->m_DB,
                  "CREATE TABLE IF NOT EXISTS \
                   device_data (id INTEGER PRIMARY KEY AUTOINCREMENT, \
                   did INTEGER NOT NULL, \
                   date INTEGER NOT NULL, state CHAR(255) NOT NULL)",
                   0, 0, 0))) {
                return 0;
            }
            return 1;
        }
        aThis->m_DB = 0;
    }
    return 0;
}

void sp_db_disconnect(sp_db *aThis) {
    if (aThis && aThis->m_DB) {
        sqlite3_close(aThis->m_DB);
        aThis->m_DB = 0;
    }
}

sp_db* sp_db_constructor() {
    sp_db *temp = (sp_db*)sp_zalloc(sizeof(sp_db));
    if (temp) {
        sp_disposable_init(&temp->m_Disposer, temp, &sp_db_destructor);
        temp->connect = &sp_db_connect;
        temp->disconnect = &sp_db_disconnect;
        temp->m_DB = 0;
        if (!temp->connect(temp)) {
            DELETE(temp);
            temp = 0;
        }
        return temp;
    }
    return 0;
}
//////////////////DB////////////////////////////////////////////////////////

//////////////////SP_DEVICE_I/////////////////////////////////////////////////
typedef struct sp_device_i sp_device_i;
struct sp_device_i {
    sp_device m_Device;
    sp_disposable m_Disposer;
    void (*set_address)(sp_device_i *aThis, const char *aAddress);
    void (*free_address)(sp_device_i *aThis);
    int m_ID;
};

void sp_device_i_destructor(sp_disposable *aThis) {
  if (aThis && aThis->m_This) {
    sp_device_i *th = (sp_device_i *)aThis->m_This;
    th->free_address(th);
  }
}

void sp_device_i_set_address(sp_device_i *aThis, const char *aAddress) {
  if (aThis) {
    aThis->free_address(aThis);
    aThis->m_Device.m_Address = (char *)sp_zalloc(strlen(aAddress) + 1);
    strcpy(aThis->m_Device.m_Address, aAddress);
  }
}

void sp_device_i_free_address(sp_device_i *aThis) {
  if (aThis && aThis->m_Device.m_Address) {
    sp_free(aThis->m_Device.m_Address);
  }
}

sp_device_i * sp_device_i_constructor() {
  sp_device_i *temp = (sp_device_i *)sp_zalloc(sizeof(sp_device_i));
  if (temp) {
    sp_disposable_init(&temp->m_Disposer, temp, &sp_device_i_destructor);
    temp->set_address = &sp_device_i_set_address;
    temp->free_address = &sp_device_i_free_address;
    temp->m_Device.m_Address = 0;
    temp->m_ID = 0;
    temp->m_Device.m_Type = SP_DEVICE_TYPE_UNKNOWN;
  }
  return temp;
}
//////////////////SP_DEVICE_I/////////////////////////////////////////////////

//////////////////SP_DEVICE_DATA_I////////////////////////////////////////////
typedef struct sp_device_data_i sp_device_data_i;
struct sp_device_data_i {
    sp_device_data m_Data;
    sp_disposable m_Disposer;
    void (*set_state)(sp_device_data_i *aThis, const char *aState);
    void (*free_state)(sp_device_data_i *aThis);
};

void sp_device_data_i_destructor(sp_disposable *aThis) {
  if (aThis && aThis->m_This) {
    sp_device_data_i *th = (sp_device_data_i *)aThis->m_This;
    th->free_state(th);
  }
}

void sp_device_data_i_set_state(sp_device_data_i *aThis, const char *aState) {
  if (aThis) {
    aThis->free_state(aThis);
    aThis->m_Data.m_State = (char *)sp_zalloc(strlen(aState) + 1);
    strcpy(aThis->m_Data.m_State, aState);
  }
}

void sp_device_data_i_free_state(sp_device_data_i *aThis) {
  if (aThis && aThis->m_Data.m_State) {
    sp_free(aThis->m_Data.m_State);
  }
}

sp_device_data_i * sp_device_data_i_constructor() {
  sp_device_data_i *temp = (sp_device_data_i *)sp_zalloc(sizeof(sp_device_data_i));
  if (temp) {
    sp_disposable_init(&temp->m_Disposer, temp, &sp_device_data_i_destructor);
    temp->set_state = &sp_device_data_i_set_state;
    temp->free_state = &sp_device_data_i_free_state;
    temp->m_Data.m_State = 0;
    temp->m_Data.m_Date = 0;
  }
  return temp;
}
//////////////////SP_DEVICE_DATA_I///////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
void sp_devices_clear() {
    sp_db *th = NEW(sp_db);
    if (th && th->m_DB) {
        if (SQLITE_OK == sqlite3_exec(th->m_DB, "DELETE FROM device_data", 0, 0, 0)) {
          sqlite3_exec(th->m_DB, "DELETE FROM devices", 0, 0, 0);
        }
    }
    DELETE(th);
}

int sp_devices_length_callback(void *aLength, int aCount, char** aValues, char** aColumns) {
    if (aLength && 1 == aCount) {
        *((int *)aLength) = atoi(aValues[0]);
    }
    return SQLITE_OK;
}

int sp_devices_length() {
    int length = 0;
    sp_db *th = NEW(sp_db);
    if (th && th->m_DB &&
        SQLITE_OK != sqlite3_exec(th->m_DB,
                                  "SELECT COUNT(id) as length FROM devices",
                                  &sp_devices_length_callback,
                                  (void *)&length, 0)) {
        length = 0;
    }
    DELETE(th);
    return length;
}

typedef struct sp_enumerate_data sp_enumerate_data;
struct sp_enumerate_data {
  int (*m_Callback)(void *aUserData, sp_device *aDevice);
  void *m_UserData;
};

int sp_devices_enumerate_callback(void *aData, int aCount, char** aValues, char** aColumns) {
  sp_enumerate_data *data = (sp_enumerate_data*)aData;
  if (data && data->m_Callback && 3 == aCount) {
    sp_device_i *device = NEW(sp_device_i);
    if (device) {
        int ret = 0;
        device->m_ID = atoi(aValues[0]);
        device->set_address(device, aValues[1]);
        device->m_Device.m_Type = atoi(aValues[2]);
        ret = data->m_Callback(data->m_UserData, (sp_device *)device);
        if (!ret) {
            return SQLITE_OK;
        }
    }
  }
  return SQLITE_ABORT;
}

void sp_devices_enumerate(int (*aCallback)(void *aUserData, sp_device *aDevice), void *aUserData) {
    sp_db *th = NEW(sp_db);
    if (aCallback && th) {
        sp_enumerate_data data;
        data.m_Callback = aCallback;
        data.m_UserData = aUserData;
        sqlite3_exec(th->m_DB, "SELECT id, address, type FROM devices",
                     &sp_devices_enumerate_callback,
                     (void *)&data, 0);
    }
    DELETE(th);
}

sp_device* sp_device_add(const char *aAddress, int aType) {
    sp_device *device = 0;
    sp_db *th = NEW(sp_db);
    if (aAddress && th) {
        sqlite3_stmt *stmt = 0;
        if (SQLITE_OK == sqlite3_prepare_v2(th->m_DB, "INSERT INTO devices(address, type) VALUES(?, ?)", -1, &stmt, 0)) {
            if (SQLITE_OK == sqlite3_bind_text(stmt, 1, aAddress, -1, 0)) {
                if (SQLITE_OK == sqlite3_bind_int(stmt, 2, aType)) {
                    int ret = sqlite3_step(stmt);
                    if (SQLITE_OK == ret || SQLITE_DONE == ret) {
                        sp_device_i *device_i = NEW(sp_device_i);
                        device_i->set_address(device_i, aAddress);
                        device_i->m_Device.m_Type = aType;
                        device_i->m_ID = (int)sqlite3_last_insert_rowid(th->m_DB);
                        device = (sp_device *)device_i;
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
    }
    DELETE(th);
    return device;
}

void sp_device_free(sp_device *aDevice) {
  DELETE(((sp_device_i *)aDevice));
}

void sp_device_add_data(sp_device *aDevice, int aDate, const char *aState) {
    sp_db *th = NEW(sp_db);
    if (aDevice && aState && aDate > 0 && th) {
        sp_device_i *device = (sp_device_i *)aDevice;
        sqlite3_stmt *stmt = 0;
        if (SQLITE_OK == sqlite3_prepare_v2(th->m_DB, "INSERT INTO device_data(did, date, state) VALUES(?, ?, ?)", -1, &stmt, 0)) {
            if (SQLITE_OK == sqlite3_bind_int(stmt, 1, device->m_ID)) {
                if (SQLITE_OK == sqlite3_bind_int(stmt, 2, aDate)) {
                    if (SQLITE_OK == sqlite3_bind_text(stmt, 3, aState, -1, 0)) {
                        sqlite3_step(stmt);
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
    }
    DELETE(th);
}

sp_device_data * sp_device_get_last_data(sp_device *aDevice) {
    sp_device_data *data = 0;
    sp_db *th = NEW(sp_db);
    if (aDevice && th) {//
        sp_device_i *device = (sp_device_i *)aDevice;
        sqlite3_stmt *stmt = 0;
        if (SQLITE_OK == sqlite3_prepare_v2(th->m_DB, "SELECT date, state FROM device_data WHERE did = ? ORDER BY date DESC LIMIT 1", -1, &stmt, 0)) {
            if (SQLITE_OK == sqlite3_bind_int(stmt, 1, device->m_ID)) {
                if (SQLITE_ROW == sqlite3_step(stmt)) {
                  sp_device_data_i *data_i = NEW(sp_device_data_i);
                  if (data_i) {
                    data_i->m_Data.m_Date = sqlite3_column_int(stmt, 0);
                    data_i->set_state(data_i, (const char *)sqlite3_column_text(stmt, 1));
                    data = (sp_device_data *)data_i;
                  }
                }
            }
            sqlite3_finalize(stmt);
        }
    }
    DELETE(th);
    return data;
}

void sp_device_data_free(sp_device_data *aData) {
  DELETE(((sp_device_data_i *)aData));
}
////////////////////////////////////////////////////////////////////////////
