
from application.controller.mqtt_client_connection import MqttClientConnection

if __name__ == '__main__':
    mqttclient = MqttClientConnection()
    mqttclient.start()
    