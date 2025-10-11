from Mqtt.application.models.MongoDBConnection import MongoDBConnection


'''
Classe para realizer a conexão com o banco de dados MongoDB
'''
class UserDBModel():

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
    Realiza um busca por um documento por ID
    '''
    def find_user_by_id(self, document_id:str) -> bool:
        return self.db.check_if_document_exists_by_id('users', document_id)
    
    '''
    Insere um novo usuário na database
    user_data = dicionário com as informações do novo usuário

    retorna um valor booleano
    '''
    def insert_user(self, user_data:dict):
        return self.db.insert_document_collection("users", user_data)

    '''
    Deletar um usuários do banco de dados
    '''
    def delete_user_by_id(self, document_id:str):
        return self.db.delete_document_by_id('users', document_id)
    
    
