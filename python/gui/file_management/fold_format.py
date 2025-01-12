import json
from typing import List, Optional, Union

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

class FOLDFormat(Frame):
    def __init__(self):
        super().__init__()
        self.file_spec = 1.1
        self.file_creator = "Straight-Skeleton Tool"
        self.file_author = ""
        self.file_classes = []
        self.file_frames = []

    def get_file_spec(self) -> float:
        return self.file_spec

    def set_file_spec(self, file_spec: float):
        self.file_spec = file_spec

    def get_file_creator(self) -> str:
        return self.file_creator

    def set_file_creator(self, file_creator: str):
        self.file_creator = file_creator

    def get_file_author(self) -> str:
        return self.file_author

    def set_file_author(self, file_author: str):
        self.file_author = file_author

    def get_file_classes(self) -> List[str]:
        return self.file_classes

    def set_file_classes(self, file_classes: List[str]):
        self.file_classes = file_classes

    def get_file_frames(self) -> List[Frame]:
        return self.file_frames

    def set_file_frames(self, file_frames: List[Frame]):
        self.file_frames = file_frames

    def get_frame(self, index: int) -> Frame:
        if index == 0:
            return self
        frame = self.file_frames[index - 1]
        if getattr(frame, 'frame_inherit', False):
            parent = self.get_frame(frame.get_frame_parent())
            return self.merge(parent, frame)
        return frame

    def merge(self, parent: Frame, child: Frame) -> Frame:
        frame = Frame()
        frame.frame_classes = parent.frame_classes or child.frame_classes
        frame.frame_attributes = parent.frame_attributes or child.frame_attributes
        frame.frame_title = parent.frame_title or child.frame_title
        frame.frame_description = parent.frame_description or child.frame_description
        frame.edges_assignment = parent.edges_assignment or child.edges_assignment
        frame.edges_vertices = parent.edges_vertices or child.edges_vertices
        frame.vertices_coords = parent.vertices_coords or child.vertices_coords
        frame.face_orders = parent.face_orders or child.face_orders
        frame.faces_vertices = parent.faces_vertices or child.faces_vertices
        return frame