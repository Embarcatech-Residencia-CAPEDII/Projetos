from flask import Blueprint, jsonify
from datetime import datetime
from bson.json_util import dumps
from Flask.auth import token_required 
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os

'''
Arquivo para rotas para retornar os documentos de chamadas
'''

'''
ROTAS

/api/chamadas/ 
/api/chamadas/dia
/api/chamadas/dia/contagem
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)

chamadas_bp = Blueprint('chamadas', __name__, url_prefix='/api/chamadas')

'''
Rota de api para retorno de todas as chamadas
'''
@chamadas_bp.route('/', methods=['GET'])
def return_all_documents_chamadas():
    mongo_conn.start_connection()

    documents = mongo_conn.list_documents('chamadas')

    mongo_conn.close_connection()

    if documents:
        json_docs = dumps(documents)
        return jsonify(json_docs), 201
    
    return {'erro': 'Documentos não encontrados'}, 400

'''
Rota de api para retorno das chamadas do dia
'''
@chamadas_bp.route('/dia', methods=['GET','POST'])
def return_documents_chamadas_day():
    mongo_conn.start_connection()

    data = datetime.now()

    start_date = f'ISODate({data.year}-0{data.month}-{data.day}T0:00:00.000Z)'
    end_date = f'ISODate({data.year}-0{data.month}-{data.day}T23:59:59:000Z)'

    print(start_date, end_date)

    documents = mongo_conn.list_documents_by_date('chamadas', 'Data', end_date)

    mongo_conn.close_connection()
    if documents:
        return {'sucesso': 'Docs encontrados'}, 201
    
    return {'Erro': 'Docs não encontrados'},400

'''
Rota de api para contagem de chamadas diárias
'''
@chamadas_bp.route('/dia/contagem')
def return_chamadas_day_count():
    mongo_conn.start_connection()

    data = datetime.now()

    start_date = f'ISODate({data.year}-0{data.month}-{data.day}T0:00:00.000Z)'
    end_date = f'ISODate({data.year}-0{data.month}-{data.day}T23:59:59:000Z)'

    print(start_date, end_date)

    documents = mongo_conn.list_documents_by_date('chamadas', 'Data', end_date)

    mongo_conn.close_connection()

    if documents:
        count_documents = len(documents)
        return {'Quantidade': count_documents}
    
    return {'Erro': 'Docs não encontrados'}, 400
