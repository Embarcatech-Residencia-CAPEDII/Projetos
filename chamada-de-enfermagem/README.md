# Sistema de Chamada Emergencial para Pacientes em Ambientes Hospitalares

O sistema proposto tem como objetivo otimizar a comunicação entre pacientes, acompanhantes e a equipe de enfermagem em ambientes hospitalares. Para isso, serão instalados pontos de chamada (botões de emergência) junto aos leitos, permitindo que os pacientes ou seus acompanhantes acionem rapidamente o atendimento em casos de emergência.


## **Instalação e Configuração do Servidor**

### **1. Clonar o Repositório**

```bash
git clone https://github.com/Embarcatech-Residencia-CAPEDII/Projetos
cd ./chamada-de-enfermagem/server
```

### **2. Criar o Ambiente Virtual**
```bash
python -m venv .venv
```

```bash
source .venv/bin/activate   # Linux/MacOS
```

```bash
.venv\Scripts\activate      #Windows
```

### **3. Instalar o Flask e dependências**
```bash
pip install Flask
pip install flask-mqtt
pip install flask-pymongo
pip install dotenv
```
### **4. No arquivo .env coloque as credenciais para conexão com seu Banco de dados 
```
MONGO_DB_HOST = ''
MONGO_DB_PORT = ''
MONGO_DB_USER = ''
MONGO_DB_PASS = ''
MONGO_DB_DATABASE = ''
MONGO_DB_COLLECTION = ''
DB_URI = ''
```

### **4. Abrir o Servidor 
```bash
cd ./chamada-de-enfermagem/server
```
```bash
py client_mqtt.py        #Windows
```
```bash
python client_mqtt.py    #Linux
```

Acesse o servidor em: `http://127.0.0.1:5000`

