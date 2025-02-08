class UndoManager:
    def __init__(self):
        self._actions = []


    def record_action(self, action, data: dict):
        self._actions.append((action, data))


    def pop_action(self):
        if self._actions:
            return self._actions.pop()
        return None


    def has_actions(self) -> bool:
        return bool(self._actions)
