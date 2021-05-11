import paho.mqtt.client as mqtt

mqttc = mqtt.Client("python_pub")
mqttc.username_pw_set("user", password="pass")
mqttc.connect("broker", 1883)
mqttc.publish("test", "Hello, World!")
mqttc.loop(2)

