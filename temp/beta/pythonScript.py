import serial 
import MySQLdb
import time

dbConn = MySQLdb.connect("localhost","root","","beta1") or die("could not connect to database")

cursor = dbConn.cursor()

device = 'COM4'
try:
    print ("Trying ....")
    arduino = serial.Serial(device,115200)
except:
    print ("Failed to connect on")

while(1):
    try :
        time.sleep(1)
        data= arduino.readline()
        print (data)
        pieces= str(data).split(" ")
        print(pieces)
        try:
            print("inserting data in dbms")
            cursor.execute("INSERT INTO basic1 (bpm,spo2) values(%s,%s)",(pieces[0][2:],pieces[1][:-5]))
            dbConn.commit()
            # cursor.close()
        except MySQLdb.IntegrityError:
            print ("fail to insert data")
    except:
        print("failed to get data from arduino")
    
    
    
cursor.close()   
    
    
    
    
    