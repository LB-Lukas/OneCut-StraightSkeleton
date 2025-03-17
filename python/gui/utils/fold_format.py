from typing import List, Dict, Any

class Frame:
    def __init__(self):
        self.frame_classes = []
        self.frame_attributes = []
        self.frame_title = ""
        self.frame_description = ""
        self.edges_assignment = []
        self.edges_vertices = []
        self.vertices_coords = []
        self.face_orders = []
        self.faces_vertices = []

    def to_dict(self) -> Dict[str, Any]:
        data = {
            "frame_title": self.frame_title,
            "frame_attributes": self.frame_attributes,
            "vertices_coords": self.vertices_coords,
            "edges_vertices": self.edges_vertices,
            "edges_assignment": self.edges_assignment,
            "faces_vertices": self.faces_vertices,
            "faceOrders": self.face_orders,
        }
        return {k: v for k, v in data.items() if v}

class FOLDFormat(Frame):
    def __init__(self):
        super().__init__()
        self.file_version = 1  # Corrected to match spec
        self.file_creator = "Straight-Skeleton Tool"
        self.file_author = ""
        self.file_classes = []
        self.file_frames = []

    def to_dict(self) -> Dict[str, Any]:
        frame_data = super().to_dict()
        data = {
            "file_version": self.file_version,
            "file_creator": self.file_creator,
            "file_author": self.file_author,
            "file_classes": self.file_classes,
            **frame_data
        }
        if self.file_frames:
            data["file_frames"] = [frame.to_dict() for frame in self.file_frames]
        return {k: v for k, v in data.items() if v}