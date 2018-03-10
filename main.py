import urllib.parse
import requests
import serial
import time
# User must provide their own api_key and city ID which
# are available (for free!) from openweathermap's website: 
# https://openweathermap.org/api

api_url = 'https://api.openweathermap.org/data/2.5/weather?id='
api_key = '&APPID='

url = api_url + api_key

json_data = requests.get(url).json()

ser = serial.Serial('COM4',9600)

ser.write(bytearray(str.capitalize(json_data['weather'][0]['description'])+"\n",'us-ascii'))

# pySerial sends data too fast, suspending the script for 150ms should do the trick
time.sleep(.15)

ser.write(bytearray("Now: "+str(int(json_data['main']['temp']))+
    " Min: "+str(int(json_data['main']['temp_min']))+
      " Max: "+str(int(json_data['main']['temp_max']))+"\n",'us-ascii'))

ser.close()
