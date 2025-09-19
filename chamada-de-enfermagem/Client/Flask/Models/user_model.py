from Mqtt.application.models.MongoDBConnection import MongoDBConnection


'''
Classe para realizer a conexão com o banco de dados MongoDB
'''
class UserModel():

    '''
    Injeção de depêndencias da classe
    '''
    def __init__(self, db:MongoDBConnection):
        self.db = db


    '''
    Realiza uma procura por um documenento na database
    username = nome do usuário 

    retorna um dict com as informações do usuário
    '''
    def find_by_username(self, username:str):
        return self.db.return_document('users','username',username)
    
    '''
    Insere um novo usuário na database
    user_data = dicionário com as informações do novo usuário

    retorna um valor booleano
    '''
    def insert_user(self, user_data:dict):
        return self.db.insert_document("users", user_data)
    

