import paho.mqtt.client as mqtt
from time import sleep
import json
from Mqtt.application.models.callbacks import on_connect as ocm, on_message as oms, on_subscribe as osub, on_disconnect as ods
import logging

class MqttClientConnection:
    '''
    Classe de conexão do mqtt

    atributos

    broker          = ip do broker
    port            = porta do broker
    client_name     = nome do client
    user_name       = nome de usuário para a senha
    password        = senha para o client
    keepalive       = tempo para verificar o estado do broker (ativo ou não)
    '''
    def __init__(self, broker_ip:str, port:int, client_name:str, user_name:str, password:str ,keepalive:int):
        
        self.broker_ip      =       broker_ip
        self.port           =       port
        self.client_name    =       client_name
        self.user_name      =       user_name
        self.password       =       password
        self.keepalive      =       keepalive
        self.mqtt_client    =       None

    def start_connection(self) -> bool:
        '''
        Função para conectar o client ao broker, definir as funções de callback
        '''

        try:
            
            mqtt_client = mqtt.Client(
                client_id=self.client_name, protocol=mqtt.MQTTv5,
                callback_api_version=mqtt.CallbackAPIVersion.VERSION2
            )
            
            if self.user_name and self.password:
                mqtt_client.username_pw_set(self.user_name, self.password)
                
            mqtt_client.on_connect      = ocm
            mqtt_client.on_message      = oms
            mqtt_client.on_subscribe    = osub
            mqtt_client.on_disconnect   = ods

            mqtt_client.connect_async(host=self.broker_ip, port=self.port, keepalive=self.keepalive)
            self.mqtt_client = mqtt_client
            self.mqtt_client.loop_start()
            
        except Exception as e:
            logging.exception(e)
            return False
        
    def start(self):
            
        '''
        Função com loop infinito para manter o client ativo
        '''
        self.start_connection()
        while True: sleep(0.001)

    '''
    def publish_on_topic(self,topic, msg):
    
        payload = {
            'id': self.client_name,
            'estado': 'oscioso',
            'mensagem': msg,
            'room_number': 00,
            'local': 'servidor',
            'comando': None
        }

        self.mqtt_client.publish(topic, json.dumps(payload))

    '''
    '''
        Função de pub em tópicos
    '''