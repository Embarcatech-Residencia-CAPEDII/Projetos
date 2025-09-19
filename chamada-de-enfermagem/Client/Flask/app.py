from flask import Flask
from Flask.Routes.auth_routes import auth_bp
from Flask.Routes.pages_routes import pages_bp

app = Flask(__name__)

# registra blueprint
app.register_blueprint(pages_bp)
app.register_blueprint(auth_bp)

if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)