from werkzeug.security import generate_password_hash, check_password_hash
from Flask.Models.user_db_model import UserDBModel
from datetime import datetime
from flask import jsonify
'''
Classe de utilitário de autenticação
'''

class UserService:
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
        if self.user_db_model.check_user_exists_by_username(username):
            return jsonify({'error': 'Nome de usuário em uso'}), 400
        
        hashed_pw = generate_password_hash(password)

        if self.user_db_model.insert_user({
            'username': username,
            'password': hashed_pw,
            'role': role, 
            'createdAt': createdAt
            }) is False:

            return jsonify({'message': 'Usuário não inserido no banco de dados'}), 500

        return jsonify({'message': 'Usuário cadastrado com sucesso'}), 201


    '''
    Realiza uma procura por usuário e senha para login no sistema

    retorna um json com o status da operação
    '''
    def login(self, username:str, password:str):
        user = self.user_db_model.return_user_by_username(username)

        if not user or not check_password_hash(user['password'], password):
            return None

        return user
    
    '''
    Função de delete de usuários
    '''
    def delete(self, document_id:str):

        if self.user_db_model.check_if_user_exists_by_id(document_id) is False:
            return jsonify({'message': 'usuário não existe'}), 404
        
        if self.user_db_model.delete_user_by_id(document_id) is False:
            return jsonify({'message': 'usuário não deletado'}), 500
        
        return jsonify({'message': 'usuário deletado com sucesso!'}), 200
    
    '''
    Função de editar usuário
    '''

    def update(self, document_id, document_with_updates:dict):

        document_with_updates.pop('document_id')

        for key, value in document_with_updates.items():

            if key != 'username' and key != 'password' and key != 'role':
                return jsonify({'Message': 'Campos inválidos'}), 400

            if not value or value.split() == '' or ' ' in value:
                return jsonify({'Message': 'Valores com faltosos ou com espaço, tente novamente'}), 400

            if key == 'username':
                if self.user_db_model.check_user_exists_by_username(document_with_updates['username']):
                    return jsonify({'message': 'nome de usuário em uso'}), 400
            
            if key == 'role':    
                if value != 'user' and value != 'admin':
                    return jsonify({'message': 'Valores errados em tipo de usuário'}) , 400
                
            if key == 'password':
                password = value
                hashed_pw = generate_password_hash(password)
                document_with_updates['password'] = hashed_pw
            
        if self.user_db_model.update_user_by_id(document_id, document_with_updates) is False:
            return jsonify({'message': 'campos não atualizados'}), 500

        return jsonify({'message': 'campos atualizados!'}), 200