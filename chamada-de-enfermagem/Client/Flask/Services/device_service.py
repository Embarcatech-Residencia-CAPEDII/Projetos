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

        if self.device_db_model.inser_device({
            'device': device,
            'createdAt': createdAt
            }) is False:

            return {'message': 'Usuário não inserido no banco de dados'}, 400

        return {'message': 'Usuário cadastrado com sucesso'}, 201



