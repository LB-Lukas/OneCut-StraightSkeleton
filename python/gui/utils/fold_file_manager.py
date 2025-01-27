import json


class FoldFileManager:
    @staticmethod
    def read_fold_file(path: str):
        with open(path, 'r') as file:
            data = json.load(file)
        return data


    @staticmethod
    def write_fold_file(path: str, data):
        with open(path, 'w') as file:
            json.dump(data, file, indent=4)
