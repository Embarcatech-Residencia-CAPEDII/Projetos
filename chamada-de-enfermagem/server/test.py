from flask import Flask, request, jsonify
from flask_mqtt import Mqtt
import os
from dotenv import load_dotenv
from mongo.MongoDBConnection import MongoDBConnection


load_dotenv()

app = Flask(__name__)
'''
Configurações para o Client
'''
app.config['MQTT_BROKER_URL']       =       'localhost' #url do broker/endereço ip
app.config['MQTT_BROKER_PORT']      =       1883
app.config['MQTT_USERNAME']         =       'Client_Flask' 
app.config['MQTT_PASSWORD']         =       ''
app.config['MQTT_KEEPALIVE']        =       10
app.config['MQTT_TLS_ENABLE']       =       False

topic = 'dispositivos/+/+'                          #topico para teste

'''
Configuração para o Mongo DB
'''
mongoDBConnection = MongoDBConnection()

emergencias = list()

mqtt = Mqtt(app)

'''
Conexão em um tópico [inscrição]
'''
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):

    if(rc == 0):
        print("Conectado ao broker MQTT Local com código:", rc)
        mqtt.subscribe(topic)
        print("Inscrito no tópico:", topic)

    else:
        print(f"Erro na conexão {rc}")

'''
Lidando com mensagens [inscrição]
'''
@mqtt.on_message()
def handle_mqtt_message(client, userdata,message):
    payload = message.payload.decode()
    print(f'Mensagem recebida no tópico {message.topic}: {payload}')

    partes = message.topic.split('/')
    dispositivo_id = partes[1]
    local = partes[2]

    print(f'ID: {dispositivo_id}; Sala: {local}; Mensagem:{payload}')
    emergencias.append(local) 

'''
Publicando uma mensagem no tópico 
'''
@app.route('/publicar', methods=['POST'])
def publish_message():
    data = request.json
    topic = data.get('topic')
    message = data.get('message')
    mqtt.publish(topic, message)
    return {'status', 'mensagem recebida'}

'''
Desiscrevendo de um tópico
'''
def unsubscribe_topic(topic):
    mqtt.unsubscribe(topic)


'''
Conexão com o MongoDB
'''


'''
Rotas/tópicos para cada microcontrolador
'''
@app.route("/")
def hello_world():
    dados = mongoDBConnection.read_data()
    return jsonify(dados)

'''
Rota/tópico para o microcontrolador do Posto de Enfermagem
'''

'''
Rota para página de login
'''

'''
Rota para página de relatório
'''

'''
Rota para salvar o chamado no BD
'''


if __name__ == '__main__':
    
    app.run(host='0.0.0.0', port=5000, debug=True)