
from flask import Blueprint, jsonify, request
import logging
from dotenv import load_dotenv
import os
from Flask.auth import token_required, admin_required
from Flask.Models.chamadas_status_model import ChamadasStatusModel
from Flask.Services.convert_objectdID import convert_all_id_to_string, convert_object_id_to_string
from MongoDB.MongoDBConnection import MongoDBConnection
from MongoDB.mongo_conn import mongo_conn


mapa_bp = Blueprint('mapa', __name__, url_prefix='/api/mapa')
'''
Rota de api para informar status das enfermaria

ROTAS
/api/mapa/status/oscioso
/api/mapa/status/emergencia
/api/mapa/status
'''
load_dotenv()

status_chamadas = ChamadasStatusModel(mongo_conn)

@mapa_bp.route('/status', methods=['GET'])
@token_required
def get_status():

    try:
        chamadas = status_chamadas.return_docs_from_status_chamadas()

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400

    return {'Error': 'Chamadas não encontradas'},404


@mapa_bp.route('/status/oscioso', methods=['GET'])
@token_required
def get_status_oscioso():

    try:
        chamadas = status_chamadas.return_docs_status('oscioso')

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400
    
    return {'Error': 'Chamadas não encontradas'},404

@mapa_bp.route('/status/emergencia', methods=['GET'])
@token_required
def get_status_emergencia():

    try:
        chamadas = status_chamadas.return_docs_status('emergencia')

        if chamadas:
            json_chamadas = convert_all_id_to_string(chamadas)
            return jsonify(json_chamadas), 200

    except Exception as e:
        logging.exception('Error in return documents')
        return jsonify({'Error':'failure in return documents'}), 400
    
    return {'Error': 'Chamadas não encontradas'},404

