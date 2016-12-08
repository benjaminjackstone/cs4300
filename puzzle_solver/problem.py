class Problem():
    def __init__(self, size):
        # self.actions = self.Actions(state)
        # self.newState = self.Result(state, self.action)
        # self.isGoal = self.GoalTest(state)
        self.size = size
        self.goalState = range(1, self.size*self.size + 1)
        self.goalState[-1] = -1

    def GoalTest(self, tileOrder):
        for i in range(len(tileOrder)):
            if self.goalState[i] == tileOrder[i]:
                continue;
            else:
                return False
        return True

    def Result(self, node, action, hole):
        state = node.tileOrder[:]
        state[hole], state[action] = state[action], state[hole]
        return state

    def Actions(self, hole):
        size = self.size
        col = hole%size
        row = hole/size
        actions = []
        if row +1 < size:
            actions.append(((hole+size), "U"))
        # else:
        #     actions.append((None, "U"))
        if row -1 >= 0:
            actions.append(((hole-size), "D"))
        # else:
        #     actions.append((None, "D"))
        if col - 1 >= 0:
            actions.append(((hole-1), "R"))
        # else:
        #     actions.append((None, "R"))
        if col + 1 < size:
            actions.append(((hole +1), "L"))
        # else:
        #     actions.append((None, "L"))
        return actions