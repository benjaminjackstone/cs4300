# make it dynamic by starting at 101 and when getting 51 big re-size the table
# this is when you are dont with the basic project
class Hash:
    def __init__(self, size):  # size can be entered as the recommended size
        table_size = size * 2 + 1
        while not self.is_prime(table_size):
            # make it as large as size + additional to make it prime
            table_size += 2
        self.table = [None] * table_size
        self.total = 0.0

    def Insert(self, item):
        if self.exists(item):
            # cannot have duplicates
            return False
        index = int(item) % len(self.table)
        while self.table[index] is not None:
            index += 1
            if index > len(self.table) - 1:
                index = 0
        self.table[index] = item
        return True

    def is_prime(self, n):
        if n == 2 or n == 3: return True  # 2 and 3 are prime
        if n < 2 or n % 2 == 0: return False  # even or ness than 2
        if n < 9: return True  # less than 9
        if n % 3 == 0: return False  # divisible by 3
        r = int(n ** 0.5)  # square root of n
        f = 5
        while f <= r:  # while 5 <= sqrt(n)
            # print '\t',f
            if n % f == 0: return False  # n divided by 5, 11, 17...
            if n % (f + 2) == 0: return False  # n divided by 7, 13, 19...
            f += 6
        return True

    def Retrieve(self, item):
        index = int(item) % len(self.table)
        first = index
        while True:
            # Slots marked as None have never been filled
            if self.table[index] is None:
                return None
            if self.table[index] is not False and self.table[index] == item:
                return self.table[index]

            index += 1  # increment
            if index >= len(self.table):  # at the end of the hash table
                index = 0  # reset the index to 0
            if index == first:
                return None

    def exists(self, item):
        index = int(item) % len(self.table)
        first = index
        while True:
            if self.table[index] is None:
                return False
            if self.table[index] is not False and self.table[index] == item:
                return True
            index += 1
            if index >= len(self.table):
                index = 0
            if index == first:
                return False
        return False

    def Size(self):
        count = 0
        for n in self.table:
            if n != False and n is not None:
                count += 1
        return count

    def Traverse(self, callback):
        for n in self.table:
            if n != False and n is not None:
                callback(n)

    def print_item(self, item):
        print item

    def print_int(self, item):
        print int(item)

    def Delete(self, item):
        index = int(item) % len(self.table)
        first = index
        while True:
            if self.table[index] is None:
                return False
            if self.table[index] is not False and self.table[index] == item:
                # when deleting set the space to False instead of None
                self.table[index] = False
                return True
            index += 1  # increment

            if index >= len(self.table):  # at the end of the hash table
                index = 0  # reset the index to 0
            if index == first:
                return False