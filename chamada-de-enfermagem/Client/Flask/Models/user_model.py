from datetime import datetime

'''
Classe para instanciar usuário
'''

class User():
    def __init__(self, username:str, password:str, role:str):
        
        self.username = username
        self.password = password
        self.role = role
        self.createdAt = datetime.now()
        

    def isValid(self):
        if not self.username or self.username.split() == '':
            print('Error in username')
            return False
        if not self.password or self.password.split() == '':
            print('Error in password')
            return False
        if self.role != 'user' and self.role != 'admin' or self.role.split() == '':
            print('Error in role')
            return False
        
        return True

    def getUsername(self):
        return self.username
    
    def getPassword(self):
        return self.password
    
    def getCreateAt(self):
        return self.createdAt

    def getRole(self):
        return self.role