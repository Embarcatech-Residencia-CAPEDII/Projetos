from Flask.Models.device_db_model import DeviceDBModel
from datetime import datetime

'''
Classe de utilitário de registro de devices
'''

class AuthServiceDevice:
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
            return {'error': 'Device já existente'}, 400

        if self.device_db_model.insert_device({
            'device': device,
            'createdAt': createdAt
            }) is False:

            return {'message': 'Device não inserido no banco de dados'}, 400

        return {'message': 'Device cadastrado com sucesso'}, 201
    
    def delete(self, document_id:str):

        if self.device_db_model.find_device_by_id(document_id) is False:
            return {'message': 'Dispositivo não encontrado'}, 400

        if self.device_db_model.delete_device_by_id(document_id) is False:
            return {'message': 'dispositivo não deletado'}, 400
        
        return {'message' : 'Dispositivo deletado!'} , 201

        
