#!/usr/bin/env python3
'''
Arquivo para adicionar dispositvos ao broker
'''

import os
import subprocess
from dotenv import load_dotenv
import logging

load_dotenv()

logging.basicConfig(
    level=logging.INFO,
    format='Data: %(levelname)s - %(asctime)s - %(filename)s - %(message)s'
)

password = os.getenv('PASSWORD_DEVICES')
path_file_password_mosquitto = os.getenv('PATH_FILE_PASSWORD_MOSQUITTO')

def run_command_to_add_device_mosquitto(device:str):
    
    cmd_to_run = f'sudo mosquitto_passwd -b {path_file_password_mosquitto} {device} {password}'

    try:
        subprocess.run(cmd_to_run, shell=True, check=True)

        logging.info('Device registrado!')
    
    except Exception as e:
        logging.exception(e)

