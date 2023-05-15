import pyiohook as iohook
import time
import ctypes

class uiohook_event(ctypes.Structure):
    _fields_ = [("type", ctypes.c_int),
                ("time", ctypes.c_uint64),
                ("mask", ctypes.c_uint16),
                ("reserved", ctypes.c_uint16),
                ("data", ctypes.c_void_p)]

def py_callback(event):
    print(f"Received event of type {event.type}")
    print(f"Received event of type {event}")

# c_callback = callback_t(py_callback)

iohook.start(py_callback)
time.sleep(10)