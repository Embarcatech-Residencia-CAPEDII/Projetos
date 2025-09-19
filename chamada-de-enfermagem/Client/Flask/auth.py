import jwt
from functools import wraps
from flask import request, jsonify, redirect, url_for
from dotenv import load_dotenv
import os

load_dotenv()

SECRET_KEY = os.getenv('SK')

'''
Decorator para verificar a existência de tokens nos cookies para permitir
acesso as demais páginas da aplicação 
f = função que chamada uma rota
'''
def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = None

        token = request.cookies.get('jwt') 

        if not token and 'Authorization' in request.headers:
            auth_header = request.headers['Authorization']

            if auth_header.startswith('Bearer '):
                token = auth_header.split(' ')[1]

        if not token:
            return redirect(url_for('auth.login'))
            
        try:
            data = jwt.decode(token, SECRET_KEY, algorithms=['HS256'])
            current_user = data.get('user')
            role = data.get('role')

        except jwt.ExpiredSignatureError:
            return redirect(url_for('pages.home'))
            
        except jwt.InvalidTokenError:
            return ({'error': 'Token inválido!'}), 401
            
        return f(current_user, role, *args, **kwargs)
    
    return decorated

