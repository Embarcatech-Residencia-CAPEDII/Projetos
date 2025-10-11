from werkzeug.security import generate_password_hash, check_password_hash
from Flask.Models.user_db_model import UserDBModel
from datetime import datetime

'''
Classe de utilitário de autenticação
'''

class AuthService:
    '''
    Utiliza um user_model para realizar as operações de registro e login
    '''
    def __init__(self, user_db_model:UserDBModel):
        self.user_db_model = user_db_model

    '''
    Registra um usuário com seu username e converte sua senha em um
    hash para ser inserido na base de dados

    retorna um json com o status da inserção
    '''
    def register(self, username:str, password:str, role:str, createdAt: datetime):
        if self.user_db_model.find_by_username(username):
            return {'error': 'Usuário já existente'}, 400
        
        hashed_pw = generate_password_hash(password)

        if self.user_db_model.insert_user({
            'username': username,
            'password': hashed_pw,
            'role': role, 
            'createdAt': createdAt
            }) is False:

            return {'message': 'Usuário não inserido no banco de dados'}, 400

        return {'message': 'Usuário cadastrado com sucesso'}, 201


    '''
    Realiza uma procura por usuário e senha para login no sistema

    retorna um json com o status da operação
    '''
    def login(self, username:str, password:str):
        user = self.user_db_model.find_by_username(username)

        if not user or not check_password_hash(user['password'], password):
            return None

        return user
    
    '''
    Função de delete de usuários
    '''
    def delete(self, document_id:str):

        if self.user_db_model.find_user_by_id(document_id) is False:
            return {'message': 'usuário não existente'}, 400
        
        if self.user_db_model.delete_user_by_id(document_id) is False:
            return {'message': 'usuário não deletado'}, 400
        
        return {'message': 'usuário deletado com sucesso!'}, 201
    
    

