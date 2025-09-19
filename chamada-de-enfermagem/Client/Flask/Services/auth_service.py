from werkzeug.security import generate_password_hash, check_password_hash
from Flask.Models.user_model import UserModel

'''
Classe de utilitário de autenticação
'''

class AuthService:
    '''
    Utiliza um user_model para realizar as operações de registro e login
    '''
    def __init__(self, user_model:UserModel):
        self.user_model = user_model


    '''
    Registra um usuário com seu username e converte sua senha em um
    hash para ser inserido na base de dados

    retorna um json com o status da inserção
    '''
    def register(self, username:str, password:str):
        if self.user_model.find_by_username(username):
            return {'error': 'Usuário já existente'}, 400
        
        hashed_pw = generate_password_hash(password)
        if self.user_model.insert_user({'username': username, 'password': hashed_pw}) is False:
            return {'message': 'Usuário não inserido no banco de dados'}, 400

        return {'message': 'Usuário cadastrado com sucesso'}, 201


    '''
    Realiza uma procura por usuário e senha para login no sistema

    retorna um json com o status da operação
    '''
    def login(self, username:str, password:str):
        user = self.user_model.find_by_username(username)

        if not user or not check_password_hash(user['password'], password):
            return None

        return user

