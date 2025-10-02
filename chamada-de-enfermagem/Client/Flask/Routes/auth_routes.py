from flask import Blueprint, request, jsonify, render_template, redirect, make_response, url_for
import jwt
from datetime import datetime, timedelta, timezone
from Flask.Services.auth_service import AuthService
from Flask.Models.user_db_model import UserDBModel
from Flask.Models.user_model import User
from Flask.auth import SECRET_KEY
from Flask.auth import token_required, admin_required
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os

'''
Arquivo para rotas do client
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
user_db_model = UserDBModel(mongo_conn)
auth_service = AuthService(user_db_model)

auth_bp = Blueprint('auth', __name__, url_prefix='/auth')

'''
ROTAS

/auth/register 
/auth/login
'''

'''
Rota de registro
POST para verificar se o usuário existe e registrá-lo no banco de dados

FORMATO DO JSON
{
    "username":"email@dominio.com",
    "password":"",
    "role":"admin/user"
}

retorna um json com mensagem de sucesso/falha na inserção
'''

@auth_bp.route('/register', methods=['POST'])
def register():
    data = request.get_json()
    mongo_conn.start_connection()

    user = User(data['username'], data['password'], data['role'])

    if user.isValid() == False:
        return {'Error': 'Valores faltosos'}, 401
        
    result = auth_service.register(user.getUsername(), user.getPassword(), user.getRole(), user.getCreateAt())
    
    mongo_conn.close_connection()

    return jsonify(result)
    

'''
Rota de login
POST para receber as informações do usuário por meio de um formulário

retorna um token para acessar o sistema ou uma mensagem de credenciais inválidas
'''
@auth_bp.route('/login', methods=['GET', 'POST'])
def login():

    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')

        mongo_conn.start_connection()
        user = auth_service.login(username, password)
        mongo_conn.close_connection()
        
        print(datetime.now() + timedelta(hours=3))
        
        if user != None:
            
            token = jwt.encode({
                'user'  :   user['username'],
                'role'  :   user['role'],
                'exp'   :   datetime.now(timezone.utc) + timedelta(minutes=10)
            }, SECRET_KEY, algorithm='HS256')

            print('TOKEN: ',token)

            resp = make_response(redirect(url_for('pages.relatorio_page')))
            resp.set_cookie('jwt', token, httponly=True)

            return resp

        return render_template('login.html', error='Credenciais Inválidas')

    return render_template('login.html')