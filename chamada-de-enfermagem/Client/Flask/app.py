from flask import Flask
from Flask.Routes.auth_routes import auth_bp
from Flask.Routes.pages_routes import pages_bp
from Flask.Routes.chamadas_route import chamadas_bp
from Flask.Routes.devices_route import devices_bp
from Flask.Routes.users_route import user_bp
from Flask.Routes.mapa_routes import mapa_bp
from Flask.Routes.error_routes import error_bp
import logging

app = Flask(__name__)

# registra blueprint
app.register_blueprint(pages_bp)
app.register_blueprint(auth_bp)
app.register_blueprint(chamadas_bp)
app.register_blueprint(devices_bp)
app.register_blueprint(user_bp)
app.register_blueprint(mapa_bp)
app.register_blueprint(error_bp)

logging.basicConfig(
    level=logging.INFO,
    format='Data: %(levelname)s - %(asctime)s - %(filename)s - %(message)s'
)

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)
