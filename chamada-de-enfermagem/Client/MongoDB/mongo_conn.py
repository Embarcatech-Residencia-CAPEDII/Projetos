from MongoDB.MongoDBConnection import MongoDBConnection
from dotenv import load_dotenv
import os

load_dotenv()

database = os.getenv('MONGO_DATABASE')
uri = os.getenv('MONGO_URI')

mongo_conn = MongoDBConnection(uri, database)
mongo_conn.start_connection()


