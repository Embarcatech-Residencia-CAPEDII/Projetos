# Sistema de Chamada Emergencial para Pacientes em Ambientes Hospitalares

O sistema proposto tem como objetivo otimizar a comunicação entre pacientes, acompanhantes e a equipe de enfermagem em ambientes hospitalares. Para isso, serão instalados pontos de chamada (botões de emergência) junto aos leitos, permitindo que os pacientes ou seus acompanhantes acionem rapidamente o atendimento em casos de emergência.


##  **Instação e configuração do Broker**

Para que o client funcione corretamente é necessário instalar em seu sistema operacional o broker Mosquitto e realizar mudanças em suas configurações para garantir mais segurança dentro da rede 

### **1. Instalação do broker Mosquitto**
Acesse o site https://mosquitto.org/download/ e siga os passos de acordo com seu sistema operacional, este passo a passo seguirá usando o terminal linux

### **2. Iniciando Mosquitto como serviço**
Permite que o broker sempre inicie ao ligar
```bash
sudo systemctl enable mosquitto     #Arch Linux
sudo systemctl start mosquitto      #Arch Linux
```
### **3. Criar/editar arquivo de configuração do broker**
```bash
sudo nano /etc/mosquitto/mosquitto.conf     #Arch Linux
```

Adicione/edite as seguintes linhas do arquivo
```bash
#################################
# CONFIGURAÇÕES BÁSICAS
#################################

listener 1883
allow_anonymous false

#################################
# AUTENTICAÇÃO
#################################

password_file /etc/mosquitto/passwd

#################################
# PERSISTÊNCIA
#################################

persistence true
persistence_location /var/lib/mosquitto/

#################################
# LIMITES
#################################

max_inflight_messages 20
max_queued_messages 100
max_packet_size 1048576

#################################
# LOGS
#################################

log_dest file /var/log/mosquitto/mosquitto.log
log_type error
log_type warning
log_type notice
log_type information
```

### **4. Usuários e Senhas**
Crie os usuários/dispositivos para conexão com o broker
```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd <usuario/dispositivo>
```
Adicionar mais usuários/dispositivos
```bash
sudo mosquitto_passwd /etc/mosquitto/passwd <usuario/dispositivo>
```

### **5.Permissões de escrita de arquivos**
Para permitir que o mosquitto possa registrar novos dispositivos
```bash
sudo chown root:mosquitto /etc/mosquitto/passwd
sudo chmod 640 /etc/mosquitto/passwd
ls -l
```
Saída esperada:
```bash
 -rw-r----- 1 root mosquitto /etc/mosquitto/passwd 
```

Arquivo de Log
```bash
sudo chown mosquitto:mosquitto /var/log/mosquitto/mosquitto.log
sudo chmod 700 /var/log/mosquitto/mosquitto.log
```

### **6. Reinicie o Broker**
```bash
sudo systemctl reset-failed mosquitto
sudo systemctl restart mosquitto
sudo systemctl status mosquitto
```

## **Instalação e Configuração do Client**

### **1. Clonar o Repositório**

```bash
git clone https://github.com/Embarcatech-Residencia-CAPEDII/Projetos
cd ./chamada-de-enfermagem
```

### **2. Criar o Ambiente Virtual e instale as dependências**
```bash
python -m venv .venv
```

```bash
source .venv/bin/activate   # Linux/MacOS
```

```bash
.venv\Scripts\activate      #Windows
```

```bash
pip install -r requirements.txt
```


### **3. Instale e crie o usuário no mongodb**
Acesse o site oficial do MongoDB e faça o tutorial de instalação, após a instalação crie o usuário de crud para a database.

```bash
mongosh
```
```bash
use admin
```
```bash
db.createUser({user:'<nome_user>',pwd:'<senha>', roles:[{role:'readWrite',db:'<nome-database>'}]})
```
### **4. No arquivo .env coloque as credenciais para conexão com seu Banco de dados e do broker**
```
#String de conexão com o banco de dados
MONGO_URI ='mongodb://<nome-user>:<senha>@localhost:27017/<nome-database>?authSource=admin' 

#Nome do banco de dados
MONGO_DATABASE = 'nome-database'

#Dados de conexão com o broker
BROKER_IP = ''
BROKER_PORT = ''
BROKER_TOPIC = ''

#Senha para novos dispositivos e caminho do arquivo de senhas
PASSWORD_DEVICES = ''
PATH_FILE_PASSWORD_MOSQUITTO = ''

#Dados para conexão do client-mqtt
CLIENT_NAME = ''
CLIENT_USERNAME = ''
CLIENT_PASS = ''

#Dados para uso interno do mqtt-client
PUB_USERNAME = ''
PUB_PASS = ''

#Secret-Key
SK = ''
```
## **Instalação e Configuração do Client**


