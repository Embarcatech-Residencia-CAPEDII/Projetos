import json
import os
from dotenv import load_dotenv
from application.models.MongoDBConnection import MongoDBConnection

load_dotenv()

topic = os.getenv('BROKER_TOPIC')
uri = os.getenv('DB_URI') 
database = os.getenv('MONGO_DATABASE')

mongo = MongoDBConnection(uri, database)
'''
Callback usado ao conectar o client a algum tópico do broker
'''
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        print('Conexão mal sucedida, erro:', rc, '\n Tentando novamente...')

    client.subscribe(topic)

def on_disconnect(client, userdata, rc):
    if rc != 0:
        print('Desconexão inesperada, tentando novamente...', rc)

    print('Tentando novamente...')
    try:
        client.reconnect()
        print('Reconexão bem sucedida!')

    except Exception as e:
        print('Conexão mal sucedida, erro:', rc)


'''
    Callback para formatar as mensagens recebidas nos tópicos

    Os dispositivos nas salas assim apenas o tópico em que estão,
    dessa forma, dentro do payload podem receber comandos de desligar/ligar ou qualquer outro
    ex: dispositivos/enfermagem/seu_id

    E publicam apenas no tópico de posto_enfermagem
    dessa foram podem enviar seu estado ligado/timeout
    ex:dispositivos/posto_enfermagem/seu_id

    o formato de payload será assim: {
        'id_dispositivo':'id_dispositivo',
        'estado':'emergência/timeout',
        'mensagem':'mensagem para debug',
        'source': 'sala do dispositivo',
        'comando': 'ligar/desligar'
    }
'''


def on_message(client, userdata, message):
    payload = json.loads(message.payload.decode())
    print(f'Mensagem recebida no tópico {message.topic}: \n{payload}')

    # Quebra o tópico em partes
    # ['dispositivos', 'enfermagem/posto', 'id_dispositivo']
    partes = message.topic.split('/')
    print(f'cabeçalho: {partes[0]}; local: {partes[1]}; id dispositivo: {partes[2]}')
    
    _, local_topic, dispositivo_id = partes

    if mongo.start_connection() != True:
        mongo.close_connection()
        
    else:
        if mongo.check_if_document_exists('devices', dispositivo_id) != True:
            print('Dispositivo não encontrado')
            mongo.close_connection() 
        
        else:
            print('Dispositivo logado!')
                
            comando = payload.get('comando')
            mensagem = payload.get('mensagem')
            estado = payload.get('estado')
            local_emergencia = payload.get('local')
            room_number = payload.get('room_number')

            print(comando)
            if local_topic == 'posto_enfermagem':
                print(f'Mensagem recebida do dispositivo: {dispositivo_id} da {local_emergencia}: {room_number}, com comando de: {comando}')

                if comando == 'ligar':
                    '''
                    Laço condicional para registrar chamada no banco de dados
                    '''
                    print(mongo.insert_document_collection('chamadas', dispositivo_id, local_emergencia, room_number))

            if local_topic == 'enfermagem':
                print(f'Mensagem enviada para: {dispositivo_id} para a {local_emergencia}: {room_number}, com o comando de: {comando}')

        mongo.close_connection()
        print()

'''
Callback para conferir se a inscrição em algum tópico foi bem sucedida
'''
def on_subscribe(client, userdata, mid, granted_os):
    print('Inscrito com sucesso!')
