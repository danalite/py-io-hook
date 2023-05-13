import pyiohook as iohook
import time

def callback(event):
    print(event)

iohook = iohook.System()
iohook.register_callback(callback)
iohook.start_event_loop()

time.sleep(10)