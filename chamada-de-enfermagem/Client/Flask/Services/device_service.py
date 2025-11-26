from Flask.Models.device_db_model import DeviceDBModel
from datetime import datetime
from flask import jsonify
from bson.objectid import ObjectId

'''
Classe de utilitário de registro de devices
'''

class DeviceService:
    '''
    Utiliza um DeviceDBModel para realizar as operações de registro
    '''
    def __init__(self, device_db_model:DeviceDBModel):
        self.device_db_model = device_db_model

    '''
    Registra um device com seu nome
    retorna um json com o status da inserção
    '''
    def register(self, device:str, createdAt: datetime):
        if self.device_db_model.find_by_device(device):
            return jsonify({'error': 'Device já existente'}), 400

        if self.device_db_model.insert_device({
            'device': device,
            'createdAt': createdAt
            }) is False:

            return jsonify({'message': 'Device não inserido no banco de dados'}), 500

        return jsonify({'message': 'Device cadastrado com sucesso'}), 201
    
    def delete(self, document_id:str):

        if ObjectId.is_valid(document_id) is False: 
            return jsonify({'message':'ID de dispositivo incorreto'}), 404

        if self.device_db_model.find_device_by_id(document_id) is False:
            return jsonify({'message': 'Dispositivo não encontrado'}), 404

        if self.device_db_model.delete_device_by_id(document_id) is False:
            return jsonify({'message': 'dispositivo não deletado'}), 500
        
        return jsonify({'message' : 'Dispositivo deletado!'}) , 200

    def update(self, document_id:str, document_with_updates:dict):
        document_with_updates.pop('document_id')

        if ObjectId.is_valid(document_id) is False: 
            return jsonify({'message':'ID de dispositivo incorreto'}), 404

        for key, name_device in document_with_updates.items():

            if key != 'device':
                return jsonify({'message': 'Campos inválidos'}), 400
    
            if not name_device or name_device.split() == '' or ' ' in name_device:
                return jsonify({'message': 'Valores faltosos ou com espaço, verifique as informações'}), 400
            
            if self.device_db_model.find_device_by_id(document_id) is False:
                return jsonify({'message': 'Device não encontrado'}), 400
            
            if self.device_db_model.find_by_device(name_device) is True:
                return jsonify({'message': 'Nome de device em uso, valor não alterado!'}), 400
            
        if self.device_db_model.update_device(document_id, document_with_updates) is False:
            return jsonify({'message': 'Device não alterado, tente novamente'}), 500
        
        return jsonify({'Message': 'Dispositivo atualizado!'}), 200