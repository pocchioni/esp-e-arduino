DHT11 su Raspberry pi

Collegamento del dht11:
3.3v -> dht11 +
gpio4 -> dht11 data
gnd -> dht11 -

Una resistenza da 4.7K oppure 10k tra + e data.

Installare la libreria Adafruit_Python_DHT scompattando il file zip in una cartella ed eseguendo:
sudo apt-get install build-essential python-dev (se necessario)
python setup.py build 
sudo python setup.py install

testare il dht11 con il comando (dalla cartella examples)
sudo python AdafruitDHT.py 11 <porta GPIO>
11 sta per il tipo di sensore (DHT11) e va specificato il numero della porta GPIO

Installare la libreria mqtt:
sudo apt-get install python-pip (se necessario)
sudo pip install paho-mqtt



Daemonize
Thanks to http://blog.scphillips.com/posts/2013/07/getting-a-python-script-to-run-in-the-background-as-a-service-on-boot/


Usage:
Copy the init script into /etc/init.d using:    sudo cp sensord.sh /etc/init.d/sensord
Make sure the script is executable (chmod again) and make sure that it has UNIX line-endings (dos2unix)
Run the command       sudo update-rc.d sensord defaults
This command adds in symbolic links to the /etc/rc?.d directories so that the init script is run at the default times
Show the links        ls -l /etc/rc?.d/*sensord
Now it is possible to start-stop-status the service     sudo service sensord status
The logs can be viewed with    tail -f /var/log/sensord.log

Riassunto comandi
sudo cp sensord.sh /etc/init.d/sensord
sudo chmod +x /etc/init.d/sensord
sudo update-rc.d sensord defaults
sudo service sensord status (o start)
tail -f /var/log/sensord.log
 
Saluti
