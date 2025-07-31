 
from pymongo import MongoClient
from dotenv import load_dotenv
import os

load_dotenv()

uri = os.getenv('DB_URI') 
database = os.getenv('MONGO_DB_DATABASE')
collection = os.getenv('MONGO_DB_COLLECTION')

'''
Classe para conexão e CRUD para o banco de dados MongoDB
'''

class MongoDBConnection:

    def __init__(self, uri=uri, db_name=database, collection=collection):
        self.client = MongoClient(uri)
        self.database = self.client[db_name]
        self.collection = self.client[collection]

    def start_connection():
        pass
    

    '''
    Retorna uma lista com os dados
    '''
    def read_data(self, filter_data={}):
        return list(self.database[collection].find({}, {'_id': 0}))
                

    def insert_data():
        pass

    def delete_data():
        pass

    def update_data():
        pass
