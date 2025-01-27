from enum import Enum

class LastAction(Enum):
    ADD_POINT = 1
    FINISH_POLYGON = 2
    UNDO = 3
    REDO = 4
    MOVE_POINT = 5
    NO_ACTION = 6
