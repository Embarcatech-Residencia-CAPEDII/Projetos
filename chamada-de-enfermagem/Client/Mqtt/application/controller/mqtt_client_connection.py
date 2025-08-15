import paho.mqtt.client as mqtt
from dotenv import load_dotenv
import os
from time import sleep
import json
from application.models.callbacks import on_connect as ocm, on_message as oms, on_subscribe as osub

load_dotenv()
broker = os.getenv('BROKER_MQTT')
port = int(os.getenv('PORT_MQTT'))
topic = os.getenv('TOPIC')

print(broker, port, topic)

'''
Classe de conexão do mqtt

atributos

broker = ip do broker
port = porta do broker
client_name = nome do client
keepalive = tempo para verificar o estado do broker (ativo ou não)
'''

class MqttClientConnection:
    def __init__(self, broker=broker, port=port, client_name= "Notebook", keepalive=60):
        self.broker_ip = broker
        self.port = port
        self.client_name = client_name
        self.keepalive = keepalive
        self.mqtt_client = None

    '''
    Função para conectar o client ao broker, definir as funções de callback
    '''
    def start_connection(self):
        mqtt_client = mqtt.Client(self.client_name)

        mqtt_client.on_connect = ocm
        mqtt_client.on_message = oms
        mqtt_client.on_subscribe = osub

        mqtt_client.connect(host=self.broker_ip, port=self.port, keepalive=self.keepalive)
        self.mqtt_client = mqtt_client
        self.mqtt_client.loop_start()
    
    '''
    Função com loop infinito para manter o client ativo
    '''
    def start(self):
        self.start_connection()

        while True: sleep(0.001)

    '''
    Função para publicar em algum tópico 
    '''
    def publish_on_topic(self, local, dispositivo_id, comando, mensagem, source=None):

        topic = f"dispositivos/{local}/{dispositivo_id}"
        
        payload = {
            "id": dispositivo_id,
            "comando": comando,
            "mensagem": mensagem,
            "source": source,
        }

        self.mqtt_client.publish(topic, json.dumps(payload))