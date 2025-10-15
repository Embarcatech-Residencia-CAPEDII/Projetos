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

    mongo_conn.start_connection()

    users = mongo_conn.list_all_documents_from_collection('users')

    if users:
        json_users = convert_all_id_to_string(users)

        return jsonify(json_users), 201
    
    return {'Error', 'Usuário não encontrado'}, 401

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
    data = request.get_json()

    mongo_conn.start_connection()

    user = User(data['username'], data['password'], data['role'])

    if user.isValid() == False:
        return {'Error': 'Valores faltosos'}, 401
        
    result = user_service.register(user.getUsername(), user.getPassword(), user.getRole(), user.getCreateAt())
    
    if result != None:
        return result

    mongo_conn.close_connection()

    return {'Error', 'usuário não inserido'}, 401
 
'''
Rota de remoção de usuário

json = {
    "document_id": "id_do_documento"
}
APENAS PARA ADMINS
'''
@user_bp.route('/delete', methods=['POST'])
def delete_user():
    data = request.get_json()

    mongo_conn.start_connection()

    result = user_service.delete(data['document_id'])

    mongo_conn.close_connection()

    print(result)

    return jsonify(result)

'''
Rota de remoção de usuário por url

APENAS PARA ADMINS
'''
@user_bp.route('/delete/<string:document_id>', methods=['GET', 'POST'])
def delete_user_by_id(document_id):

    mongo_conn.start_connection()

    result = user_service.delete(document_id)

    print(result)

    return jsonify(result)

'''
Rota de edição de usuários

O json pode ser qualquer um dos campos de usuários [username, password, role] 
mas com obrigatóriedade de ter seu ID
json = {
    "document_id" = "id_do_usuario"

}
APENAS PARA ADMINS
'''
@user_bp.route('/update', methods=['GET', 'POST'])
def update_user_by():
    

    data = json.loads(request.get_data())

    if not 'document_id' in data:
        return {'Error': 'Campos faltosos'}, 401
    
    mongo_conn.start_connection()
    result = user_service.update(data['document_id'], data)
    mongo_conn.close_connection()

    return jsonify(result)