from flask import Blueprint, jsonify, request
from datetime import datetime
from Flask.auth import token_required 
from Flask.Models.chamadas_db_model import ChamadasDBModel
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from MongoDB.mongo_conn import mongo_conn
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
/api/chamadas/contagem
/api/chamadas/dia/mes
'''

load_dotenv()

uri = os.getenv('MONGO_URI')
database = os.getenv('MONGO_DATABASE')

chamadas_db_model = ChamadasDBModel(mongo_conn)

chamadas_bp = Blueprint('chamadas', __name__, url_prefix='/api/chamadas')

'''
Rota de api para retorno de todas as chamadas
'''
@chamadas_bp.route('/', methods=['GET'])
@token_required
def return_all_documents_chamadas():

    try:
        documents = chamadas_db_model.return_all_chamadas()

        if documents:
            json_docs = convert_all_id_to_string(documents)
            return jsonify(json_docs), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 500

    return {'erro': 'Documentos não encontrados'}, 404

'''
Rota de api para contagem de todas as chamadas
'''
@chamadas_bp.route('/contagem', methods=['GET'])
@token_required
def return_all_chamadas_count():

    try:
        count_all_chamadas = chamadas_db_model.return_count_all_chamadas()

        if count_all_chamadas:
            return {'Quantidade': count_all_chamadas}, 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'Falha ao procurar informações'}), 500


    return {'Error': 'Docs não encontrados'}, 404

'''
Rota de api para retorno das chamadas do dia
'''
@chamadas_bp.route('/dia', methods=['GET'])
@token_required
def return_documents_chamadas_day():
    
    try:
        chamadas = chamadas_db_model.return_chamadas_by_day()

        if chamadas:
            
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200
        
    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'Falha em procurar documentos'}), 500
    
    return {'Erro': 'Docs não encontrados'},404

'''
Rota de api para contagem de chamadas diárias
'''
@chamadas_bp.route('/dia/contagem', methods=['GET'])
@token_required
def return_chamadas_day_count():
    
    try:
        count_chamadas = chamadas_db_model.return_number_of_chamadas_by_day()
    
        if count_chamadas or count_chamadas == 0:
            return {'Quantidade': count_chamadas}, 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'Falha em procurar documentos'}), 500
    
    return {'Error': 'Docs não encontrados'}, 404

'''
Rota de api paraa retornar a quantidade de chamadas por dia no mês
'''
@chamadas_bp.route('/dia/mes', methods=['GET'])
@token_required
def return_chamadas_by_day_in_month():
        
    try:
        count_chamadas = chamadas_db_model.return_count_chamadas_day_in_month()
        
        if count_chamadas:
            return jsonify(count_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error': 'Falha em procurar documentos'}), 500

    return {'Error': 'Docs não encontrados'}, 404