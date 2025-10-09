 
from pymongo.mongo_client import MongoClient, PyMongoError, ConnectionFailure
from pymongo.server_api import ServerApi
from bson.objectid import ObjectId
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
    
        '''
    Verifica a existência de algum dado no banco de dados
    '''
    def check_if_document_exists_by_id(self, collection:str, document_id:str):

        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({'_id':ObjectId(document_id)})

                if result is not None:
                    return True                
        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def return_document(self,collection:str, label_to_search:str, value_to_match:str) -> dict:
         
        try:
            if self.client is not None:
                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({label_to_search:value_to_match})
        
                return result

        except PyMongoError as e:
            print('Error in check values...')
            print(e)
            
        return False
    
    def return_document_by_id(self, collection:str, id:str):

        try:
            if self.client is not None:

                collection_to_search = self.db[collection]

                result = collection_to_search.find_one({'_id':ObjectId(id)})

                return result
            
        except PyMongoError as e:
            print('Error in check values...')
            print(e)

    '''
    Retorna os dados com uma query de data
    '''
    def list_documents_by_date(self, collection:str, field_data:str, end_date:str):
        try:
            if self.client is not None:
                collection = self.db[collection]
                return list(collection.find({
                                field_data:{'$lt': end_date}}))
            else:
                print('Client not connected')
        except PyMongoError as e:
            print('Error in list documents...')
            print(e)

    '''
    Insere um novo documento na coleção
    '''
    def insert_document_collection(self,collection:str, document: dict):
        try:
            if self.client is not None:
                document_to_save = document 

                collection = self.db[collection]
                result = collection.insert_one(document_to_save)

                return result

            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error on insert document...')
            print(e) 

    '''
    Função para update de um valor de um label em uma collection
    '''
    def update_document_by_id(self, collection:str, document_id:str, label_to_update:str, new_value:str) -> bool: 

        try:
            if self.client is not None:

                document_to_update = self.return_document_by_id(collection, document_id)
                
                if document_to_update == None:
                    print('No values in DB')
                    return False
                
                
                if document_to_update.get(label_to_update) == new_value:
                    print('Same Value, update not complete')
                    return False
                
                collection_search = self.db[collection]                
                result = collection_search.update_one({'_id':ObjectId(document_id)}, {
                    '$set': {label_to_update: new_value}
                } )


                if result == None:
                    print(f'Error in update operation: {result}')
                    return False
                
                print(f'Successfully update operation: {result}')
                return True
            
            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error on update document...')
            print(e)

    '''
    Função para deletar um documento da database
    '''
    def delete_document(self, collection:str, label_to_match:str, value_to_match:str) -> bool:

        try:
            if self.client is not None:
                
                if self.check_if_document_exists(collection, label_to_match, value_to_match) == False:
                    print('No values in DB')
                    return False
                
                collection_delete = self.db[collection]
                result = collection_delete.delete_one({label_to_match: value_to_match})

                if result == False:
                    print(f'Error in delete value: {result}')
                    return False
                
                print(f'Successfully in delete value: {result}')
                return True
            
            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error in delete value')
            print(e)
            return False
        
    '''
    Função para deletar um documento da database
    '''
    def delete_document_by_id(self, collection:str, document_id:str) -> bool:

        try:
            if self.client is not None:
                
                if self.check_if_document_exists_by_id(collection, document_id) == False:
                    print('No values in DB')
                    return False
                
                collection_delete = self.db[collection]
                result = collection_delete.delete_one({'_id': ObjectId(document_id)})

                if result == False:
                    print(f'Error in delete value: {result}')
                    return False
                
                print(f'Successfully in delete value: {result}')
                return True
            
            else:
                print('Client not connected')

        except PyMongoError as e:
            print('Error in delete value')
            print(e)
            return False

    '''
    Função para fechar a conexão com o banco de dados
    '''
    def close_connection(self):
        if self.client:
            self.client.close()
            print("Conexão fechada.")

