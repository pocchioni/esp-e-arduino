#!/usr/bin/python
import Adafruit_DHT as dht
import paho.mqtt.client as mqtt
import time
import json
from collections import OrderedDict

topic="sensors/test/1"
broker="broker"
port=1883

mqttc = mqtt.Client("python_pub")
mqttc.username_pw_set("user", password="pass")
mqttc.connect(broker, port)
mqttc.publish("control/test/1", "started")
mqttc.disconnect()
print "Dht11 started!"

# tempo letture dht (in secondi)
tread = 60
# numero di letture dopo le quali inviare
nsend = 10
#array di letture
t = []
u = []
for i in range(0,nsend):
    t.append(0)
    u.append(0)	

while True:
    for i in range(0,nsend):
        u[i],t[i] = dht.read_retry(dht.DHT11, 4)
	print '{0:d} Temp={1:0.1f}*C  Humidity={2:0.1f}%'.format(i, t[i], u[i])
        time.sleep(tread)
    lat = 39.240951
    lon = 9.098669
    pos = OrderedDict([("lat", lat), ("lon",lon)])
    
    id = 1
    temp = round(sum(t)/len(t),2) 
    humy = round(sum(u)/len(u),2)
    state = 0
    result = json.dumps(OrderedDict([("ID", id), ("temp", temp), ("humy", humy), ("pos", pos), ("state", state)]))
    print result
    mqttc.connect(broker, port)    
    mqttc.publish(topic, result)
    mqttc.disconnect()
