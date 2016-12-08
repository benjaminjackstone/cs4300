import Queue
from Node import *
from problem import *
from shuffle import *
from hash import *

class BFS ():
    def __init__(self, tiles, size):
        self.tileOrder = tiles
        self.size = size

    def Search(self):
        hash = Hash(10000)
        P = Problem(self.size)
        N = Node(self.tileOrder, None, None)
        Q = Queue.Queue(maxsize=0)
        Q.put(N)
        path = []
        nodesProcessed = 0
        while not Q.empty():
            hole = -1
            N = Q.get()
            if P.GoalTest(N.tileOrder):
                break
            for i in range(len(N.tileOrder)):
                if N.tileOrder[i] == - 1:
                    hole = i
                    break
            actions = P.Actions(hole)
            # stringList = ''
            # for i in range(self.size*self.size):
            #     if N.tileOrder[i] != -1:
            #         stringList += str(N.tileOrder[i])
            #     else:
            #         stringList += '0'
            # hash.Insert(stringList)
            for a in actions:
                #if a[0] != None:
                    newOrder = P.Result(N, a[0], hole)
                    n1 = Node(newOrder, N, a[1])
                    # stringList = ''
                    # for i in range(self.size*self.size):
                    #     if n1.tileOrder[i] != -1:
                    #         stringList += str(n1.tileOrder[i])
                    #     else:
                    #         if i == 0 and n1.tileOrder[0] == 0:
                    #             stringList += '10'
                    #             continue
                    #         stringList += '0'
                    # if hash.exists(stringList):
                    #     continue
                    # else:
                    Q.put(n1)
                    #hash.Insert(stringList)
            nodesProcessed += 1
        while N.parent != None:
            path.append(N.action)
            N = N.parent
        path.reverse()
        print Q.qsize(), " = remaining queue size"
        print nodesProcessed, " = nodes taken from queue"
        print Q.qsize() + nodesProcessed, " = total nodes"
        return path

def main():
	for i in range(100):
		size = 3
		shuf = Shuffle(size)
		num = random.randint(5,10)
		tileOrder = shuf.ShuffleList(num)
	#flag = input("type '1' for int by int input or '2' for random generator: ")
	#if flag == 2:
	 #   size = input('How many rows? ')
	  #  shuf = Shuffle(size)
	   # num = input('How many moves? ')
	    #tileOrder = shuf.ShuffleList(num)
	#else:
	#	b = input("input size 'b': ")
	#	size = b
	#	tileOrder = []
	#	for i in range(b*b):
	#		ans = input("input index " + str(i) + ": ")
	#		tileOrder.append(ans)
    # fout = open('input.txt', "w")
    # fout.write(str(size) + '\n')
    # for i in range(size*size):
    #     fout.write(str(tileOrder[i]) + '\n')
    # fout.close()
    # file = raw_input("Type the file name with extension .txt: ")
    # fin = open(file, "rb")
    # line = fin.readline()
    # size = int(line)
    # tileOrder = []
    # while line:
    #     line = fin.readline()
    #     if line != '':
    #         tileOrder.append(int(line))
    # fin.close()
    #tileOrder = [0,1,2,3,4,5,6,7,9,-1,10,11,8,12,13,14]
		bfs = BFS(tileOrder, size)
    		print "STARTING TILE ORDER"
    		for i in range(len(tileOrder)):
        		if i%size == 0:
            			print '\n'
        		print tileOrder[i],
    		print
   		print "STARTING SEARCH"
    		path = bfs.Search()
    	#fout = open('path.txt', "w")
    	#for i in range(len(path)):
        #	fout.write(str(path[i]) + '\n')
    	#fout.close()
    		print path
main()