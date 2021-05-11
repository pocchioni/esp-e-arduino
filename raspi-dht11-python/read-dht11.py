import Adafruit_DHT as dht

while True:
   h,t = dht.read_retry(dht.DHT11, 4)
   print 'Temp={0:0.1f}*C  Humidity={1:0.1f}%'.format(t, h)	
     
