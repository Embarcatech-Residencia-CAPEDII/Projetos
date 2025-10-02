from flask import Blueprint, jsonify, request
from datetime import datetime
from bson.json_util import dumps
from Flask.auth import token_required 
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from Flask.Services.device_service import AuthServiceDevice
from Flask.Models.device_db_model import DeviceDBModel
from Flask.Models.device_model import Device
from dotenv import load_dotenv
import os

'''
Arquivo para rotas para retornar os documentos de chamadas
'''

'''
ROTAS

/api/devices 
/api/devices/quantidade
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
device_db_model = DeviceDBModel(mongo_conn)
device_service = AuthServiceDevice(device_db_model)

devices_bp = Blueprint('devices', __name__, url_prefix='/api/devices')

'''
Rota que retorna uma lista com todos os dispositivos
'''
@devices_bp.route('/', methods=['GET'])
def return_all_devices():
    mongo_conn.start_connection()

    devices = device_db_model.return_all_devices()

    mongo_conn.close_connection()
    if devices:

        json_devices = dumps(devices)

        return jsonify(json_devices), 201
    
    return {'Error': 'Devices não encontrados'},400

'''
Rota que retorna a quantidade com todos os dispositivos cadastrados
'''
@devices_bp.route('/quantidade', methods=['GET'])
def return_count_devices():
    mongo_conn.start_connection()

    devices = device_db_model.return_all_devices()

    mongo_conn.close_connection()
    if devices:
        count_devices = len(devices)

        return {'Quantidade': count_devices}, 201
    
    return {'Error': 'Devices não encontrados'}, 400

'''
Rota que registra um novo dispositivo no banco de dados
'''
@devices_bp.route('/register', methods=['GET', 'POST'])
def register_device():
    data = request.get_json()

    mongo_conn.start_connection()

    device = Device(data['device'])

    if device.isValid() == False:
        return {'Error', 'Valores faltosos'}, 401
    
    result = device_service.register(device.getDevice(), device.getCreatedAt())

    mongo_conn.close_connection()

    return jsonify(result)
