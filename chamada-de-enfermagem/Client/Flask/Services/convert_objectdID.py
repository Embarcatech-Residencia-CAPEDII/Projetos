from bson.objectid import ObjectId

'''
Converte ObjectID em ids do tipo String
'''

def convert_object_id_to_string(document:dict) -> dict:

    objectId_to_convert = ObjectId(document.get('_id'))
    object_id_string = str(objectId_to_convert)
    document['_id'] = object_id_string

    return document

def convert_all_id_to_string(documents:list) -> list:

    new_docs = list()
    for doc in documents:
        doc = convert_object_id_to_string(doc)

        new_docs.append(doc)

    return new_docs
