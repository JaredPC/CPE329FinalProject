# file: rfcomm-client.py
# auth: Albert Huang <albert@csail.mit.edu>
# 
# desc: simple demonstration of a client application that uses RFCOMM sockets
#       intended for use with rfcomm-server
#
# Modified for use and added keylogger to send keypresses to BLE device
# by Jared Peter-Contesse jpeterco@calpoly.edu

# Bluetooth Serial Libraries
from bluetooth import *
import sys
import bluetooth

# Key-Logger libraries
import win32api
import win32console
import win32gui
import pythoncom, pyHook

def restart_line():
    sys.stdout.write('\r')
    sys.stdout.flush()

def OnKeyboardEvent(event):
    keypress = event.KeyID
    
    if keypress == 38: #UP
        restart_line()
        print("UP    ", end='', flush=True)
        sock.send(bytes([1]))
    elif keypress == 40: #DOWN
        restart_line()
        print("DOWN  ", end='', flush=True)
        sock.send(bytes([2]))
    elif keypress == 39: #RIGHT
        restart_line()
        print("RIGHT ", end='', flush=True)
        sock.send(bytes([3]))
    elif keypress == 37: #LEFT
        restart_line()
        print("LEFT  ", end='', flush=True)
        sock.send(bytes([4]))
    elif keypress == 32: #SPACE
        restart_line()
        print("STOP  ", end='', flush=True)
        sock.send(bytes([5]))
    elif keypress == 81: #Quit
        sock.close()
        exit()
    return True

if sys.version < '3':
    input = raw_input

addr = None

if len(sys.argv) < 2:
    print("no device specified.  Searching bluetooth devices")
    nearby_devices = bluetooth.discover_devices(lookup_names=True)
    print("found %d devices" % len(nearby_devices))

    for addresses, names in nearby_devices:
        print("  %s - %s" % (addresses, names))
    sys.exit(0)
else:
    addr = sys.argv[1]
    print("Searching for Connection with %s" % addr)

# UUID of serial port Bluetooth device
uuid = "00001101-0000-1000-8000-00805f9b34fb"
service_matches = find_service( uuid = uuid, address = addr )

if len(service_matches) == 0:
    print("couldn't find the device =(")
    sys.exit(0)

first_match = service_matches[0]
port = first_match["port"]
name = first_match["name"]
host = first_match["host"]

print("connecting to \"%s\" on %s" % (port, host))

# Create the client socket
sock=BluetoothSocket( RFCOMM )
sock.connect((host, port))

print("Connected")
print("Press Arrow Keys to Move, SPACE to stop, Q to Quit\n")
print("Current:\n")

# create a hook manager object
hm = pyHook.HookManager()
hm.KeyDown = OnKeyboardEvent
# set the hook
hm.HookKeyboard()
# wait forever 
pythoncom.PumpMessages()

sock.close()