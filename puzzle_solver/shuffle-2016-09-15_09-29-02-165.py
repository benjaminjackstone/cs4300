import random

class Shuffle():
    def __init__(self, size):
        self.neighbors = []
        self.hole = size*size - 1
        self.size = size
        self.tiles = []
        self.tiles = range(1, self.size*self.size + 1)
        self.tiles[-1] = -1
        self.last = -1

    def legalNeighbors(self):
        col = self.hole%self.size
        row = self.hole/self.size
        self.neighbors = []
        if col - 1 >= 0:
            self.neighbors.append(self.hole-1)
        if col + 1 < self.size:
            self.neighbors.append(self.hole +1)
        if row -1 >= 0:
            self.neighbors.append(self.hole-self.size)
        if row +1 < self.size:
            self.neighbors.append(self.hole+self.size)
        return self.neighbors

    def ShuffleList(self, count):
        for i in range(count):
            list1 = self.legalNeighbors()
            newTile = random.randrange(len(list1))
            newHole = list1[newTile]
            while newHole == self.last:
                newTile = random.randrange(len(list1))
                newHole = list1[newTile]
            self.swapCells(newHole)
            self.last = self.hole
        return self.tiles

    def swapCells(self, index):
        self.tiles[self.hole], self.tiles[index] = self.tiles[index], self.tiles[self.hole]
        self.hole = index