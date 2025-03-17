import json


class FoldFileManager:
    @staticmethod
    def read_fold_file(path: str):
        with open(path, 'r') as file:
            data = json.load(file)
            
        # Validate critical structure
        if "vertices_coords" not in data:
            raise ValueError("FOLD file missing required 'vertices_coords'")
        if "edges_vertices" not in data:
            raise ValueError("FOLD file missing required 'edges_vertices'")
            
        return data


    @staticmethod
    def write_fold_file(path: str, data):
        with open(path, 'w') as file:
            json.dump(data, file, indent=4)
