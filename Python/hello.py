import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="",
  database="ksiegarnia"
)

print(mydb)
print("\n")

print("Show databases:")
mycursor = mydb.cursor()
mycursor.execute("SHOW DATABASES")
for x in mycursor:
  print(x)
print("\n")
print("Conected to microclimate")
print("\n")

mycursor.execute("SHOW TABLES")
print("Show tables:")
for x in mycursor:
  print(x)
print("\n")

print("Select idksiazki where imieautora = Łukasz")
sql = "SELECT idksiazki FROM ksiazki WHERE imieautora = 'Łukasz'"
mycursor.execute(sql)
myresult = mycursor.fetchone()
print(myresult)
print("\n")

print("Insert into zamowienia with previous id")
idksiazki = myresult[0]
idklienta = 2
sql_insert_query = """ INSERT INTO `zamowienia` (`idklienta`, `idksiazki`) VALUES (%s,%s)"""
insert_tuple = (idklienta, idksiazki)
mycursor.execute(sql_insert_query, insert_tuple)
mydb.commit()
print(mycursor.rowcount, "record inserted.")
print("\n")