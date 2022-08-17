import serial 
import MySQLdb
import time

dbConn = MySQLdb.connect("localhost","root","","django_db_v1") or die("could not connect to database")

cursor = dbConn.cursor()

device = 'COM4'
state_dic={
    '1' :  'Haryana',
    '2' : 'Delhi',
    '3' : 'Rajasthan',
    '4' : 'Himachal Pradesh',
    '5' : 'Uttarakhand',
    '6' : 'Punjab',
    '7' : 'Tamil Nadu',
    '8' : 'Karnataka',
}
print("state id's")
for i in state_dic:
    print(state_dic[i],i)

print("---------------------------------")
try:
    print ("Trying ....")
    arduino = serial.Serial(device,115200)
except:
    print ("Failed to connect on")

# while(1):
#     try :
#         time.sleep(1)
#         arduino = serial.Serial(device,115200)
#         data= arduino.readline()
#         print (data)
#         pieces= str(data).split(" ")
#         print(pieces)
#         print("Enter the name and state id the patient")

#         name =input()
#         state_id=int(input())
#         try:
#             print("inserting data in dbms")
#             cursor.execute("INSERT INTO chartapp_patient (name,bpm,spo2,state_id) values(%s,%s,%s,%s)",(name,pieces[0][2:],pieces[1][:-5]),state)
#             dbConn.commit()
#             # cursor.close()
#         except MySQLdb.IntegrityError:
#             print ("fail to insert data")
#     except:
#         print("failed to get data from arduino")


try :
    time.sleep(1)
    # arduino = serial.Serial(device,115200)
    data= arduino.readline()
    print (data)
    pieces= str(data).split(" ")
    print(pieces)
    print("Enter the name and state id the patient")

    name =input()
    state_id=input()
    # idd=None
    try:
        print("inserting data in dbms")
        cursor.execute("INSERT INTO chartapp_patient (name,bpm,spo2,state_id) values(%s,%s,%s,%s)", (name,pieces[0][2:],pieces[1][:-5],state_id) )
        # insert_stmt = (
        #     "INSERT INTO chartapp_patient (emp_no, first_name, last_name, hire_date) "
        #     "VALUES (%s, %s, %s, %s)"
        # )
        dbConn.commit()
        cursor.close()
    except MySQLdb.IntegrityError:
        print ("fail to insert data")
except:
    print("failed to get data from arduino")
    
    
    
# cursor.close()   
    
    
    
    
    