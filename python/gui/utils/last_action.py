from enum import Enum

class LastAction(Enum):
    NO_ACTION = 0
    ADD_POINT = 1
    FINISH_POLYGON = 2
    UNDO = 3
    REDO = 4
    MOVE_POINT = 5
    GENERATE_SKELETON = 6
    REMOVE_SKELETON = 7
    
    
