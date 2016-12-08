#!/usr/bin/env python3

prices = [ "A", "B", "C", "D" ] 
sizes = [ "E", "F", "G", "H" ] 
customers = [ "I", "K", "P", "Z" ] 

## a customer must have only one home size
#for person in customers:     
#    for size in sizes:         
#        s = "tell (%s%s) <=> (" % (person, size)         
#        first = True         
#        for size2 in sizes:             
#            if size2 != size:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (person, size2)                 
#                first = False         
#        s += ")"         
#        print(s)
# #a customer must have at least one home size
#for person in customers:     
#    s = "tell ("     
#    first = True     
#    for size in sizes:         
#        if not first:             
#            s += " | "         
#        s += "%s%s" % (person, size)         
#        first = False     
#    s += ")"     
#    print(s)    
### a size can have at most one customer 
#for size in sizes:     
#    for person in customers:         
#        s = "tell (%s%s) <=> (" % (person, size)         
#        first = True         
#        for person2 in customers:             
#            if person2 != person:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (person2, size)                 
#                first = False         
#        s += ")"         
#        print(s)
### a size must have at least one customer 
#for size in sizes:     
#    s = "tell ("     
#    first = True     
#    for person in customers:         
#        if not first:
#            s += " | "         
#        s += "%s%s" % (person, size)         
#        first = False     
#    s += ")"     
#    print(s)

### when a customer has a home size and price

### when a customer has a home size and price   
##for size in sizes:      
##    for price in prices:                      
##        s = "dpll (%s%s%s)" % ("K", size, price)               
##        print(s)
##        print("echo")
##for size in sizes:         
##    for price in prices:                      
##        s = "dpll !(%s%s%s)" % ("K", size, price)               
##        print(s)
##        print("echo")
##for person in customers:         
##    for price in prices:                      
##        s = "dpll (%s%s%s)" % (person, "F", price)               
##        print(s)
##        print("echo")
##for person in customers:         
##    for price in prices:                      
##        s = "dpll !(%s%s%s)" % (person, "F", price)               
##        print(s)
##        print("echo")
##for person in customers:         
##    for size in sizes:                      
##        s = "dpll (%s%s%s)" % (person, size, "A")               
##        print(s)
##        print("echo")
##for person in customers:         
##    for size in sizes:                      
##        s = "dpll !(%s%s%s)" % (person, size, "A")               
##        print(s)
##        print("echo")
##for price in prices:         
##    for size in sizes:                      
##        s = "dpll (%s%s%s)" % ("P", size, price)               
##        print(s)
##        print("echo")
##for price in prices:         
##    for size in sizes:                      
##        s = "dpll !(%s%s%s)" % ("P", size, price)               
##        print(s)
##        print("echo")
## a customer must have only one home price
#for person in customers:     
#    for price in prices:         
#        s = "tell (%s%s) <=> (" % (person, price)         
#        first = True         
#        for price2 in prices:             
#            if price2 != price:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (person, price2)                 
#                first = False         
#        s += ")"         
#        print(s)
# #a customer must have at least one home price
#for person in customers:     
#    s = "tell ("     
#    first = True     
#    for price in prices:         
#        if not first:             
#            s += " | "         
#        s += "%s%s" % (person, price)         
#        first = False     
#    s += ")"     
#    print(s)
##have at most one customer 
#for price in prices:     
#    for person in customers:         
#        s = "tell (%s%s) <=> (" % (person, price)         
#        first = True         
#        for person2 in customers:             
#            if person2 != person:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (person2, price)                 
#                first = False         
#        s += ")"         
#        print(s)
### a price must have at least one customer 
#for price in prices:     
#    s = "tell ("     
#    first = True     
#    for person in customers:         
#        if not first:
#            s += " | "         
#        s += "%s%s" % (person, price)         
#        first = False     
#    s += ")"     
#    print(s)

### a price can have at most one size 
#for price in prices:     
#    for size in sizes:         
#        s = "tell (%s%s) <=> (" % (price, size)         
#        first = True         
#        for price2 in prices:             
#            if price2 != price:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (price2, size)                 
#                first = False         
#        s += ")"         
#        print(s)
### a price must have at least one size 
#for price in prices:     
#    s = "tell ("     
#    first = True     
#    for size in sizes:         
#        if not first:
#            s += " | "         
#        s += "%s%s" % (price, size)         
#        first = False     
#    s += ")"     
#    print(s)

### a size can have at most one price 
#for size in sizes:         
#    for price in prices:     
#        s = "tell (%s%s) <=> (" % (price, size)         
#        first = True         
#        for price2 in prices:             
#            if price2 != price:                 
#                if not first:                     
#                    s += " & "                 
#                s += "(!%s%s)" % (price2, size)                 
#                first = False         
#        s += ")"         
#        print(s)
### a size must have at least one price 
#for size in sizes:         
#    s = "tell ("     
#    first = True     
#    for price in prices:     
#        if not first:
#            s += " | "         
#        s += "%s%s" % (price, size)         
#        first = False     
#    s += ")"     
#    print(s)
for size in sizes:         
        for price in prices:                
                s = "dpll (%s%s)" % (price, size)               
                print(s)
for person in customers:         
        for price in prices:                
                s = "dpll (%s%s)" % (person, price)               
                print(s)
for person in customers:         
        for size in sizes:                
                s = "dpll (%s%s)" % (person, size)               
                print(s)
for size in sizes:         
        for price in prices:                
                s = "dpll (!%s%s)" % (price, size)               
                print(s)
for person in customers:         
        for price in prices:                
                s = "dpll (!%s%s)" % (person, price)               
                print(s)
for person in customers:         
        for size in sizes:                
                s = "dpll (!%s%s)" % (person, size)               
                print(s)
for person in customers:     
    for size in sizes:         
        for price in prices:            
                s = "tell !(%s%s & %s%s) <=> (!%s%s%s)" % (person, size, price, size, person, size, price)               
                print(s)
for person in customers:     
    for size in sizes:         
        for price in prices:                    
                s = "tell (%s%s & %s%s) <=> (%s%s%s)" % (person, size, person, price, person, size, price)               
                print(s)
