import json
import os
from dotenv import load_dotenv
from application.models.MongoDBConnection import MongoDBConnection

load_dotenv()

topic = os.getenv('TOPIC')
uri = os.getenv('DB_URI') 
database = os.getenv('MONGO_DB_DATABASE')
collection = os.getenv('MONGO_DB_COLLECTION')


mongo = MongoDBConnection(uri, database, collection)
'''
Callback usado ao conectar o client a algum tópico do broker
'''
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        print('Conexão mal sucedida, erro:', rc, '\n Tentando novamente...')

    client.subscribe(topic)


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
    # ['dispositivos', 'enfermagem/posto', 'id_dispositivo', 'status/comando']
    partes = message.topic.split('/')
    print(
        f'cabeçalho: {partes[0]}; local: {partes[1]}; id dispositivo: {partes[2]}')
    _, local, dispositivo_id = partes

    comando = payload.get('comando')
    mensagem = payload.get('mensagem')
    source = payload.get('source')

    if local == 'posto_enfermagem':
        print(f'Mensagem recebida do dispositivo: {dispositivo_id} da sala: {source}, com comando de: {comando}')


        if comando == 'ligar':
            '''
            Laço condicional para registrar chamada no banco de dados
            '''
            mongo.start_connection()

            mongo.insert_document_collection('Enfermagem',source)

            mongo.close_connection()

    if local == 'enfermagem':
        print(f'Mensagem enviada para: {dispositivo_id} para a sala: {source}, com o comando de: {comando}')

    print()

'''
Callback para conferir se a inscrição em algum tópico foi bem sucedida
'''
def on_subscribe(client, userdata, mid, granted_os):
    print('Inscrito com sucesso!')
