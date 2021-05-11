#!/usr/bin/python
# Dare i permessi di esecuzione al file
#
# Invio di dati fittizi con mqtt

import paho.mqtt.client as mqtt
import time
import json
from collections import OrderedDict


broker = "broker"
port = 1883
pausa =  1800  #in secondi

mqttc = mqtt.Client("python_pub")
mqttc.username_pw_set("user", password="pass")
mqttc.connect(broker, port)
mqttc.publish("control/test/100", "Connected!")
mqttc.disconnect()

temp = 20
humy = 50
lat = 40.00000
lon = 9.00000
state = 0
dir = True

pos = OrderedDict([("lat", lat), ("lon",lon)])

print pos

print json.dumps({'ID': 100, 'temp': temp, 'humy': humy, 'pos': {'lat': lat, 'lon':lon}, 'state': 0}, separators=(',',':'))


while True:
   feed = json.dumps(OrderedDict([("ID",100), ("temp",round(temp,2)), ("humy", round(humy,2)), ("pos", pos), ("state", state)]))
   print feed
   mqttc.connect(broker, port)
   print feed
   mqttc.connect(broker, port)
   mqttc.publish("sensors/test/100", feed )
   mqttc.disconnect()

   time.sleep(pausa)
   #ciclo valori
   if dir:
      temp=temp + 0.4
      humy=humy + 1
   else:
      temp=temp - 0.5
      humy=humy - 1.4
   if temp > 32:
      dir=False
   if temp < 9:
      dir=True
   if humy < 20:
      dir=True
   if humy > 90:
      dir=False
