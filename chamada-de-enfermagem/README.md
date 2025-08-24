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
allow_anonymous false   #desativa entrada de subscribers sem autenticação
```

```bash
persistence true   #Diretórui de persistência
persistence_location /var/lib/mosquitto/    #caminho do diretório
```

```bash
log_dest file /var/log/mosquitto/mosquitto/log #Diretoŕio para logs
```

### **4. Criar diretórios de log e permissões**
```bash
sudo mkdir -p /var/log/mosquitto /var/lib/mosquitto
sudo touch /var/log/mosquitto/mosquitto.log
sudo chown -R mosquitto:mosquitto /var/log/mosquitto /var/lib/mosquitto
sudo chown mosquitto:mosquitto /etc/mosquitto/passwd
sudo chmod 600 /etc/mosquitto/passwd
```
### **5.Autenticação de usuários/dispositivos e senhas**
Para criar o arquivo de usuário e senhas, e digite e confirme a senha para o usuário
```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd <nome_do_usuário>    #Arch Linux
```

Para adicionar novos usuários/dispositivos
```bash
sudo mosquitto_passwd /etc/mosquitto/passwd <nome_do_usuário>   #Arch Linux 
```

### **6. Reinicie o Broker**
```bash
sudo systemctl restart mosquitto    #Arch Linux
```

## **Instalação e Configuração do Client**

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
pip install paho-mqtt
```
### **4. No arquivo .env coloque as credenciais para conexão com seu Banco de dados e do broker 
```
MONGO_URI = ''
MONGO_DATABASE = ''

BROKER_IP = ''
BROKER_PORT = ''
BROKER_TOPIC = ''

CLIENT_NAME = ''
CLIENT_USERNAME = ''
CLIENT_PASS = ''

```

### **4. Abrir o Servidor 
```bash
cd ./chamada-de-enfermagem/server/Mqtt/
```
```bash
py main.py        #Windows
```
```bash
python main.py    #Linux
```

### **5. Realizar um teste
Em outro terminal execute o arquivo pub_test.py
```bash
py pub_test.py  #Windows
```
```bash
python pub_test.py  #Linux
```

