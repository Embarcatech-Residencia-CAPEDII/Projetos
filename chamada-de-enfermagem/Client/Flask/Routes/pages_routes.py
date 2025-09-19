from flask import Blueprint, render_template
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
from Flask.auth import token_required 
import os

'''
Arquivo para renderizar os templates de cada rota
'''
load_dotenv()

pages_bp = Blueprint('pages', __name__)

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)

'''
Rota de home(/)
renderiza o template de login no navegador do usuário
'''
@pages_bp.route('/')
def home():
    return render_template('login.html')

'''
Rota para o template de relatório apenas quando o usuário estiver logado
'''
@pages_bp.route('/relatorio', methods=['GET'])
@token_required
def relatorio_page(current_user, role):

    mongo_conn.start_connection()

    chamadas = mongo_conn.list_documents('chamadas')
    print(chamadas)

    mongo_conn.close_connection()

    return render_template('relatorio.html', chamadas=chamadas)


