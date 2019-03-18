import serial
import mysql.connector
from datetime import date, datetime, timedelta
#import serial.tools.list_ports as port_list

#ports = list(port_list.comports())
#for p in ports: 
#    print(p)
dbName = "microclimate"
mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="",
  database=dbName
)

print("Conected to: ", dbName)
print("\n")

print("Show databases:")
mycursor = mydb.cursor()
mycursor.execute("SHOW DATABASES")
for x in mycursor:
  print(x)
print("\n")

mycursor.execute("SHOW TABLES")
print("Show tables:")
for x in mycursor:
  print(x)
print("\n")

ser = serial.Serial('com4', 115200)
while True:
    arduinoData = str(ser.readline())
    splitedData = (arduinoData[2:][:-5]).split()
    roomName = splitedData[0]
    sensorName = splitedData[1]
    sensorValue = int(splitedData[2])
    #print(sensorName, sensorValue, roomName)
    #print("\n")

    query = ("SELECT sensor_id FROM sensors "
            "WHERE name = %s")
    mycursor.execute(query, (sensorName,))
    sensorId = mycursor.fetchone()
    sensorId = sensorId[0]
    #print("Sensor id:",sensorId)
    #print("\n")

    query = ("SELECT room_id FROM rooms "
            "WHERE name = %s")
    mycursor.execute(query, (roomName,))
    roomId = mycursor.fetchone()
    roomId = roomId[0]

    actualDate = datetime.now().date()
    actualHour = datetime.now().time()
    formatedHour = actualHour.strftime("%H:%M:%S")
    #print("Aktualna data:", actualDate, formatedHour)
    #print("\n")

    #print("Insert", sensorName, "ID =", sensorId, "Value =", sensorValue, "czas:",actualDate, formatedHour, "pokoj", roomName, "Id pokoju", roomId)
    sql_insert_query = ("INSERT INTO `measurement` (`sensor_id`, `value`,`room_id`, `date`, `hour`) VALUES (%s,%s,%s,%s,%s)")
    insert_tuple = (sensorId, sensorValue, roomId, actualDate, formatedHour)
    mycursor.execute(sql_insert_query, insert_tuple)
    mydb.commit()
    #print(mycursor.rowcount, "record inserted.")
    #print("\n")
