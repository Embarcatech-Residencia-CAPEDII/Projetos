 
from pymongo.mongo_client import MongoClient, PyMongoError, ConnectionFailure
from pymongo.server_api import ServerApi
import os
from datetime import datetime

'''
Classe para conexão com banco de dados MongoDB
'''

class MongoDBConnection:

    def __init__(self, uri:str, database_name:str):
        self.uri = uri
        self.database_name = database_name
        self.client = None
        self.db = None

    '''
    Função de iniciar a conexão com o banco de dados
    '''
    def start_connection(self):
        
        if self.uri != None and self.database_name != None:
            try:
                self.client = MongoClient(self.uri, server_api=ServerApi('1'))
                self.db = self.client[self.database_name]
                self.client.admin.command('ping')
                print("Pinged your deployment. You successfully connected to MongoDB!")
                return True
            except ConnectionFailure as e:
                self.client = None
                self.db = None
                print(e)
                return False
        else:
            print('Database não definida')
            return False
        
    '''
    Insere um novo documento na coleção
    '''
    def insert_document_collection(self,collection:str,id_device:int, local:str, room_number:int):

        try:
            if self.client is not None:
                document = {
                    'id_dispositivo': id_device,        #id do arduino
                    'Local': local,                     #Onde foi a chamada (Enfermaria ou outra sala)
                    'Enfermaria': room_number,          #Número da enfermaria
                    'data': datetime.now()              #Hora da chamada
                }

                collection = self.db[collection]
                result = collection.insert_one(document)

                print(result)

            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error on insert document...')
            print(e) 

    def insert_document(self,collection:str, data:dict)->bool:
        
        try:
            if self.client is not None:
                document = data
                collection = self.db[collection]
                result= collection.insert_one(document)

                print(result)

                return True

            else:
                print('Client not connected')
                return False

        except PyMongoError as e:
            print('Error on insert document...')
            print(e) 
            return False
        

    '''
    Lista os documentos na coleção
    '''
    def list_documents(self, collection:str):
        try:
            if self.client is not None:
                collection = self.db[collection]
                return list(collection.find())
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)
    '''
    Verifica a existência de algum dado no banco de dados
    '''
    def check_if_document_exists(self, collection:str, label:str, value:str):

        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({label:value})

                if result is not None:
                    return True                
        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def return_document(self,collection:str, label_to_search:str, value_to_match:str):
         
        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({label_to_search:value_to_match})
        
                return result

        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    '''
    Função para fechar a conexão com o banco de dados
    '''
    def close_connection(self):
        if self.client:
            self.client.close()
            print("Conexão fechada.")
