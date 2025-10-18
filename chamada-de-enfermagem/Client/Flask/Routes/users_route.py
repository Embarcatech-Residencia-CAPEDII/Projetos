from flask import Blueprint, jsonify, request
from Flask.Services.user_service import UserService
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from Flask.Models.user_db_model import UserDBModel
from Flask.Models.user_model import User
from Flask.auth import SECRET_KEY
from Flask.auth import token_required 
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os
import json
import logging

'''
Arquivo para rotas do client com users
'''

'''
ROTAS

/api/users 
/api/users/register
/api/users/delete
/api/users/delete/<document_id>
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
user_db_model = UserDBModel(mongo_conn)
user_service = UserService(user_db_model)

user_bp = Blueprint('user', __name__, url_prefix='/api/users')

'''
Rota de api para retornar todos os usuários

APENAS PARA ADMINS
'''
@user_bp.route('/', methods=['GET'])
def return_all_users():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Error in connect to database')
        return {'Error': 'Erro interno do banco de dados'}, 500
    try:
        users = mongo_conn.list_all_documents_from_collection('users')

        if users:
            json_users = convert_all_id_to_string(users)

            return jsonify(json_users), 201

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'falha ao procurar documentos'})

    finally:
        mongo_conn.close_connection()
    
    return {'Error': 'Usuários não encontrados'}, 404

'''
Rota de api para registar usuário

json = {
    "username": "nome_do_usuario",
    "password": "senha_do_usuário"
}

APENAS PARA ADMINS
'''
@user_bp.route('/register', methods=['POST'])
def register():

    
    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Error in connect to database')
        return jsonify({'Error': 'Erro interno do banco de dados'}), 500
    
    try:
        data = request.get_json()

        if not 'username' in data or 'password' not in data or 'role' not in data:
            return jsonify({'Error': 'Campos inválidos'}), 400
            
        user = User(data['username'], data['password'], data['role'])
        if user.isValid() == False:
            return {'Error': 'Campos inválidos'}, 400
            
        result = user_service.register(user.getUsername(), user.getPassword(), user.getRole(), user.getCreateAt())
          
    except Exception as e:
        logging.exception('Error in register user')
        result = jsonify({'Error': 'falha em registrar usuário'}), 500

    finally:
        mongo_conn.close_connection()

    return result
 
'''
Rota de remoção de usuário

json = {
    "document_id": "id_do_documento"
}
APENAS PARA ADMINS
'''
@user_bp.route('/delete', methods=['DELETE'])
def delete_user():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Error in connect to database')
        return jsonify({'Error': 'Erro interno do banco de dados'}),500
    
    try:
        data = request.get_json()

        if not 'document_id' in data:
            return jsonify({'Error': 'Campos inválidos'}), 400
        
        result = user_service.delete(data['document_id'])
        
    except Exception as e:
        logging.exception('Error in delete user')
        result = jsonify({'Error': 'Falha ao deletar usuário'}), 500
    
    finally:
        mongo_conn.close_connection()

    return result

'''
Rota de remoção de usuário por url

APENAS PARA ADMINS
'''
@user_bp.route('/delete/<string:document_id>', methods=['DELETE'])
def delete_user_by_id(document_id):

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Error in connect to database')
        return jsonify({'Error': 'Erro interno do banco de dados'}),500
    
    try:
        if not document_id:
            return jsonify({'Error': 'ID inválido'}), 400
        
        result = user_service.delete(document_id)
        
    except Exception as e:
        logging.exception('Error in delete user')
        result = jsonify({'Error': 'Falha ao deletar usuário'}), 500
    
    finally:
        mongo_conn.close_connection()

    return result

'''
Rota de edição de usuários

O json pode ser qualquer um dos campos de usuários [username, password, role] 
mas com obrigatóriedade de ter seu ID
json = {
    "document_id" = "id_do_usuario"

}
APENAS PARA ADMINS
'''
@user_bp.route('/update', methods=['PUT'])
def update_user_by():
    
    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Error in connect to database')
        return jsonify({'Error': 'Erro interno do banco de dados'}),500
    
    try:
        data = request.get_json()

        if not 'document_id' in data:
            return {'Error': 'Campos inválidos'}, 400
        
        result = user_service.update(data['document_id'], data)
        
    except Exception as e:
        logging.exception('Error in update user')
        result = jsonify({'Error': 'Falha ao atualizar usuário'}), 500
    
    finally:
        mongo_conn.close_connection()

    return result