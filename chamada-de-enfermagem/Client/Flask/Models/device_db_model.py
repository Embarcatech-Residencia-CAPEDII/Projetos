from Mqtt.application.models.MongoDBConnection import MongoDBConnection


'''
Classe para procurar e inserir novos dispositivos 
'''
class DeviceDBModel():

    def __init__(self, db:MongoDBConnection):
        self.db = db

    def find_by_device(self, device:str):
        return self.db.return_document('devices', 'device', device)
    
    def insert_device(self, device_data:dict):
        return self.db.insert_document_collection('devices', device_data)
    
    def return_all_devices(self):
        return self.db.list_documents('devices')