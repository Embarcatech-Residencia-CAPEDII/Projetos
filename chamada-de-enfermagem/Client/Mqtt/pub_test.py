import json
import paho.mqtt.client as mqtt

BROKER_HOST = 'localhost'
BROKER_PORT = 1883

def publish_message(topic: str, payload: dict):
    """Publica uma mensagem MQTT no tópico especificado."""
    client = mqtt.Client(client_id='Publisher')
    client.connect(BROKER_HOST, BROKER_PORT)
    client.publish(topic, json.dumps(payload))
    client.disconnect()
    print(f"Mensagem publicada em '{topic}': {payload}")

def pub_enfermagem_dispositivo(dispositivo_id: str, estado: str, mensagem: str, source: str, comando: str):
    """Publica um evento vindo da enfermagem."""
    topic = f'dispositivos/posto_enfermagem/{dispositivo_id}'
    payload = {
        'id': dispositivo_id,
        'estado': estado,
        'mensagem': mensagem,
        'source': source,
        'comando': comando
    }
    publish_message(topic, payload)

def pub_posto_enfermagem(dispositivo_id: str, estado: str, mensagem: str, source: str, comando: str):
    """Publica um evento vindo do posto de enfermagem no tópico local."""
    topic = f'dispositivos/enfermagem/{dispositivo_id}'
    payload = {
        'id': dispositivo_id,
        'estado': estado,
        'mensagem': mensagem,
        'source': source,
        'comando': comando
    }
    publish_message(topic, payload)

# ====== EXEMPLOS DE USO ======
pub_enfermagem_dispositivo(
        dispositivo_id='Enfermagem2',
        estado='emergencia',
        mensagem='Ligar LED',
        source='14',
        comando='ligar'
    )


pub_posto_enfermagem(
        dispositivo_id='Enfermagem1',
        estado='oscioso',
        mensagem='Desligar dispositivo',
        source='posto_enfermagem',
        comando='desligar'
    )