### **1. Rodar as aplicações nativamente no servidor**
Utilize o terminal para chegar na pasta system
```bash
cd /etc/systemd/system
```
Crie um arquivo de serviço do Mqtt-Client
```bash
sudo nano mqtt-client.service
```
Copie e cole as seguintas linhas
```bash
[Unit]

Description=MQTT Client Service
After=network.target

[Service]
User=<usuario>
Group=<grupo do usuário>
Type=simple
Environment="PYTHONPATH=/caminho/do/projeto/Projetos-EmbarcaTech/chamada-de-enfermagem/Client"
WorkingDirectory=/caminho/do/projeto/Projetos-EmbarcaTech/chamada-de-enfermagem/Client/Mqtt
ExecStart=/caminho/do/projeto/Projetos-EmbarcaTech/chamada-de-enfermagem/.venv/bin/python -m Mqtt.main

#Arquivos de logs
StandardOutput=file:/caminho/do/arquivo/de/logs/Chamada-Enfermagem/mqtt-log.err.log
StandardError=file:/caminho/do/arquivo/de/logs/Chamada-Enfermagem/mqtt-log.err.log


Restart=always
RestartSec=5
Environment="PYTHONUNBUFFERED=1"

[Install]
WantedBy=multi-user.target
```

Faça o mesmo para o Flask-Backend.Service
Crie um arquivo de serviço do Flask-Backend
```bash
sudo nano flask-backend.service
```
Copie e cole as seguintas linhas
```bash
[Unit]

Description=Gunicorn service for Flask Backend
After=network.target

[Service]
User=<usuário>
Group=<grupo do usuário>

Environment="PYTHONPATH=/caminho/do/projeto/Projetos-EmbarcaTech/chamada-de-enfermagem/Client"
WorkingDirectory=/caminho/do/projeto//Projetos-EmbarcaTech/chamada-de-enfermagem/Client/Flask
ExecStart=/caminho/do/projeto//Projetos-EmbarcaTech/chamada-de-enfermagem/.venv/bin/gunicorn -w 4 -k gevent  -b 0.0.0.0:5000 app:app

#Arquivos de logs
StandardOutput=file:/caminho/do/projeto/logs/Chamada-Enfermagem/gunicorn.log
StandardError=file:/caminho/do/projeto/logs/Chamada-Enfermagem/gunicorn.err.log


Restart=always
Environment="PYTHONUNBUFFERED=1"
[Install]
WantedBy=multi-user.target
```

Após criar os arquivos de configurações habilite os serviços
```bash
sudo systemctl daemon-reload
sudo systemctl enable flask-backend.service
sudo systemctl enable mqtt-client.service
```

Inicie os serviços  
```bash
sudo systemctl start flask-backend
sudo systemctl start mqtt-client
```

Veja o status dos serviços  
```bash
sudo systemctl status flask-backend
```
```bash
sudo systemctl status mqtt-client
```

### **6. Abrir o Client Flask em ambiente de desenvolvimento para visualizar o software**
```bash
cd ./chamada-de-enfermagem/Client
```
```bash
python -m Flask.app        #Windows/Linux
```

### **7. Abrir o Client Flask em ambiente de produção para visualizar o software**
```bash
cd ./chamada-de-enfermagem/Client
```
```bash
gunicorn -w 4 -b 0.0.0.0:5000 Flask.app:app        #Linux
```

### **8. Abrir a documentação da API em ambiente de produção**
```bash
cd ./chamada-de-enfermagem/Client
```
```bash
python -m Flask.doc        #Linux
```


### **9. Realizar um teste**
Em outro terminal execute o arquivo pub_test.py e veja a nova chamada dentro do app 
```bash
python pub_test.py  #Windows/Linux
```

