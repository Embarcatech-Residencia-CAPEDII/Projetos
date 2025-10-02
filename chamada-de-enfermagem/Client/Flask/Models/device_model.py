from datetime import datetime

'''
Classe para instanciar device
'''

class Device():
    def __init__(self, device:str):
        self.device = device
        self.createdAt = datetime.now()


    def isValid(self):
        if not self.device or self.device.split() == '':
            print('Error in device')
            return False
        
        return True
    
    def getDevice(self):
        return self.device
    
    def getCreatedAt(self):
        return self.createdAt