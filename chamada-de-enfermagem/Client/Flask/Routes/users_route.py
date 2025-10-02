from flask import Blueprint, jsonify, request
from Flask.Services.auth_service import AuthService
from Flask.Models.user_db_model import UserDBModel
from Flask.Models.user_model import User
from Flask.auth import SECRET_KEY
from Flask.auth import token_required 
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from bson.json_util import dumps
from dotenv import load_dotenv
import os

'''
Arquivo para rotas do client com users
'''

'''
ROTAS

/api/users 
/api/users/register
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
user_db_model = UserDBModel(mongo_conn)
auth_service = AuthService(user_db_model)

user_bp = Blueprint('user', __name__, url_prefix='/api/users')

'''
Rota de api para retornar todos os usuários
'''
@user_bp.route('/', methods=['GET'])
def return_all_users():

    mongo_conn.start_connection()

    users = mongo_conn.list_documents('users')

    if users:
        json_users = dumps(users)

        return jsonify(json_users), 201
    
    return {'Error', 'Usuário não encontrado'}, 401

'''
Rota de api para registar usuário
'''
@user_bp.route('/register', methods=['POST'])
def register():
    data = request.get_json()

    mongo_conn.start_connection()

    user = User(data['username'], data['password'], data['role'])

    if user.isValid() == False:
        return {'Error': 'Valores faltosos'}, 401
        
    result = auth_service.register(user.getUsername(), user.getPassword(), user.getRole(), user.getCreateAt())
    
    if result != None:
        return result

    mongo_conn.close_connection()

    return {'Error', 'usuário não inserido'}, 401
 