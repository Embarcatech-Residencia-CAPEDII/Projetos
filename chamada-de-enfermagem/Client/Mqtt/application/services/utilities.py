import requests
import logging
from dotenv import load_dotenv
import os
from MongoDB.mongo_conn import mongo_conn 
from datetime import datetime

load_dotenv()

def check_if_device_exists(device:str) -> bool:
    '''
    Verifica se existe um device na database
    '''

    logging.info(f'Verificação de dispositivo: {device}')

    try:

        result = mongo_conn.check_if_document_exists('devices', 'device', device)

        if result:
            logging.info(f'Device [{device}] no banco de dados')
            return True

        logging.info(f'Device [{device}] não registrado no banco de dados')

    except Exception as e:
        logging.exception(e)

    return False

def register_chamada_mongodb(payload:dict):
    '''
    Registra uma nova chamada na database, realiza o tratamento do payload
    '''

    logging.info(f'Registro de payload de chamada')

    dispositivo_id = payload.get('id')
    mensagem = payload.get('mensagem')
    room_number = payload.get('room_number')
    local_emergencia = payload.get('local')

    document_to_save = {
        'dispositivo_id': dispositivo_id,
        'local': local_emergencia,
        'sala': room_number,
        'data': datetime.now()
    }

    logging.info(f'Mensagem do dispositivo {dispositivo_id}:{mensagem}')

    try:
        result = mongo_conn.insert_document_collection('chamadas', document_to_save)

        if result:
            logging.info('Chamada registrada no banco de dados')

    except Exception as e:
        logging.exception(e)

def register_status_device_mongodb(device:str, payload:dict):
    '''
    Registra o status de um device na database
    '''

    if 'room_number' in payload:
        return
    
    logging.info(f'Verificação de confirmação de recebimento da mensagem do dispositivo: {device}')
     
    document_to_save = {
        'device': device,
        'status': payload.get('status'),
        'updateAt': datetime.now()
    }
    try:

        result = mongo_conn.return_document('status_device', 'device', device)

        if not result:    
            logging.warning(f'Status de device [{device}] não encontrado na database!')

            logging.info(f'Registrando status do [{device}]')    
            mongo_conn.insert_document_collection('status_device', document_to_save)

            return

        logging.info(f'documento de status do device [{device}] já existe, atualizando')

        id_status_device = str(result['_id'])
        mongo_conn.update_document_by_id('status_device', id_status_device, document_to_save)

    except Exception as e:
        logging.exception(e)

    return True

def register_status_chamada_mongo_db(device:str, payload:dict):
    '''
    Função para registrar/atualizar uma chamada na tabela de status_chamadas
    '''

    
    if not ('room_number' in payload) and ('estado' not in payload):
        return 

    logging.info(f'Payload de registro de status do dispositivo: {device}')

    room_number = payload.get('room_number')
    estado = payload.get('estado')
    
    document = {
        'device': device,
        'room_number': room_number,
        'status': estado,
        'updateAt': datetime.now()
    }
    
    try:
        result = mongo_conn.return_document('status_chamadas', 'device', device)

        print(result)

        if not result:        
            logging.warning(f'Device [{device}] não encontrado no status de chamada!')

            logging.info(f'Registrando status de chamada no mapa de [{device}]')    
            mongo_conn.insert_document_collection('status_chamadas', document)

            return 

        logging.info(f'documento de status de [{device}] já existe, atualizando')

        id_chamada = str(result['_id'])
        mongo_conn.update_document_by_id('status_chamadas', id_chamada, document)

    except Exception as e:
        print(e)



