from flask import Blueprint, jsonify, request
from datetime import datetime
from Flask.auth import token_required, admin_required
from MongoDB.mongo_conn import mongo_conn
from MongoDB.MongoDBConnection import MongoDBConnection
from Flask.Services.device_service import DeviceService
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from Flask.Models.device_db_model import DeviceDBModel
from Flask.Models.device_model import Device
from dotenv import load_dotenv
import os
import logging


'''
Arquivo para rotas para retornar os documentos de chamadas
'''

'''
ROTAS

/api/devices 
/api/devices/quantidade
/api/devices/register
/api/devices/delete
/api/devices/delete/<document_id>
'''

load_dotenv()

device_db_model = DeviceDBModel(mongo_conn)
device_service = DeviceService(device_db_model)

devices_bp = Blueprint('devices', __name__, url_prefix='/api/devices')

'''
Rota que retorna uma lista com todos os dispositivos
'''
@devices_bp.route('/', methods=['GET'])
@admin_required
def return_all_devices():
    
    try:
        devices = device_db_model.return_all_devices()

        if devices:
            json_devices = convert_all_id_to_string(devices)
            return jsonify(json_devices), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400

    return {'Error': 'Devices não encontrados'},404

'''
Rota que retorna a quantidade com todos os dispositivos cadastrados
'''
@devices_bp.route('/quantidade', methods=['GET'])
@admin_required
def return_count_devices():

    try:
        count_devices = device_db_model.return_count_all_devices()
        if count_devices:
            return jsonify({'Quantidade': count_devices}), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error'})
    
    return jsonify({'Error': 'Devices não encontrados'}), 404

'''
Rota que registra um novo dispositivo no banco de dados
json = {
    "device":"nome_device"
}

APENAS PARA ADMINS
'''
@devices_bp.route('/register', methods=['POST'])
@admin_required
def register_device():

    data = request.get_json()

    try:
        if not 'device' in data:
            return jsonify({'Erro': 'Campos incorretos'}), 400
        
        device = Device(data['device'])
        if device.isValid() == False:
            return jsonify({'Error': 'Valores incorretos, tente novamente'}), 400
        
        result = device_service.register(device.getDevice(), device.getCreatedAt())

    except Exception as e:
        logging.exception('Error in register', e)
        result = jsonify({'Error':'Falha em tentativa de registro'}), 500

    return result

'''
Rota para deletar um dispositivo com seu id de dispositivo

json = {
    "document_id": "id_do_dispositivo"
}

APENA PARA ADMINS
'''
@devices_bp.route('/delete', methods=['DELETE'])
@admin_required
def delete_device():
    
    try:
        data = request.get_json()

        if not "document_id" in data:
            return jsonify({'Error': 'Campo inválido'}), 400
        
        result = device_service.delete(data['document_id'])

        logging.info(f'{result}')
    
    except Exception as e:
        logging.exception('Error in delete', e)
        result = jsonify({'Error': 'Falha na tentativa de deletar device'}), 500

    return result


'''
Rota para deletar um dispositivo com seu nome de dispositivo por url
APENA PARA ADMINS
'''
@devices_bp.route('/delete/<string:document_id>', methods=['DELETE'])
@admin_required
def delete_device_by_id(document_id):
    
    try:
        if not document_id:
            return jsonify({'Error': 'Campo inválido'}), 400
        
        result = device_service.delete(document_id)
    
    except Exception as e:
        logging.exception('Error in register', e)
        result = jsonify({'Error': 'Falha na tentativa de deletar device'}), 500

    return result

'''
Rota para atualizar um dispositivo

json{
    "document_id":"id_do_dispositivo"
    "device": "novo_nome_device"
}

APENAS PARA ADMINS
'''
@devices_bp.route('/update', methods=['PUT'])
@admin_required
def update_device_by_id():

    try:
        data = request.get_json()

        if not 'document_id' in data or 'device' not in data:
            return jsonify ({'Error': 'Campos inválidos'}), 400

        result = device_service.update(data['document_id'], data)
    except Exception as e:
        logging.exception('Error in update')
        result = jsonify({'Error':'falha ao atualizar device'}), 500

    return result