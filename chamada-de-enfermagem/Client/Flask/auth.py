import jwt
from functools import wraps
from flask import request, jsonify, redirect, url_for, render_template
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
            return redirect(url_for('pages.home')), 200
            
        try:
            data = jwt.decode(token, SECRET_KEY, algorithms=['HS256'])
            current_user = data.get('user')
            role = data.get('role')

        except jwt.ExpiredSignatureError:
            return render_template('login.html', error='Sessão expirada, Faça login novamente'), 401
            
        except jwt.InvalidTokenError:
            return render_template('login.html', error= 'Token inválido. Faça login novamente'), 401
            
        return f(*args, **kwargs)
    
    return decorated

'''
Decorator para uso em rotas para acesso somente a admins
'''
def admin_required(f):
    @wraps(f)
    @token_required # já valida token

    def decorated(*args, **kwargs):
        role = jwt.decode(request.cookies.get('jwt'), SECRET_KEY, algorithms=['HS256']).get('role')
        
        if role != 'admin':
            return render_template('login.html', error = 'Acesso negado'), 401
        
        return f(*args, **kwargs)
    return decorated


