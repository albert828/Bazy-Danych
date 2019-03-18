import serial
import serial.tools.list_ports as port_list

ports = list(port_list.comports())
for p in ports: 
    print(p)

ser = serial.Serial('com4', 9600)
arduinoData = str(ser.readline())
print(arduinoData[2:][:-5])
print("\n")