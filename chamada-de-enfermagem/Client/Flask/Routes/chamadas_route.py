from flask import Blueprint, jsonify
from datetime import datetime
from Flask.auth import token_required 
from Flask.Models.chamadas_db_model import ChamadasDBModel
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from Mqtt.application.models.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os
import logging
'''
Arquivo para rotas para retornar os documentos de chamadas
'''

'''
ROTAS

/api/chamadas/ 
/api/chamadas/dia
/api/chamadas/dia/contagem
/api/contagem
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

mongo_conn = MongoDBConnection(uri, database)
chamadas_db_model = ChamadasDBModel(mongo_conn)

chamadas_bp = Blueprint('chamadas', __name__, url_prefix='/api/chamadas')

'''
Rota de api para retorno de todas as chamadas
'''
@chamadas_bp.route('/', methods=['GET'])
def return_all_documents_chamadas():
    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'error':'Erro interno do banco de dados'}), 500

    try:
        documents = chamadas_db_model.return_all_chamadas()
    
        if documents:
            json_docs = convert_all_id_to_string(documents)
            return jsonify(json_docs), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 500
    
    finally:
        mongo_conn.close_connection()

    return {'erro': 'Documentos não encontrados'}, 404

'''
Rota de api para contagem de todas as chamadas
'''
@chamadas_bp.route('/contagem', methods=['GET'])
def return_all_chamadas_count():
    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'error':'Erro interno do banco de dados'}), 500

    try:
        count_all_chamadas = chamadas_db_model.return_count_all_chamadas()

        if count_all_chamadas:
            return {'Quantidade': count_all_chamadas}, 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'Falha ao procurar informações'}), 500
    
    finally:
        mongo_conn.close_connection()

    return {'Error': 'Docs não encontrados'}, 404

'''
Rota de api para retorno das chamadas do dia
'''
@chamadas_bp.route('/dia', methods=['GET'])
def return_documents_chamadas_day():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'error':'Erro interno do banco de dados'}), 500
    
    try:
        chamadas = chamadas_db_model.return_chamadas_by_day()

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'Falha em procurar documentos'}), 500
    
    finally:
        mongo_conn.close_connection()
    
    return {'Erro': 'Docs não encontrados'},404

'''
Rota de api para contagem de chamadas diárias
'''
@chamadas_bp.route('/dia/contagem', methods=['GET'])
def return_chamadas_day_count():

    try:
        mongo_conn.start_connection()
    except Exception as e:
        logging.exception('Erro ao conectar ao banco de dados')
        return jsonify({'error':'Erro interno do banco de dados'}), 500
    
    try:
        count_chamadas = chamadas_db_model.return_number_of_chamadas_by_day()
    
        if count_chamadas:
            return {'Quantidade': count_chamadas}, 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'Falha em procurar documentos'}), 500
    
    finally:
        mongo_conn.close_connection()

    return {'Error': 'Docs não encontrados'}, 404
